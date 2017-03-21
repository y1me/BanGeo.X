#include "./h/i2c.h"
#include "./h/hwprofile.h"

int I2CTimeout;

/** P R I V A T E  P R O T O T Y P E S ***************************************/
char I2C_Op_Reg( uint8_t , uint8_t );
/*****************************************************************************/

char I2C_Op_Reg( uint8_t op, uint8_t reg)
{
     while (I2CnTransmit || (I2CBusy))
     {
        if ( I2CTimeout > I2CMaxTimeout )  return I2CFreeBusFail;
     }
            
    I2C_Start = 1;             //Initiate start condition
    
    while (I2CnTransmit || (I2CBusy)) 
    {
            if ( I2CTimeout > I2CMaxTimeout )  return I2CStartFail;
    }

    I2C_BUFF = (op << 1 & I2C_W_Bit);
    
    while (I2CnTransmit || (I2CBusy)) 
    {
            if ( I2CTimeout > I2CMaxTimeout )  return I2COpcodeFail;
    }
    
    I2C_BUFF = reg;

    while (I2CnTransmit || (I2CBusy)) 
    {
        if ( I2CTimeout > I2CMaxTimeout )  return I2CRegaddFail;
    }
    return 0;
}

char I2C_Write( uint8_t opcode, uint8_t regadd, uint8_t *pdata, uint8_t length)
{
    I2CTimeout = 0;
    char len =0, error;
    
    error = I2C_Op_Reg( opcode, regadd);
    if (error) return error;
    
    while (length > len)
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
    char len = 0, error,i = 0;
    char debug[10];
    
    error = I2C_Op_Reg( opcode, regadd);
    if (error) return error;
    
    I2C_Restart = 1;
    
    while (I2CnTransmit || (I2CBusy)) {
            if ( I2CTimeout > I2CMaxTimeout )  return I2CRestartFail;
    }
    
    I2C_BUFF = (opcode << 1 | I2C_R_Bit);
    
    while (I2CnTransmit || (I2CBusy))
    {
            if ( I2CTimeout > I2CMaxTimeout )  return I2COpcodeFail;
    }
    while (length > len)
    {
        len++;
        I2C_Reicv = 1;

        while (I2CnTransmit || (I2CBusy)) {  
                if ( I2CTimeout > I2CMaxTimeout )  return I2CReicvFail;
        }
        
        while(!I2C_BF) {  
                if ( I2CTimeout > I2CMaxTimeout )  return I2CReicvFail;
        }
        
        debug[i] = I2C_BUFF;  
        *pdata = debug[i];
        *pdata++;
        I2C_BF = 0;
        i++;
        if (i > 10) i = 0;
        if (length == len) I2C_ACKDT = 1;
        I2C_ACKEN = 1; 
        while (I2CnTransmit || (I2CBusy)) 
                if ( I2CTimeout > I2CMaxTimeout )  return I2CAcknFail;
    }
    I2C_ACKDT = 0;
    I2C_Stop = 1;
    
    while (I2CnTransmit || (I2CBusy)) 
            if ( I2CTimeout > I2CMaxTimeout )  return I2CStopFail;
    return 0;
}



/**
 End of File
 */
