

#include  <stm32f10x.h>
#include <stm32f10x_spi.h>

#include "spi.h"
#include "lcd.h"
#include "usart.h"
#include "nrf24l01.h"

// RF 전송 속도
// SPI 9MHz, 32byte Payload, RF 0db Power, 2Mbps 전송, Auto ACK /  일 경우에 약 Send Speed = 519424bps(약520kbps)
// spi speed 9Mhz (24L01 maximum SPI clock is 10Mhz)

const u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; // send address
const u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; // send address

// Initialize the IO port 24L01
void bsp_nrf24l01_init (void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;

	/* For use nrf24l01 */
	/* Configure SPI2 pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = NRF24L01_SCK_PIN | NRF24L01_MISO_PIN | NRF24L01_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(NRF24L01_SPI2_PORT, &GPIO_InitStructure);

	/* Configure NRF_CS(CSN) pin */
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(NRF24L01_CS_PORT, &GPIO_InitStructure);
	
	/* Configure NRF_CE pin */
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CE_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(NRF24L01_CE_PORT, &GPIO_InitStructure);
		

	/* Configure NRF_IRQ interrupt */
	GPIO_InitStructure.GPIO_Pin =  NRF24L01_IRQ_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(NRF24L01_IRQ_PORT, &GPIO_InitStructure);		

	/* SPI2 Config */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);	

	/* SPI2 enable */
	SPI_Cmd(SPI2, ENABLE);

	// start transmission
	bsp_readwritebyte_spi2(0xff);	

	NRF24L01_CE=0;
	NRF24L01_CSN=1;
	
}


#if 1

// Check whether there 24L01
// Return value: 0 success; 1, failure
u8 nRF24L01_Check (void)
{

	u8 buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
	u8 i;
	
	bsp_set_spi2_speed (SPI_SPEED_8); // spi speed 9Mhz (24L01 maximum SPI clock is 10Mhz)
	
	nRF24L01_Write_Buf (NRF24L01_WRITE_REG + TX_ADDR, buf, 5); // write 5 bytes of the address.
	nRF24L01_Read_Buf (TX_ADDR, buf, 5); // read the address written
	
	for (i = 0; i<5; i++) 
		if (buf[i] != 0XA5) 
			break;
		
	if (i != 5) 
		return 1; // error detection 24L01
		
	return 0; // detect 24L01


}

// SPI write register
// Reg: register address specified
// Value: The value to write
u8 nRF24L01_Write_Reg (u8 reg, u8 value)
{
	u8 status;
	
	NRF24L01_CSN = 0; // Enable SPI transfer
	
	status = bsp_readwritebyte_spi2 (reg); // send the register number
	bsp_readwritebyte_spi2 (value); // write registers
	
	NRF24L01_CSN = 1; // SPI transfer ban
	
	return (status); // return status values
}

// Read SPI register values
// Reg: register to read
u8 nRF24L01_Read_Reg (u8 reg)
{
	u8 reg_val;

	NRF24L01_CSN = 0; // Enable SPI transfer

	bsp_readwritebyte_spi2 (reg); // send the register number
	reg_val = bsp_readwritebyte_spi2 (0XFF); // read register

	NRF24L01_CSN = 1; // SPI transfer ban

	return (reg_val); // return status values
}

// Read in the specified location data in a specified length
// Reg: register (location)
// * PBuf: data pointer
// Len: data length
// Return value, the value of the read status register

u8 nRF24L01_Read_Buf (u8 reg, u8 * pBuf, u8 len)
{
	u8 status, u8_ctr;

	NRF24L01_CSN = 0; // Enable SPI transfer

	status = bsp_readwritebyte_spi2 (reg); // send the register values (location), and read the status value
	for (u8_ctr=0; u8_ctr<len; u8_ctr++) 
		pBuf[u8_ctr] = bsp_readwritebyte_spi2(0XFF); // read data

	NRF24L01_CSN = 1; // turn off SPI transfer

	return status; // return status value read
}

// Write the specified length at the specified location data
// Reg: register (location)
// * PBuf: data pointer
// Len: data length
// Return value, the value of the read status register
u8 nRF24L01_Write_Buf (u8 reg, u8 * pBuf, u8 len)
{
	u8 status, u8_ctr;
	
	NRF24L01_CSN = 0; // Enable SPI transfer
	
	status = bsp_readwritebyte_spi2 (reg); // send the register values (location), and read the status value
	
	for (u8_ctr=0;u8_ctr<len;u8_ctr++) 
		bsp_readwritebyte_spi2(*pBuf++); // write data
		
	NRF24L01_CSN = 1; // turn off SPI transfer
	
	return status; // return status value read
}

// Start to send a data NRF24L01
// Txbuf: the first address data to be transmitted
// Return value: Send completed state
u8 nRF24L01_TxPacket (u8 * txbuf)
{
	u8 sta;
	
	bsp_set_spi2_speed(SPI_SPEED_8); // spi speed 9Mhz (24L01 maximum SPI clock is 10Mhz)
	
	NRF24L01_CE = 0;
	nRF24L01_Write_Buf (WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); // write data to the TX BUF 32 bytes
	NRF24L01_CE = 1; // start transmission
	
	while (NRF24L01_IRQ != 0); // wait for transmission complete
	sta = nRF24L01_Read_Reg (STATUS); // read status register
	nRF24L01_Write_Reg (NRF24L01_WRITE_REG + STATUS, sta); // Clear interrupt flag TX_DS or MAX_RT
	if (sta & MAX_TX) // maximum number of retransmissions
	{
		nRF24L01_Write_Reg (FLUSH_TX, 0xff); // Clear TX FIFO registers
		return MAX_TX;
	}
	if (sta & TX_OK) // send the completed
	{
		return TX_OK;
	}
	
	return 0xff; // send failed for other reasons
	
}

// Start to send a data NRF24L01
// Txbuf: the first address data to be transmitted
// Return value: 0, receive completed; other, error codes
u8 nRF24L01_RxPacket (u8 * rxbuf)
{
	u8 sta;

	bsp_set_spi2_speed(SPI_SPEED_8); // spi speed 9Mhz (24L01 maximum SPI clock is 10Mhz)
	
	sta = nRF24L01_Read_Reg (STATUS); // read status register
	nRF24L01_Write_Reg (NRF24L01_WRITE_REG + STATUS, sta); // Clear interrupt flag TX_DS or MAX_RT
	
	if (sta & RX_OK) // receive data
	{
		nRF24L01_Read_Buf (RD_RX_PLOAD, rxbuf, RX_PLOAD_WIDTH); // read data
		nRF24L01_Write_Reg (FLUSH_RX, 0xff); // clear the RX FIFO registers
		return 0;
	}
	
	return 1; // did not receive any data
}

// This function initializes NRF24L01 to RX mode
// Set RX address, write RX data width, select the RF channel, baud rate and LNA HCURR
// When CE high, the RX mode is entered, and can receive the data
void nRF24L01_RX_Mode (void)
{
	NRF24L01_CE = 0;
	
	nRF24L01_Write_Buf (NRF24L01_WRITE_REG + RX_ADDR_P0, (u8 *) RX_ADDRESS, RX_ADR_WIDTH); // write node address RX

	nRF24L01_Write_Reg (NRF24L01_WRITE_REG + EN_AA, 0x01); // enable the automatic response channel 0
	nRF24L01_Write_Reg (NRF24L01_WRITE_REG + EN_RXADDR, 0x01); // Enable receiver address channel 0
	nRF24L01_Write_Reg (NRF24L01_WRITE_REG + RF_CH, 40); // set the RF communication frequency
	nRF24L01_Write_Reg (NRF24L01_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); // select a valid data width of the channel 0
	nRF24L01_Write_Reg (NRF24L01_WRITE_REG + RF_SETUP, 0x0f); // set the TX emission parameters, 0db gain, 2Mbps, Low Noise Gain Open
	nRF24L01_Write_Reg (NRF24L01_WRITE_REG + CONFIG, 0x0f); // configure the basic parameters of the model work; PWR_UP, EN_CRC, 16BIT_CRC, receive mode
	
	NRF24L01_CE = 1; // CE is high, into the receive mode
}

// This function initializes NRF24L01 to TX mode
// Set TX address and write data width of the TX, RX set the address auto-answer, fill TX transmit data, select the RF channel, baud rate and LNA HCURR
// PWR_UP, CRC Enable
// When CE high, the RX mode is entered, and can receive the data
// CE is taller than 10us, then start sending.
void nRF24L01_TX_Mode (void)
{
	NRF24L01_CE = 0;
	
	nRF24L01_Write_Buf (NRF24L01_WRITE_REG + TX_ADDR, (u8 *) TX_ADDRESS, TX_ADR_WIDTH); // Write TX node address
	nRF24L01_Write_Buf (NRF24L01_WRITE_REG + RX_ADDR_P0, (u8 *) RX_ADDRESS, RX_ADR_WIDTH); // set the TX node address, mainly in order to enable ACK

	nRF24L01_Write_Reg (NRF24L01_WRITE_REG + EN_AA, 0x01); // enable the automatic response channel 0
	nRF24L01_Write_Reg (NRF24L01_WRITE_REG + EN_RXADDR, 0x01); // Enable receiver address channel 0
	nRF24L01_Write_Reg (NRF24L01_WRITE_REG + SETUP_RETR, 0x1a); // set up automatic retransmission interval: 500us + 86us; largest auto retransmissions: 10
	nRF24L01_Write_Reg (NRF24L01_WRITE_REG + RF_CH, 40); // set the RF channel 40
	nRF24L01_Write_Reg (NRF24L01_WRITE_REG + RF_SETUP, 0x0f); // set the TX emission parameters, 0db gain, 2Mbps, Low Noise Gain Open
	nRF24L01_Write_Reg (NRF24L01_WRITE_REG + CONFIG, 0x0e); // configure the basic parameters of the model work; PWR_UP, EN_CRC, 16BIT_CRC, receive mode, turn on all interrupts
	
	NRF24L01_CE = 1; // CE is high, 10us start sending
} 

#endif


