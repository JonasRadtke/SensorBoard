/**
* \file
*
* \brief Empty user application template
*
*/

/**
* \mainpage User Application template doxygen documentation
*
* \par Empty user application template
*
* Bare minimum empty user application template
*
* \par Content
*
* -# Include the ASF header files (through asf.h)
* -# Minimal main function that starts with a call to system_init()
* -# "Insert application code here" comment
*
*/

/*
* Include header files for all drivers that have been imported from
* Atmel Software Framework (ASF).
*/
/*
* Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
*/
#include <asf.h>
#include "main.h"

	volatile uint32_t ticks = 0; // Time since Start (ms)
	comm commStack;
	current currentData;

int main (void)
{
	system_init();
	SysTick_Config(48000000 / 1000);

	uartInit();
	tc0TimerInit();

	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction  = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(PIN_PA14, &config_port_pin); // Current
	port_pin_set_config(PIN_PA15, &config_port_pin); // Gas
	
	uint32_t readwritetime = ticks;
	uint32_t counterTime = ticks;
	uint32_t gasImpLastState = 0;
	uint32_t stromImpLastState = 0;
	
	while (1)
	{
	
		if (ticks >= counterTime + 20) // Entprellzeit
		{
			counterTime = ticks; 
			if (!port_pin_get_input_level(PIN_PA15) && !(gasImpLastState == 1))
			{
				gasImpLastState = 1;
				currentData.gasImp++;
			}
			else if(port_pin_get_input_level(PIN_PA15))
			{
				gasImpLastState = 0;
			}
			if (!port_pin_get_input_level(PIN_PA14) && !(stromImpLastState == 1))
			{
				stromImpLastState = 1;
				currentData.stromImp++;
			}
			else if (port_pin_get_input_level(PIN_PA14))
			{
				stromImpLastState = 0;
			}

		}
		
		
		if (ticks >= readwritetime + 1000)
		{
			readwritetime = ticks;
			
			commStack.outputregisters[0] = currentData.gasImp;
			commStack.outputregisters[1] = currentData.stromImp;
			currentData.gasImp = 0;
			currentData.stromImp = 0;
			
			
		}

		procotolRoutine(&commStack);

	}
}

void SysTick_Handler(){
	ticks++;

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