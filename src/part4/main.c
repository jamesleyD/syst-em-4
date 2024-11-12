#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include <stdio.h>


int uart_putchar(char c, FILE *stream) {
    UART__putc(c);
    return 0;
}

int uart_getchar(FILE *stream) {
    char c = UART__getc();
    if (c == '\r') {
        c = '\n';
    }
    return c;
}

void uart_io_init(uint32_t baud_rate) {
    UART__init(baud_rate);

    static FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
    static FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

    stdout = &uart_output;
    stdin = &uart_input;
}


int main(void) {
    uart_io_init(9600);

    char buffer[64];

    while(1){
        char c = getchar();
        printf(&c);
    }
}