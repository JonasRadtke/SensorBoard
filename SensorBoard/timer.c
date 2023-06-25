/*
 * timer.c
 *
 * Created: 21.01.2022 20:05:03
 *  Author: Radtke
 */ 

#include "timer.h"

extern	comm commStack;

void tc0TimerInit(void){
	struct tc_module tc_instance;
	struct tc_config config_tc;
	tc_get_config_defaults(&config_tc);
	config_tc.clock_source = GCLK_GENERATOR_0;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV1;
	config_tc.counter_size    = TC_COUNTER_SIZE_16BIT;
	config_tc.wave_generation = TC_CTRLA_WAVEGEN_MFRQ;
	config_tc.counter_16_bit.compare_capture_channel[0] = ((48000000UL/1000000UL)*100);
	config_tc.oneshot = 1;
	tc_init(&tc_instance, TC1, &config_tc);
	TC1->COUNT16.INTENSET.bit.OVF = 1; // Overflow aktivieren
	system_interrupt_enable(TC1_IRQn);
	tc_enable(&tc_instance);
	TC1->COUNT16.CTRLBSET.reg = TC_CTRLBSET_CMD_STOP;
	
	return;
}

void reTriggerTc1hUS(uint16_t x){
	TC1->COUNT16.CTRLBSET.bit.CMD = 0x2; // Stop Timmer
	TC1->COUNT16.CTRLBSET.reg = TC_CTRLBSET_CMD_RETRIGGER| TC_CTRLBSET_ONESHOT; // Retrigger and One Shot
	TC1->COUNT16.CC->reg = ((48000000UL/1000000UL)*x); // New "time"
	return;
}

void stopTC1(){
	TC1->COUNT16.CTRLBSET.bit.CMD = 0x2;	// Stop Timer
	return;
}

void TC1_Handler(void){
	if (TC1->COUNT16.INTFLAG.bit.OVF) // If Timer overflow
	{
		TC1->COUNT16.CTRLBSET.bit.CMD = 0x2; // Stop Timer
		TC1->COUNT16.INTFLAG.bit.OVF = 1; // Reset Interupt flag
		commStack.timeDone = 1;	// Time up
	}
	return;
}

