#include "../config_bits.h"
#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 8000000UL

// Pin mapping (uPIC46)
#define LED1_LAT LATCbits.LATC0
#define LED1_TRIS TRISCbits.TRISC0
#define LED2_LAT LATDbits.LATD3
#define LED2_TRIS TRISDbits.TRISD3

int main(void){
    // Make sure analogs disabled on ports used
    ANSELC = 0; ANSELD = 0; ANSELB = 0;
    LED1_TRIS = 0; // output
    LED2_TRIS = 0; // output
    LED1_LAT = 0;
    LED2_LAT = 0;
    while(1){
        LED1_LAT = 1;
        __delay_ms(250);
        LED1_LAT = 0;
        LED2_LAT = 1;
        __delay_ms(250);
        LED2_LAT = 0;
        __delay_ms(250);
    }
    return 0;
}
