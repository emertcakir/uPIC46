#include "../config_bits.h"
#include <xc.h>
#include <stdint.h>
#include "../Inc/uart_common.h"
#define _XTAL_FREQ 8000000UL

// I2C bit-bang pins (SDA -> RB4, SCL -> RB5)
#define SDA_TRIS TRISBbits.TRISB4
#define SDA_LAT LATBbits.LATB4
#define SDA_PORT PORTBbits.RB4
#define SCL_TRIS TRISBbits.TRISB5
#define SCL_LAT LATBbits.LATB5

#define EEPROM_ADDR 0x50 // 24LC256 with A2,A1,A0 = GND

static inline void i2c_init(void){ ANSELB = 0; SDA_TRIS = 1; SCL_TRIS = 1; }
static inline void i2c_delay(void){ __delay_us(5); }
static inline void sda_high(void){ SDA_TRIS = 1; } // release (pull-up)
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
        i2c_delay();
        scl_high(); i2c_delay();
        scl_low(); i2c_delay();
        b <<= 1;
    }
    // ack
    sda_high();
    i2c_delay();
    scl_high(); i2c_delay();
    uint8_t ack = (SDA_PORT == 0);
    scl_low(); i2c_delay();
    return ack;
}
uint8_t i2c_read_byte(uint8_t ack){
    uint8_t v=0;
    sda_high();
    for(int i=0;i<8;i++){
        v <<= 1;
        scl_high(); i2c_delay();
        if(SDA_PORT) v |= 1;
        scl_low(); i2c_delay();
    }
    // send ack/nack
    if(ack) sda_low(); else sda_high();
    i2c_delay();
    scl_high(); i2c_delay();
    scl_low(); i2c_delay();
    sda_high();
    return v;
}

void eeprom_write_page(uint16_t addr, uint8_t *buf, uint8_t len){
    i2c_start();
    i2c_write_byte((EEPROM_ADDR<<1) | 0); // write
    i2c_write_byte((uint8_t)(addr>>8));
    i2c_write_byte((uint8_t)(addr & 0xFF));
    for(int i=0;i<len;i++) i2c_write_byte(buf[i]);
    i2c_stop();
    __delay_ms(10); // write cycle
}

void eeprom_read(uint16_t addr, uint8_t *buf, uint16_t len){
    i2c_start();
    i2c_write_byte((EEPROM_ADDR<<1) | 0);
    i2c_write_byte((uint8_t)(addr>>8));
    i2c_write_byte((uint8_t)(addr & 0xFF));
    i2c_start();
    i2c_write_byte((EEPROM_ADDR<<1) | 1);
    for(int i=0;i<len;i++) buf[i] = i2c_read_byte(i < (len-1));
    i2c_stop();
}

int main(void){
    ANSELB = 0; ANSELC = 0; ANSELD = 0;
    uart_init();
    uart_puts("uPIC46 EEPROM test (24LC256)\r\n");
    i2c_init();
    uint8_t wbuf[16];
    for(int i=0;i<16;i++) wbuf[i] = 0xA0 + i;
    eeprom_write_page(0x0000, wbuf, 16);
    uart_puts("Wrote 16 bytes to EEPROM, reading back...\r\n");
    uint8_t rbuf[16];
    eeprom_read(0x0000, rbuf, 16);
    for(int i=0;i<16;i++){
        uart_puthex(rbuf[i]);
        uart_puts(" ");
    }
    uart_puts("\r\nDone.\r\n");
    while(1) { __delay_ms(1000); }
    return 0;
}
