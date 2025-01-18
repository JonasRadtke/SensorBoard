
#ifndef UARTT_H_
#define UARTT_H_

#include <stdint.h>
#include "asf.h"
#include "structs.h"
#include "modbusSlave.h"


void uartInit(void);
void sendByteSercom0(uint8_t);
uint32_t sendArraySercom0(comm *);
void sendByteSercom1(uint8_t);
uint32_t sendArraySercom1(uint8_t *, uint8_t);

uint32_t zaehlerstandSuchen(current *);

#endif /* UART_H_ */