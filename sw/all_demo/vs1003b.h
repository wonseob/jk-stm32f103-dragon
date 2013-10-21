
#ifndef __VS1003B_H__
#define __VS1003B_H__


#define MP3_XREST_PORT GPIOA
#define MP3_XREST      GPIO_Pin_8

// DREQ
#define MP3_DREQ_PORT  GPIOD
#define MP3_DREQ       GPIO_Pin_2

// XCS
#define MP3_CCS_PORT   GPIOC
#define MP3_CMD_CS     GPIO_Pin_10

// XDCS
#define MP3_DCS_PORT   GPIOC
#define MP3_DATA_CS    GPIO_Pin_11

#define MP3_CCS_SET(x)  GPIOC->ODR=(GPIOC->ODR&~MP3_CMD_CS)|(x ? MP3_CMD_CS:0)	   // XCS
#define MP3_RST_SET(x)  GPIOA->ODR=(GPIOA->ODR&~MP3_XREST)|(x ? MP3_XREST:0)	   // XRESET
#define MP3_DCS_SET(x)  GPIOC->ODR=(GPIOC->ODR&~MP3_DATA_CS)|(x ? MP3_DATA_CS:0)   // XDCS

#define VS_WRITE_COMMAND 	0x02
#define VS_READ_COMMAND 	0x03

#define CLOCK_REG      0xc000 //0x1c00 //0x1800 //0x1000 //0x4400 //0x0800 //0x1000 //0x2800 //0x0000 // 0x2000 //0x4000 //0x6000 //0xa000 //0xe000  // 0x0000
#define DefaultVolume   0x4444


#define SPI_MODE        	0x00   
#define SPI_STATUS      	0x01   
#define SPI_BASS        	0x02   
#define SPI_CLOCKF      	0x03   
#define SPI_DECODE_TIME 	0x04   
#define SPI_AUDATA      	0x05   
#define SPI_WRAM        	0x06   
#define SPI_WRAMADDR    	0x07   
#define SPI_HDAT0       	0x08   
#define SPI_HDAT1       	0x09 
#define SPI_AIADDR      	0x0a   
#define SPI_VOL         	0x0b   
#define SPI_AICTRL0     	0x0c   
#define SPI_AICTRL1     	0x0d   
#define SPI_AICTRL2     	0x0e   
#define SPI_AICTRL3     	0x0f   
#define SM_DIFF         	0x01   
#define SM_JUMP         	0x02   
#define SM_RESET        	0x04   
#define SM_OUTOFWAV     	0x08   
#define SM_PDOWN        	0x10   
#define SM_TESTS        	0x20   
#define SM_STREAM       	0x40   
#define SM_PLUSV        	0x80   
#define SM_DACT         	0x100   
#define SM_SDIORD       	0x200   
#define SM_SDISHARE     	0x400   
#define SM_SDINEW       	0x800   
#define SM_ADPCM        	0x1000   
#define SM_ADPCM_HP     	0x2000 		 


 

u16  Vs1003_REG_Read(u8 address);
void Vs1003_DATA_Write(unsigned char  * buf);
void Vs1003_CMD_Write(u8 address,u16 data);
u16 Vs1003_Init(void);
void VS1003B_Reset(void);
void Vs1003SoftReset(void);
void set1003(void);
void VsRamTest(void);
void VsSineTest(void);
u16 GetDecodeTime(void);
u16 GetHeadInfo(void);
void ResetDecodeTime(void);
void LoadPatch(void);
void GetSpec(u8 *p);
u16 VS1003B_NeedData(void);
void VS1003B_Fill2048Zero(void);
void vs1003b_volume_set(u16 vol);
#endif



