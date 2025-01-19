
#include <asf.h>
#include "main.h"

	volatile uint32_t ticks = 0; // Time since Start (ms)
    comm commStack;
	current currentData;
	mbComm modbusComm;
	mbDataMapping mbDaten;

int main (void)
{
	system_init();
	SysTick_Config(48000000 / 1000);

	uartInit();
//	tc0TimerInit();

	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction  = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(PIN_PA14, &config_port_pin); // Current
	
	uint32_t readwritetime = ticks;
	uint32_t counterTime = ticks;
	uint32_t gasImpLastState = 0;
	
	while (1)
	{
	
		if (ticks >= counterTime + 100) // Entprellzeit
		{
			counterTime = ticks; 
			if (!port_pin_get_input_level(PIN_PA14) && !(gasImpLastState == 1))
			{
				gasImpLastState = 1;
				currentData.gasImp++;
			}
			else if (port_pin_get_input_level(PIN_PA14))
			{
				gasImpLastState = 0;
			}

		}
		
		if (ticks >= readwritetime + 1000)
		{
			readwritetime = ticks;
			
			mbDaten.holdingRegisters[0] = 0;
			mbDaten.holdingRegisters[1] = currentData.gasImp;
			if (currentData.strom > 0)
			{
				mbDaten.holdingRegisters[3] = (uint16_t)(currentData.strom >> 16);
				mbDaten.holdingRegisters[4] = (uint16_t)currentData.strom;
			}
			mbDaten.holdingRegisters[5] = (uint16_t)(currentData.leistung >> 16);
			mbDaten.holdingRegisters[6] = (uint16_t)currentData.leistung;
			currentData.gasImp = 0;
			currentData.stromImp = 0;
		}
		if (commStack.rx0Done)
		{
			zaehlerstandSuchen(&currentData);
		}	
		modbusSlaveCyclic(&modbusComm ,&mbDaten);

	}
}

void SysTick_Handler(){
	ticks++;
	MBTimer();
}










/*
		if (ticks >= (ms50)){
			ms50 = ticks + 50;
			reTriggerTc1hUS(50);
			PORT->Group->OUTSET.reg = 0x10;
		}
		if (checkTimer())
		{
			
	//		TC1->COUNT16.INTFLAG.bit.OVF = 1;
			PORT->Group->OUTCLR.reg = 0x10;			
		}
*/

/* //Test Pin Config
    struct port_config config_port_pin;
    port_get_config_defaults(&config_port_pin);
    config_port_pin.direction  = PORT_PIN_DIR_INPUT;
    config_port_pin.input_pull = PORT_PIN_PULL_UP;
    config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
    port_pin_set_config(PIN_PA04, &config_port_pin);
*/

//	uint32_t ms50 = ticks;