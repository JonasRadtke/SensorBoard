/*
 * protocol.h
 *
 * Created: 25.01.2022 18:33:29
 *  Author: Radtke
 */ 


#ifndef MODBUSSLAVE_H_
#define MODBUSSLAVE_H_

#include <asf.h>
#include <stdint.h>
#include "uartt.h"
#include "structs.h"

void procotolRoutine(comm *);
uint16_t crc16(uint8_t * , uint8_t);

#endif /* PROTOCOL_H_ */