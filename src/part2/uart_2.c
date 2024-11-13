#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ring_buffer.h"

void UART__init(uint32_t);
uint8_t UART__getc(void);
void UART__putc(uint8_t);

void UART__init(uint32_t baud_rate) {
    uint16_t ubrr = F_CPU / 16 / baud_rate - 1;
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t) ubrr;

    // Activation de RX et TX
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    UCSR0C = 0
    // 1 bit de stop
    | (0 << USBS0)
    // 8 bits de données
    | (1 << UCSZ00) | (1 << UCSZ01)
    // pas de bit de parité
    | (0<<UPM01) | (0<<UPM00);
}

uint8_t UART__getc(void){
    // Attendre de recevoir des données
    while (!(UCSR0A & (1<<RXC0)));

    return UDR0;
}

void UART__putc(uint8_t c){
    // Attendre que le buffer soit vide
    while (!(UCSR0A & (1<<UDRE0)));

    // Envoyer le char
    UDR0 = c;
}