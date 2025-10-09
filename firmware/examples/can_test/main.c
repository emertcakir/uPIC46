#include "../config_bits.h"
#include <xc.h>
#include <stdint.h>
#include "../Inc/uart_common.h"
#define _XTAL_FREQ 8000000UL

// SPI bit-bang pins for MCP2515
#define SPI_SCK_LAT LATDbits.LATD0
#define SPI_SCK_TRIS TRISDbits.TRISD0
#define SPI_MOSI_LAT LATDbits.LATD4
#define SPI_MOSI_TRIS TRISDbits.TRISD4
#define SPI_MISO_PORT PORTDbits.RD1
#define SPI_MISO_TRIS TRISDbits.TRISD1
#define SPI_CS_LAT LATDbits.LATD2
#define SPI_CS_TRIS TRISDbits.TRISD2

static inline void spi_init(void){
    ANSELD = 0; // digital
    SPI_SCK_TRIS = 0; SPI_MOSI_TRIS = 0; SPI_MISO_TRIS = 1; SPI_CS_TRIS = 0;
    SPI_CS_LAT = 1; SPI_SCK_LAT = 0; SPI_MOSI_LAT = 0;
}
void spi_delay(void){ __delay_us(2); }
uint8_t spi_transfer(uint8_t out){
    uint8_t in = 0;
    for(int i=0;i<8;i++){
        SPI_MOSI_LAT = (out & 0x80) ? 1 : 0;
        out <<= 1;
        spi_delay();
        SPI_SCK_LAT = 1; spi_delay();
        in <<= 1;
        if(SPI_MISO_PORT) in |= 1;
        SPI_SCK_LAT = 0; spi_delay();
    }
    return in;
}

// MCP2515 commands
#define MCP_RESET 0xC0
#define MCP_READ  0x03
#define MCP_WRITE 0x02
#define MCP_BITMOD 0x05
#define MCP_READ_STATUS 0xA0

void mcp_cs_low(){ SPI_CS_LAT = 0; __delay_us(1); }
void mcp_cs_high(){ SPI_CS_LAT = 1; __delay_us(1); }

void mcp_reset(void){
    mcp_cs_low();
    spi_transfer(MCP_RESET);
    mcp_cs_high();
    __delay_ms(10);
}
uint8_t mcp_read(uint8_t addr){
    uint8_t v;
    mcp_cs_low();
    spi_transfer(MCP_READ);
    spi_transfer(addr);
    v = spi_transfer(0);
    mcp_cs_high();
    return v;
}
void mcp_write(uint8_t addr, uint8_t val){
    mcp_cs_low();
    spi_transfer(MCP_WRITE);
    spi_transfer(addr);
    spi_transfer(val);
    mcp_cs_high();
}
void mcp_bitmod(uint8_t addr, uint8_t mask, uint8_t data){
    mcp_cs_low();
    spi_transfer(MCP_BITMOD);
    spi_transfer(addr);
    spi_transfer(mask);
    spi_transfer(data);
    mcp_cs_high();
}
uint8_t mcp_read_status(void){
    uint8_t v;
    mcp_cs_low();
    spi_transfer(MCP_READ_STATUS);
    v = spi_transfer(0);
    mcp_cs_high();
    return v;
}

int main(void){
    ANSELD = 0; ANSELB=0; ANSELC=0;
    uart_init();
    uart_puts("uPIC46 MCP2515 basic test\r\n");
    spi_init();
    mcp_reset();
    uart_puts("MCP reset done, reading CANCTRL/CANSTAT...\r\n");
    // CANCTRL at 0x0F, set loopback REQOP = 010 using BIT MODIFY (mask 0xE0)
    mcp_bitmod(0x0F, 0xE0, 0x40);
    __delay_ms(10);
    uint8_t canctrl = mcp_read(0x0F);
    uint8_t canstat = mcp_read(0x0E); // CANSTAT is 0x0E typically
    uart_puts("CANCTRL=0x"); uart_puthex(canctrl); uart_puts(" CANSTAT=0x"); uart_puthex(canstat); uart_puts("\r\n");
    uint8_t status = mcp_read_status();
    uart_puts("MCP READ STATUS: 0x"); uart_puthex(status); uart_puts("\r\n");
    while(1) __delay_ms(1000);
    return 0;
}
