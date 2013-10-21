
#ifndef  NRF24L01_PRESENT
#define  NRF24L01_PRESENT

#include "hw_config.h"
#include <stm32f10x.h>

#define NRF24L01_SPI2_PORT		GPIOB

#define NRF24L01_SCK_PORT		NRF24L01_SPI2_PORT
#define NRF24L01_SCK_PIN		GPIO_Pin_13

#define NRF24L01_MISO_PORT		NRF24L01_SPI2_PORT
#define NRF24L01_MISO_PIN		GPIO_Pin_14

#define NRF24L01_MOSI_PORT		NRF24L01_SPI2_PORT
#define NRF24L01_MOSI_PIN		GPIO_Pin_15


#define NRF24L01_CS_PORT		GPIOC
#define NRF24L01_CS_PIN			GPIO_Pin_8

#define NRF24L01_CE_PORT		GPIOD
#define NRF24L01_CE_PIN			GPIO_Pin_3

#define NRF24L01_IRQ_PORT		GPIOC
#define NRF24L01_IRQ_PIN		GPIO_Pin_9

#define NRF24L01_CE PDout (3) 		// 24L01 chip select signal
#define NRF24L01_CSN PCout (8) 	// SPI Chip Select
#define NRF24L01_IRQ PCin (9) 		// IRQ host data input


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// NRF24L01 register operation commands
#define NRF24L01_READ_REG 0x00 // read configuration register, register address lower 5 bits
#define NRF24L01_WRITE_REG 0x20 // write configuration register, register address lower 5 bits
#define RD_RX_PLOAD 0x61 // read RX valid data, 1 ~ 32 bytes
#define WR_TX_PLOAD 0xA0 // Write TX valid data, 1 ~ 32 bytes
#define FLUSH_TX 0xE1 // Clear TX FIFO registers. Transmit mode with
#define FLUSH_RX 0xE2 // clear the RX FIFO registers. Receive mode with
#define REUSE_TX_PL 0xE3 // re-use on a packet of data, CE is high, the data packets are continuously sent.
#define NOP 0xFF // air operations, can be used to read status register

// SPI (NRF24L01) register address
// configuration register address; 
// bit0 : 1 receive mode, 0 transmit mode; 
// bit1: electric choice; 
// bit2: CRC mode; 
// bit3: CRC enabled;
// Bit4 : Interrupt MAX_RT (re-issued the maximum number of interrupt) is enabled; 
// bit5: Enable interrupt TX_DS; 
// bit6: Enable interrupt RX_DR
#define CONFIG 0x00 
#define EN_AA 0x01 // Enable auto answer bit0 ~ 5, corresponding to channel 0 to 5
#define EN_RXADDR 0x02 // recipient address allows, bit0 ~ 5, corresponding to channel 0 to 5
#define SETUP_AW 0x03 // set the address width (all data channels): bit1, 0:00,3 bytes; 01,4 bytes; 02,5 bytes;
#define SETUP_RETR 0x04 // set up automatic repeat; bit3: 0, automatic repeat counter; bit7: 4, automatic re-issued delay of 250 * x +86 us
#define RF_CH 0x05 // RF channel, bit6: 0, working channel frequency;
#define RF_SETUP 0x06 // RF registers; bit3: transfer rate (0:1 Mbps, 1:2 Mbps); bit2: 1, transmission power; bit0: low-noise amplifier gain
#define STATUS 0x07 // status register; bit0: TX FIFO full flag; bit3: 1, receive data channel number (maximum: 6); bit4, to achieve the best re-issued several times
                              // Bit5: data transmission completion interrupt; bit6: receive data interrupt;
#define MAX_TX 0x10 // send the maximum number of interrupt
#define TX_OK 0x20 // TX interrupt transmission completed
#define RX_OK 0x40 // receive data interrupt

#define OBSERVE_TX 0x08 // send a test registers, bit7: 4, packet loss counter; bit3: 0, retransmission counter
#define CD 0x09 // Carrier detect register, bit0, carrier detection;

#define RX_ADDR_P0 0x0A // Receive data channel 0 address, the maximum length of 5 bytes, low byte first
#define RX_ADDR_P1 0x0B // Data channel 1 receiver address, the maximum length of 5 bytes, low byte first
#define RX_ADDR_P2 0x0C // Receive data channel 2 address byte can be set low, high byte must be the same RX_ADDR_P1 [39:8] are equal;
#define RX_ADDR_P3 0x0D // data channel 3 receiver address, set the lowest byte, high byte must be the same RX_ADDR_P1 [39:8] are equal;
#define RX_ADDR_P4 0x0E // data channel 4 receiver address, set the lowest byte, high byte must be the same RX_ADDR_P1 [39:8] are equal;
#define RX_ADDR_P5 0x0F // data channel 5 to receive the address, set the lowest byte, high byte must be the same RX_ADDR_P1 [39:8] are equal;

#define TX_ADDR 0x10 // send the address (LSB first), ShockBurstTM mode, RX_ADDR_P0 equal to this address

#define RX_PW_P0 0x11 // Channel 0 receive data valid data width (1 to 32 bytes), set to 0 illicit
#define RX_PW_P1 0x12 // Receive data channel 1 valid data width (1 to 32 bytes), set to 0 illicit
#define RX_PW_P2 0x13 // receive data valid data width of the channel 2 (1 ~ 32 bytes), set to 0 illicit
#define RX_PW_P3 0x14 // receive data channel 3 valid data width (1 to 32 bytes), set to 0 illicit
#define RX_PW_P4 0x15 // receive data valid data width of the channel 4 (1 ~ 32 bytes), set to 0 illicit
#define RX_PW_P5 0x16 // receive data channel 5 valid data width (1 to 32 bytes), set to 0 illicit

#define FIFO_STATUS 0x17 // FIFO Status Register; bit0, RX FIFO register empty flag; bit1, RX FIFO full flag; bit2, 3, to retain
                              // Bit4, TX FIFO empty flag; bit5, TX FIFO full flag; bit6, 1, loop sending data packets on a .0, no circulation;
//////////////////////////////////////////////////////////////////////////////////////////////////////////


// 24L01 send and receive data width defined
#define TX_ADR_WIDTH 5 // 5 the width of the address byte
#define RX_ADR_WIDTH 5 // 5 the width of the address byte

#define TX_PLOAD_WIDTH 32 // 20 bytes of user data width
#define RX_PLOAD_WIDTH 32 // 20 bytes of user data width

void bsp_nrf24l01_init (void); // initialization
void nRF24L01_RX_Mode (void); // configured to receive mode
void nRF24L01_TX_Mode (void); // configured to transmit mode
u8 nRF24L01_Write_Buf (u8 reg, u8 * pBuf, u8 u8s); // write data area
u8 nRF24L01_Read_Buf (u8 reg, u8 * pBuf, u8 u8s); // read data area
u8 nRF24L01_Read_Reg (u8 reg); // read register
u8 nRF24L01_Write_Reg (u8 reg, u8 value); // write register
u8 nRF24L01_Check (void); // check whether there 24L01
u8 nRF24L01_TxPacket (u8 * txbuf); // send a packet of data
u8 nRF24L01_RxPacket (u8 * rxbuf); // Receive a packet of data


#endif


