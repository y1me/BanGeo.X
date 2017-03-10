#ifndef _I2C_H
#define _I2C_H

/**
  Section: Included Files
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <xc.h>


    void I2C_MasterWrite(
            uint8_t *opcode,
            uint8_t *regadd,
            uint8_t *pdata,
            uint8_t length);


    void I2C_MasterRead(
            uint8_t *opcode,
            uint8_t *regadd,
            uint8_t *pdata,
            uint8_t length);

#endif //_I2C_H

/**
 End of File
 */
