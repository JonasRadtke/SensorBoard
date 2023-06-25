/*
 * uart.h
 *
 * Created: 22.05.2017 11:00:07
 *  Author: Jonas Radtke
 */ 


#ifndef UARTT_H_
#define UARTT_H_

#include "asf.h"
#include "structs.h"
#include "modbusSlave.h"
#include "timer.h"
#include <stdint.h>

void uartInit(void);

void sendByteSercom0(uint8_t);
uint32_t sendArraySercom0(comm *);
void sendByteSercom1(uint8_t);
uint32_t sendArraySercom1(comm *);

uint32_t checkTimer(void);


#endif /* UART_H_ */