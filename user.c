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
    char data[2];
    //MCP23008
    data[0] = 0x00;
    error = I2C_Write(ADD_IOEXP, IODIR_IOEXP, &data[0], 1);
    data[0] = 0x01;
    error = I2C_Write(ADD_IOEXP, GPIO_IOEXP, &data[0], 1);
    error = I2C_Read(ADD_IOEXP, IODIR_IOEXP, &data[0], 1);
    
    //ADS1115
    data[0] = 0x43;
    data[1] = 0x83;
    error = I2C_Write(ADD_ADS, CFG_ADS, &data[0], 2);
    error = I2C_Read(ADD_ADS, CFG_ADS, &data[0], 2);
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

void SetMux(char cfg)
{
    // valid input value : FAST_CHG, TAIL_CHG, PRE_CHG 
    char data[2];
    error = I2C_Read(ADD_ADS, CFG_ADS, &data[0], 2);
    data[0] &= MUX_n_MASK;
    cfg &= MUX_MASK;
    data[0] |= cfg;  
    error = I2C_Write(ADD_ADS, CFG_ADS, &data[0], 2);  
}

int Getconv(void)
{
    // valid input value : return conversion value 
    char data[2];
    int conv = 0x8000;
    error = I2C_Read(ADD_ADS, CFG_ADS, &data[0], 2);
    if (data[0] < 128 ) return conv;
    
    error = I2C_Read(ADD_ADS, CONV_ADS, &data[0], 2);
    conv |= data[0];
    conv = conv << 8;
    conv |= data[1];
    return conv; 
}

void Startconv(void)
{
    // valid input value : AIN0,AIN1,AIN2,AIN3 
    char data[2];
    error = I2C_Read(ADD_ADS, CFG_ADS, &data[0], 2);
    data[0] |= CONV_MASK; 
    error = I2C_Write(ADD_ADS, CFG_ADS, &data[0], 2); 
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
        SetMux(0x70);
        Getconv();
        Startconv();
}

