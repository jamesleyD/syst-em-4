#include "uart_2.c"

int main(void) {
    UART__init(9600);
    DDRB |= _BV(PORTB5);

    while (1) {
        uint8_t c = UART__getc();
        UART__putc(c);
        PORTB |= _BV(PORTB5);
        _delay_ms(5000);
        PORTB &= ~_BV(PORTB5);
    }
}