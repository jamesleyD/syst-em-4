#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"


int main(void) {
    DDRB = _BV(DDB5);
    PORTB = 0;

    // Initialise l'USART avec un baud rate de 9600
    UART__init(9600);

    for (;;) {
        uint8_t data;
        
        // pop un caractère du buffer
        if (!UART__pop(&data)) {
            // Si un caractère a été récupéré, l'envoyer
            UART__putc(data);
            // Allumer la LED
            PORTB |= _BV(PORTB5);
            // Attendre 500 ms
            _delay_ms(500);
            // Éteindre la LED
            PORTB &= ~_BV(PORTB5);
        }
    }
}
