#include <avr/io.h>
#include <util/delay.h>

void UART_init(uint32_t baud_rate);
void UART_putc(uint8_t c);

void UART_init(uint32_t baud_rate) {
    // Calcul de la valeur UBRR pour le baud rate souhaité
    uint16_t ubrr = F_CPU / 16 / baud_rate - 1;
    // Définir le baud rate
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    // Activer l'émetteur (TX)
    UCSR0B = (1 << TXEN0);
    // Configurer le format des données : 8 bits, 1 bit de stop, sans parité
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_putc(uint8_t c) {
    // Attendre que le buffer d'envoi soit vide
    while (!(UCSR0A & (1 << UDRE0)));
    // Envoyer le caractère
    UDR0 = c;
}

int main(void) {
    // Initialiser l'UART avec un baud rate de 9600
    UART_init(9600);

    while (1) {
        // Envoyer la commande 'A' pour allumer la LED
        UART_putc('A');
        // Attendre 1 seconde
        _delay_ms(1000);

        // Envoyer la commande 'E' pour éteindre la LED
        UART_putc('E');
        // Attendre 1 seconde
        _delay_ms(1000);
    }

    return 0;
}
