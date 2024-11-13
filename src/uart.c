#include <avr/io.h>
#include <avr/interrupt.h>
#include "ring_buffer.h"
#include "uart.h"

#define RING_BUFFER_SIZE 64

// Déclaration des variables globales pour le buffer circulaire
volatile struct ring_buffer rb;
volatile uint8_t rb_buffer[RING_BUFFER_SIZE];

void UART__init(uint32_t baud_rate, short withInterrupt) {
    // Initialisation du buffer circulaire
    ring_buffer__init(
        (struct ring_buffer*) &rb,
        (uint8_t *) rb_buffer,
        RING_BUFFER_SIZE);

    // La valeur de UBRR est calculée en fonction de l'horloge (ici 16 mHZ) et du baud rate souhaité
    // Elle est définit par la formule suivante
    uint16_t ubrr = F_CPU / 16 / baud_rate - 1;
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t) ubrr;


    // Activation de RX, TX et de l'interruption RX Complete
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    
    UCSR0C = 0
      // 1 bit de stop
      | (0 << USBS0)
      // 8 bits de données
      | (1 << UCSZ00) | (1 << UCSZ01)
      // pas de bit de parité
      | (0<<UPM01) | (0<<UPM00);

    // Activation des interruptions globales si besoin
    if (withInterrupt){
        sei();
    }
}

uint8_t UART__getc(){
    // UCSR0A contient des indicateurs sur l'état de la transmission et de la réception
    // le bit RXC0 est mis à 1 lorsque des données ont été reçues et sont prêtes à être lues dans UDR0
    // la boucle continue tant que RXCO n'est pas à 1 (cad tant que les données n'ont pas été recus)
    while (!(UCSR0A & (1<<RXC0)));

    // retourne le caractère
    return UDR0;
}

void UART__putc(uint8_t c) {
    // UCSR0A contient des indicateurs sur l'état de la transmission et de la réception
    // le bit UDREO est mis à 1 lorsque le registre de données d'envoi (UDR0) est vide et peut recevoir des données
    // la boucle continue tant que UDREO n'est pas à 1 (cad tant que UDR0 n'est pas prêt)
    while (!(UCSR0A & (1<<UDRE0)));

    // Envoyer le char
    UDR0 = c;
}

uint8_t UART__pop(uint8_t* data) {
    uint8_t result;
    // Désactiver les interruptions pour éviter les problèmes de concurrence
    cli();
    result = ring_buffer__pop((struct ring_buffer*) &rb, data);
    // Réactiver les interruptions
    sei();
    return result;
}

// ISR pour l'interruption RX
ISR(USART_RX_vect) {
    // Lire le caractère reçu
    uint8_t data = UDR0;
    // Le stocker dans le buffer circulaire
    ring_buffer__push((struct ring_buffer*) &rb, data);
}