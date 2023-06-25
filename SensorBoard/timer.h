/*
 * timer.h
 *
 * Created: 21.01.2022 20:05:15
 *  Author: Radtke
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <asf.h>
#include <stdint.h>
#include "structs.h"

void tc0TimerInit(void);
void reTriggerTc1hUS(uint16_t);
void stopTC1(void);

#endif /* TIMER_H_ */