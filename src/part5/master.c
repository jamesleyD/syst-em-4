#include <avr/io.h>
#include <util/delay.h>

void UART_init(uint32_t baud_rate);
void UART_putc(uint8_t c);

const uint8_t bit_pattern[] = {1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};

void UART_init(uint32_t baud_rate) {
    uint16_t ubrr = F_CPU / 16 / baud_rate - 1;
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (1 << TXEN0);
    // 8 bits, 1 bit de stop, sans parité
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_putc(uint8_t c) {
    // Attendre que le buffer d'envoi soit vide
    while (!(UCSR0A & (1 << UDRE0)));
    // Envoyer le caractère
    UDR0 = c;
}

int main(void) {
    UART_init(9600);
    char index = 0;
    // Pas besoin de renvoyer une commande d'allumage si la LED est déjà allumée
    char lastSend = 0;
    while (1) {
        if (bit_pattern[index] == 1 && lastSend != 'A') {
            // Envoyer la commande 'A' pour allumer la LED
            UART_putc('A');
            lastSend = 'A';
        } else if (bit_pattern[index] == 0 && lastSend != 'E'){
            // Envoyer la commande 'E' pour éteindre la LED
            UART_putc('E');
            lastSend = 'E';
        }
        if (++index >= sizeof(bit_pattern)) {
            index = 0;
        }
        // Attendre 1 seconde
        _delay_ms(1000);
    }

    return 0;
}
