// uart_common.h - small helper for EUSART1 (TX=RC6, RX=RC7)
#ifndef UART_COMMON_H
#define UART_COMMON_H
#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 8000000UL

static inline void uart_init(void){
    // Configure pins as digital
    ANSELC = 0; ANSELD = 0; ANSELB = 0;
    // TX on RC6, RX on RC7
    TRISCbits.TRISC6 = 0; // TX output
    TRISCbits.TRISC7 = 1; // RX input
    // Baud 115200 @ 8MHz: BRG16=1, BRGH=1 -> SPBRG = 16
    BAUDCONbits.BRG16 = 1;
    TXSTAbits.BRGH = 1;
    SPBRG = 16; SPBRGH = 0;
    RCSTAbits.SPEN = 1;  // Enable serial port (configures RX/TX pins)
    TXSTAbits.TXEN = 1;  // Enable transmitter
    RCSTAbits.CREN = 1;  // Enable continuous receive
}

static inline void uart_putc(char c){
    while(!PIR1bits.TXIF); // wait
    TXREG = c;
}

static inline void uart_puts(const char *s){
    while(*s) uart_putc(*s++);
}

static inline void uart_puthex(uint8_t v){
    const char hex[] = "0123456789ABCDEF";
    uart_putc(hex[v>>4]);
    uart_putc(hex[v & 0xF]);
}

#endif
