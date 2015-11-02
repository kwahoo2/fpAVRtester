#include "uart.h"

void usart_init( uint16_t ubrr) {

    // Set baud rate

    UBRRH = (uint8_t)(ubrr>>8);
    UBRRL = (uint8_t)ubrr;

    // Enable receiver and transmitter

    UCSRB = (1<<RXEN)|(1<<TXEN);

    // Set frame format: 8data, 1stop bit

    UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

void usart_putchar(char data) {

    // Wait for empty transmit buffer

    while ( !(UCSRA & (_BV(UDRE))) );

    // Start transmission

    UDR = data;
}
char usart_getchar(void) {

    // Wait for incoming data

    while ( !(UCSRA & (_BV(RXC))) );

    // Return the data

    return UDR;
}

unsigned char usart_kbhit(void) {

    //return nonzero if char waiting polled version

    unsigned char b;
    b=0;
    if(UCSRA & (1<<RXC)) b=1;
    return b;
}

void usart_pstr(char *s) {

    // loop through entire string

    while (*s) {
        usart_putchar(*s);
        s++;
    }
}