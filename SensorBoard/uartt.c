
#include "uartt.h"

extern comm commStack;

void uartInit()
{	

	struct usart_module usart_instance1;
	struct usart_config config_usart;

	//Sercom 0 Stromzähler
	usart_get_config_defaults(&config_usart);
	config_usart.baudrate    = 9800;
	config_usart.pinmux_pad0 = PINMUX_UNUSED;
	config_usart.pinmux_pad1 = PINMUX_PA15C_SERCOM0_PAD1;
	config_usart.pinmux_pad2 = PINMUX_UNUSED;
	config_usart.pinmux_pad3 = PINMUX_UNUSED;
	while (usart_init(&usart_instance1, SERCOM0, &config_usart) != STATUS_OK){}
	SERCOM0->USART.CTRLA.bit.RXPO = 0x1; // Pad 1
//	SERCOM1->USART.CTRLA.bit.TXPO = 0x1; // Pad 2
	usart_enable(&usart_instance1);
	SERCOM0->USART.INTENSET.bit.RXC = 1;
	system_interrupt_enable(SERCOM0_IRQn);
//	SERCOM1->USART.CTRLB.bit.TXEN = 1;

	//Sercom 1 FTDI Modbus
	usart_get_config_defaults(&config_usart);
	config_usart.baudrate    = MODBUSBAUDRATE;
	config_usart.pinmux_pad0 = PINMUX_PA22C_SERCOM1_PAD0;
	config_usart.pinmux_pad1 = PINMUX_UNUSED;
	config_usart.pinmux_pad2 = PINMUX_PA24C_SERCOM1_PAD2;
	config_usart.pinmux_pad3 = PINMUX_UNUSED;
	while (usart_init(&usart_instance1, SERCOM1, &config_usart) != STATUS_OK){}
	SERCOM1->USART.CTRLA.bit.RXPO = 0x0; // Pad 0
	SERCOM1->USART.CTRLA.bit.TXPO = 0x1; // Pad 2
	usart_enable(&usart_instance1);
	SERCOM1->USART.INTENSET.bit.RXC = 1;
	system_interrupt_enable(SERCOM1_IRQn);
	SERCOM1->USART.CTRLB.bit.TXEN = 1;

	return;
}


// Byte Sercom 0 senden
void sendByteSercom0(uint8_t c)
{
	while (!SERCOM0->USART.INTFLAG.bit.TXC && !SERCOM0->USART.INTFLAG.bit.DRE );
	
	SERCOM0->USART.DATA.reg = c;
	return;
}

// Byte Sercom 1 senden
void sendByteSercom1 (uint8_t c)
{
	while (!SERCOM1->USART.INTFLAG.bit.TXC && !SERCOM1->USART.INTFLAG.bit.DRE );
	
	SERCOM1->USART.DATA.reg = c;
	return;
}

// Array an sercom 0 senden
uint32_t sendArraySercom0(comm *communication)	// Uart senden eines Strings
{
	uint8_t i = 0;
	while (i < communication->tx0Length) // Sende bis Ende erreicht
	{			
		sendByteSercom0(communication->tx0Buffer[i]);		// Zeichen senden
		i++;					// nächstes zeichen
	}
	while(!SERCOM0->USART.INTFLAG.bit.TXC); // Warte bis SendeBuffer leer
	
	i = SERCOM0->USART.DATA.reg;

	return 1;
}

// Array an sercom 1 senden
uint32_t sendArraySercom1(uint8_t *x, uint8_t txLength)	// Uart senden eines Strings
{
	uint8_t i = 0;
	while (i < txLength) // Sende bis Ende erreicht
	{
		sendByteSercom1(*x);		// Zeichen senden
		x++;
		i++;					// nächstes zeichen
	}
	while(!SERCOM1->USART.INTFLAG.bit.TXC); // Warte bis SendeBuffer leer

	i = SERCOM1->USART.DATA.reg;

	return 1;
}

//
void SERCOM0_Handler(){
	// Falls neues Zeichen empfangen wurde
	uint32_t i;
	if (SERCOM0->USART.INTFLAG.bit.RXC && !commStack.rx0Done)
	{
		commStack.rx0Buffer[commStack.rx0Length] = SERCOM0->USART.DATA.reg; // Zeichen in Buffer schreiben
		if (commStack.rx0Buffer[0] = 0x1B){
			commStack.rx0Length++;	// Länge um 1 erhöhen
		}
		else
		{
			commStack.rx0Length = 0;
		}
		
		if(commStack.rx0Length > 600)
		{
			commStack.rx0Length = 0;
		}
		commStack.rx0Active = 1;	// Empfang auf Aktiv setzen
		
		if ((commStack.rx0Buffer[0] == 0x1B) && (commStack.rx0Buffer[commStack.rx0Length - 4] == 0x1A) &&(commStack.rx0Buffer[commStack.rx0Length - 5] == 0x1B) && (commStack.rx0Buffer[commStack.rx0Length - 6] == 0x1B) && (commStack.rx0Buffer[commStack.rx0Length - 7] == 0x1B) && (commStack.rx0Buffer[commStack.rx0Length - 8] == 0x1B))
		{
			commStack.rx0Done = 1;
		}
		
	}
	else 
	{
		commStack.rx0Buffer[500] = SERCOM0->USART.DATA.reg;
	}
	return;
}

uint32_t zaehlerstandSuchen(current *x){
	
	uint32_t i;
	uint32_t strom = 0;
	uint32_t leistung = 0;
	uint8_t needle[] = { 0x77, 0x07, 0x01, 0x00, 0x01, 0x08, 0x00, 0xFF}; // Zählerstand
	uint8_t needleLeistung[] = { 0x77, 0x07, 0x01, 0x00, 0x10, 0x07, 0x00, 0xFF}; // Leistung	

	if (commStack.rx0Done)
	{	
		for (i=0; i<600; i++)
		{
			if ((commStack.rx0Buffer[i] == needle[0]) && (commStack.rx0Buffer[i+1] == needle[1]) && (commStack.rx0Buffer[i+2] == needle[2]) && (commStack.rx0Buffer[i+3] == needle[3])
			&&(commStack.rx0Buffer[i+4] == needle[4]) && (commStack.rx0Buffer[i+5] == needle[5]) && (commStack.rx0Buffer[i+6] == needle[6]) && (commStack.rx0Buffer[i+7] == needle[7]))
			{
				i = i;
				break;
			}
		}
		
		if (i < 599)
		{
			 strom = 0;
			 strom |= (((uint32_t)commStack.rx0Buffer[i + 23] << 24));// && 0xFF000000);
			 strom |= (((uint32_t)commStack.rx0Buffer[i + 24] << 16));// && 0x00FF0000);
			 strom |= (((uint32_t)commStack.rx0Buffer[i + 25] << 8));//  && 0x0000FF00);
			 strom |= (((uint32_t)commStack.rx0Buffer[i + 26]) );//      && 0x000000FF);			 
		}

		for (i=0; i<600; i++)
		{
			if ((commStack.rx0Buffer[i] == needleLeistung[0]) && (commStack.rx0Buffer[i+1] == needleLeistung[1]) && (commStack.rx0Buffer[i+2] == needleLeistung[2]) && (commStack.rx0Buffer[i+3] == needleLeistung[3])
			&&(commStack.rx0Buffer[i+4] == needleLeistung[4]) && (commStack.rx0Buffer[i+5] == needleLeistung[5]) && (commStack.rx0Buffer[i+6] == needleLeistung[6]) && (commStack.rx0Buffer[i+7] == needleLeistung[7]))
			{
				i = i;
				break;
			}
		}

		if (i < 599)
		{
			leistung = 0;
			leistung |= (((uint32_t)commStack.rx0Buffer[i + 15] << 24));// && 0xFF000000);
			leistung |= (((uint32_t)commStack.rx0Buffer[i + 16] << 16));// && 0x00FF0000);
			leistung |= (((uint32_t)commStack.rx0Buffer[i + 17] << 8));//  && 0x0000FF00);
			leistung |= (((uint32_t)commStack.rx0Buffer[i + 18]) );//      && 0x000000FF);
		}

		x->strom = strom;
		x->leistung = leistung;

		for (i=0; i<600; i++)
		{
			commStack.rx0Buffer[i] = 0;
		}
		commStack.rx0Done = 0;
		commStack.rx0Length = 0;
	}
	
	return 0;
}



