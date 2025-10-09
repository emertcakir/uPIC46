#include "../config_bits.h"
#include <xc.h>
#include <stdint.h>
#include "../Inc/uart_common.h"
#define _XTAL_FREQ 8000000UL

#define LED1_LAT LATCbits.LATC0
#define LED1_TRIS TRISCbits.TRISC0

int main(void){
    // init
    ANSELC = 0; ANSELD = 0; ANSELB = 0;
    LED1_TRIS = 0; LED1_LAT = 0;
    uart_init();
    uart_puts("uPIC46 UART Echo example\r\n");
    while(1){
        if(PIR1bits.RCIF){
            char c = RCREG;
            // echo back
            uart_putc(c);
            // toggle LED so user can see activity
            LED1_LAT = ~LED1_LAT;
        }
    }
    return 0;
}
