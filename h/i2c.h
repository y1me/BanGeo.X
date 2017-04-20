#ifndef _I2C_H
#define _I2C_H

/**
  Section: Included Files
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <xc.h>


#define I2CBusy              SSP1CON2 & 0x1F
#define I2CnTransmit         SSP1STATbits.R_nW
#define I2CMaxTimeout        50

/** I2C ************************************************************/
#define I2C_ACKEN             SSP1CON2bits.ACKEN
#define I2C_ACKDT             SSP1CON2bits.ACKDT
#define I2C_ACKSTAT           SSP1CON2bits.ACKSTAT
#define I2C_GCEN              SSP1CON2bits.GCEN
#define I2C_Stop              SSP1CON2bits.PEN
#define I2C_Reicv             SSP1CON2bits.RCEN
#define I2C_Restart           SSP1CON2bits.RSEN
#define I2C_Start             SSP1CON2bits.SEN
#define I2C_BUFF              SSP1BUF
#define I2C_INT_F             PIR1bits.SSP1IF
#define I2C_BF                SSP1STATbits.BF
#define I2C_W_Bit             0xFE
#define I2C_R_Bit             0x01
#define I2C_BF                SSP1STATbits.BF


    char I2C_Write(
            const uint8_t opcode,
            const uint8_t regadd,
            uint8_t *pdata,
            const uint8_t length);


    char I2C_Read(
            const uint8_t opcode,
            const uint8_t regadd,
            uint8_t *pdata,
            const uint8_t length);

#endif //_I2C_H

/**
 End of File
 */
