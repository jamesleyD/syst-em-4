#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include <stdio.h>

int uart_putchar(char, FILE *);
int uart_getchar(FILE *);
void uart_io_init(uint32_t);

int uart_putchar(char c, FILE *stream) {
    UART__putc(c);
    return 0;
}

int uart_getchar(FILE *stream) {
    char c = UART__getc();
    // En utilisant screen, faire entrée envoie un retour chariot, il faut donc le changer en retour à la ligne
    // puisque gets en attend un pour terminer le remplissage du buffer
    if (c == '\r') {
        c = '\n';
    }
    return c;
}

void uart_io_init(uint32_t baud_rate) {
    UART__init(baud_rate, 0);

    // On crée des flux de données qui permettront d'être utilisées à l'écriture/lecture
    // Cette macro nous permet de créer une struct FILE qui utilise une fonction qu'on lui donne en paramètre
    static FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
    static FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

    // On redirige les entrée et sortie standards vers les flux que nous venons de créer
    stdout = &uart_output;
    stdin = &uart_input;
}


int main(void) {
    uart_io_init(9600);

    char buffer[128];

    while(1){
        gets(buffer);
        printf(buffer);
    }
}