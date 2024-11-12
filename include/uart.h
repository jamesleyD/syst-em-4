#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART__init(uint32_t baud_rate);
void UART__putc(uint8_t c);
uint8_t UART__getc(void);
uint8_t UART__pop(uint8_t* data);

#endif
