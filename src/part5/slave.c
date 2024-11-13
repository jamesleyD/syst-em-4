#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void UART_init(uint32_t baud_rate);
void process_command(uint8_t cmd);

volatile uint8_t received_data = 0;
volatile uint8_t data_received_flag = 0;

void UART_init(uint32_t baud_rate) {
    uint16_t ubrr = F_CPU / 16 / baud_rate - 1;
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    // Activer le récepteur (RX) et l'interruption sur réception
    UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
    // 8 bits, 1 bit de stop, sans parité
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    // Activer les interruptions globales
    sei();
}

ISR(USART_RX_vect) {
    // Lire le caractère reçu
    received_data = UDR0;
    // Indiquer qu'un nouveau caractère a été reçu
    data_received_flag = 1;
}

void process_command(uint8_t cmd) {
    if (cmd == 'A') {
        PORTB |= (1 << PORTB5);
    } else if (cmd == 'E') {
        PORTB &= ~(1 << PORTB5);
    }
}

int main(void) {
    DDRB |= (1 << DDB5);
    PORTB &= ~(1 << PORTB5);

    UART_init(9600);

    while (1) {
        // Vérifier si un nouveau caractère a été reçu
        if (data_received_flag) {
            // Réinitialiser le flag
            data_received_flag = 0;
            // Traiter la commande
            process_command(received_data);
        }
    }

    return 0;
}
