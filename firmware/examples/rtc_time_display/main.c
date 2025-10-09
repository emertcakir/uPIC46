#include "../config_bits.h"
#include <xc.h>
#include <stdint.h>
#include "../Inc/uart_common.h"
#define _XTAL_FREQ 8000000UL

// I2C pins: SDA->RB4, SCL->RB5 (same as EEPROM example)
#define SDA_TRIS TRISBbits.TRISB4
#define SDA_LAT LATBbits.LATB4
#define SDA_PORT PORTBbits.RB4
#define SCL_TRIS TRISBbits.TRISB5
#define SCL_LAT LATBbits.LATB5
#define DS3231_ADDR 0x68

static inline void i2c_delay(void){ __delay_us(5); }
static inline void sda_high(void){ SDA_TRIS = 1; }
static inline void sda_low(void){ SDA_TRIS = 0; SDA_LAT = 0; }
static inline void scl_high(void){ SCL_TRIS = 1; }
static inline void scl_low(void){ SCL_TRIS = 0; SCL_LAT = 0; }

void i2c_start(void){
    sda_high(); scl_high(); i2c_delay();
    sda_low(); i2c_delay();
    scl_low(); i2c_delay();
}
void i2c_stop(void){
    sda_low(); i2c_delay();
    scl_high(); i2c_delay();
    sda_high(); i2c_delay();
}
uint8_t i2c_write_byte(uint8_t b){
    for(int i=0;i<8;i++){
        if(b & 0x80) sda_high(); else sda_low();
        i2c_delay(); scl_high(); i2c_delay(); scl_low(); i2c_delay();
        b <<= 1;
    }
    sda_high(); i2c_delay(); scl_high(); i2c_delay();
    uint8_t ack = (SDA_PORT == 0);
    scl_low(); i2c_delay();
    return ack;
}
uint8_t i2c_read_byte(uint8_t ack){
    uint8_t v = 0;
    sda_high();
    for(int i=0;i<8;i++){
        v <<= 1;
        scl_high(); i2c_delay();
        if(SDA_PORT) v |= 1;
        scl_low(); i2c_delay();
    }
    // ack
    if(ack) sda_low(); else sda_high();
    i2c_delay(); scl_high(); i2c_delay(); scl_low(); i2c_delay(); sda_high();
    return v;
}

uint8_t bcd2bin(uint8_t b){ return ((b>>4)*10) + (b & 0x0F); }

void ds3231_read_time(uint8_t *hh, uint8_t *mm, uint8_t *ss){
    i2c_start();
    i2c_write_byte((DS3231_ADDR<<1) | 0);
    i2c_write_byte(0x00);
    i2c_start();
    i2c_write_byte((DS3231_ADDR<<1) | 1);
    uint8_t s = i2c_read_byte(1);
    uint8_t m = i2c_read_byte(1);
    uint8_t h = i2c_read_byte(0);
    i2c_stop();
    *ss = bcd2bin(s);
    *mm = bcd2bin(m);
    *hh = bcd2bin(h & 0x3F);
}

int main(void){
    ANSELB=0; ANSELC=0; ANSELD=0;
    uart_init();
    uart_puts("uPIC46 DS3231 RTC read example\r\n");
    while(1){
        uint8_t hh, mm, ss;
        ds3231_read_time(&hh, &mm, &ss);
        // print HH:MM:SS
        if(hh<10) uart_putc('0'); uart_putc('0'+(hh/10)); uart_putc('0'+(hh%10));
        uart_putc(':');
        uart_putc('0'+(mm/10)); uart_putc('0'+(mm%10));
        uart_putc(':');
        uart_putc('0'+(ss/10)); uart_putc('0'+(ss%10));
        uart_puts("\r\n");
        __delay_ms(1000);
    }
    return 0;
}
