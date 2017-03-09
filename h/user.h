#ifndef USER_H
#define USER_H

#define MAX_RETRY         100
#define SLAVE_ADDRESS     0x50 
#define PAGE_LIMIT         16  // Change as stated on EEPROM device datasheet
/** P U B L I C  P R O T O T Y P E S *****************************************/

int I2C_ByteWrite(uint8_t *dataAddress, uint8_t dataByte, uint8_t addlen);
uint8_t I2C_ByteRead(uint8_t *dataAddress, uint8_t dataByte,uint8_t addlen);
int I2C_BufferWrite(uint8_t *dataAddress, uint8_t *dataBuffer,  uint8_t addlen, uint8_t buflen);
void I2C_BufferRead(uint8_t *dataAddress, uint8_t *dataBuffer,  uint8_t addlen, uint8_t buflen);

/** S T R U C T U R E S ******************************************************/
typedef struct DataMSF DataMSF;
struct DataMSF
{
	unsigned char track;
	unsigned char min;
	unsigned char sec;
	unsigned char frame;
};

#endif //USER_H