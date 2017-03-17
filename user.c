#include "./h/user.h"
#include "./h/i2c.h"
#include "./h/hwprofile.h"

extern volatile struct chbits{
						unsigned RxUart:1; 
						unsigned TxUart:1; 
						unsigned I2c_Init:1; 
						unsigned tim1:1; 
						unsigned Dtime:1; 
						unsigned Data1:1; 
						unsigned Data2:1; 
						unsigned bit7:1;
		
					}flag ;

volatile char error = 0;                   

void InitI2cChip(void)
{
    char data;
    //MCP23008
    data = 0x00;
    error = I2C_Write(ADD_IOEXP, IODIR_IOEXP, &data, 1);
    data = 0x01;
    error = I2C_Write(ADD_IOEXP, GPIO_IOEXP, &data, 1);
    error = I2C_Read(ADD_IOEXP, IODIR_IOEXP, &data, 1);
    
    //ADS1115
    
}

void SetCharge(char cfg)
{
    // valid input value : FAST_CHG, TAIL_CHG, PRE_CHG 
    char data;
    error = I2C_Read(ADD_IOEXP, IODIR_IOEXP, &data, 1);
    data &= CHG_n_MASK;
    cfg &= CHG_MASK;
    data |= cfg;  
    error = I2C_Write(ADD_IOEXP, IODIR_IOEXP, &data, 1);  
}

void ProcessIO(void)
{
    if (!flag.I2c_Init)
    {
        InitI2cChip();
        flag.I2c_Init = 0;
    }
    if (flag.RxUart)
    {
        SetCharge(PRE_CHG);
        flag.RxUart = 0;
    }
        
}

