#include "./h/i2c.h"
#include "./h/hwprofile.h"

int I2CTimeout;

char I2C_Write( uint8_t opcode, uint8_t regadd, uint8_t *pdata, uint8_t length)
{
    I2CTimeout = 0;
    char len =0;
    
    while (I2CnTransmit || (I2CBusy)) 
        if ( I2CTimeout > I2CMaxTimeout )  return I2CFreeBusFail;
            
    I2C_Start = 1;             //Initiate start condition
    
    while (I2CnTransmit || (I2CBusy)) 
            if ( I2CTimeout > I2CMaxTimeout )  return I2CStartFail;

    I2C_BUFF = (opcode << 1 & I2C_W_Bit);
    
    while (I2CnTransmit || (I2CBusy)) 
            if ( I2CTimeout > I2CMaxTimeout )  return I2COpcodeFail;
    
    I2C_BUFF = regadd;

    while (I2CnTransmit || (I2CBusy)) 
        if ( I2CTimeout > I2CMaxTimeout )  return I2CRegaddFail;
    
    while (length < len)
    {
        len++;
        I2C_BUFF = *pdata;
        *pdata++;
        while (I2CnTransmit || (I2CBusy)) 
            if ( I2CTimeout > I2CMaxTimeout )  return I2CSdataFail;
    }
    
    I2C_Stop = 1;
    
    while (I2CnTransmit || (I2CBusy)) 
            if ( I2CTimeout > I2CMaxTimeout )  return I2CStopFail;
    
    return 0;  
}


char I2C_Read( uint8_t opcode, uint8_t regadd, uint8_t *pdata, uint8_t length)
{
    I2CTimeout = 0;
    char len =0;
    
    while (I2CnTransmit || (I2CBusy)) 
        if ( I2CTimeout > I2CMaxTimeout )  return I2CFreeBusFail;
            
    I2C_Start = 1;             //Initiate start condition
    
    while (I2CnTransmit || (I2CBusy)) 
            if ( I2CTimeout > I2CMaxTimeout )  return I2CStartFail;

    I2C_BUFF = (opcode << 1 & I2C_W_Bit);
    
    while (I2CnTransmit || (I2CBusy)) 
            if ( I2CTimeout > I2CMaxTimeout )  return I2COpcodeFail;
    
    I2C_BUFF = regadd;

    while (I2CnTransmit || (I2CBusy)) 
        if ( I2CTimeout > I2CMaxTimeout )  return I2CRegaddFail;
    
    I2C_Restart = 1;
    
    while (I2CnTransmit || (I2CBusy)) 
            if ( I2CTimeout > I2CMaxTimeout )  return I2CRestartFail;
    
    I2C_BUFF = (opcode << 1 | I2C_R_Bit);
    
    while (I2CnTransmit || (I2CBusy)) 
            if ( I2CTimeout > I2CMaxTimeout )  return I2COpcodeFail;
    
    while (length < len)
    {
        len++;
        I2C_Reicv = 1;

        while (I2CnTransmit || (I2CBusy)) 
                if ( I2CTimeout > I2CMaxTimeout )  return I2CReicvFail;
        *pdata = I2C_BUFF;
        *pdata++;

        I2C_ACKEN = 1; 
        while (I2CnTransmit || (I2CBusy)) 
                if ( I2CTimeout > I2CMaxTimeout )  return I2CAcknFail;
    }
 
    I2C_Stop = 1;
    
    while (I2CnTransmit || (I2CBusy)) 
            if ( I2CTimeout > I2CMaxTimeout )  return I2CStopFail;
    
    return 0;
}



/**
 End of File
 */
