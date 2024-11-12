#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void UART_init(uint32_t baud_rate);
void process_command(uint8_t cmd);

volatile uint8_t received_data = 0;
volatile uint8_t data_received_flag = 0;

void UART_init(uint32_t baud_rate) {
    // Calcul de la valeur UBRR pour le baud rate souhaité
    uint16_t ubrr = F_CPU / 16 / baud_rate - 1;
    // Définir le baud rate
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    // Activer le récepteur (RX) et l'interruption sur réception
    UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
    // Configurer le format des données : 8 bits, 1 bit de stop, sans parité
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
        // Allumer la LED (connectée à PB5 sur l'Arduino Uno)
        PORTB |= (1 << PORTB5);
    } else if (cmd == 'E') {
        // Éteindre la LED
        PORTB &= ~(1 << PORTB5);
    }
}

int main(void) {
    // Configurer PB5 (LED intégrée) comme sortie
    DDRB |= (1 << DDB5);
    // S'assurer que la LED est éteinte au départ
    PORTB &= ~(1 << PORTB5);

    // Initialiser l'UART avec un baud rate de 9600
    UART_init(9600);

    while (1) {
        // Vérifier si un nouveau caractère a été reçu
        if (data_received_flag) {
            // Réinitialiser le drapeau
            data_received_flag = 0;
            // Traiter la commande reçue
            process_command(received_data);
        }
    }

    return 0;
}
