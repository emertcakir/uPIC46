uPIC46 Example Firmware Collection

This archive contains simple example projects for the uPIC46 development board (PIC18F46K22).
Each example is self-contained (single-file main.c) and intended to be imported into MPLAB X (XC8).
Adjust configuration bits in MPLAB X Project -> Configuration Bits before programming the device.

Assumptions used in the examples:
- MCU clock: external 8 MHz crystal (update _XTAL_FREQ if different)
- LED1 -> RC0
- LED2 -> RD3
- UART1 TX -> RC6 (EUSART TX)
- UART1 RX -> RC7 (EUSART RX)
- I2C (bit-banged) SDA -> RB4, SCL -> RB5
- SPI (bit-banged for MCP2515) SCK -> RD0, MOSI -> RD4, MISO -> RD1, CS -> RD2
- RS485 DE (if used) -> RB3

These examples are for demonstration and testing; they are not production-ready drivers.
