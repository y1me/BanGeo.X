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
volatile uint8_t test[10];
extern volatile char RX_BUFF[32];
extern volatile char TX_BUFF[32];
extern volatile unsigned char error, *pRX_W, *pTX_stop, *pTX_W;

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

void SetLed(char cfg)
{
    // valid input value : any 
    cfg &= CHG_n_MASK; 
    error = I2C_Write(ADD_IOEXP, GPIO_IOEXP, &cfg, 1);  
}

void SetMux(char cfg)
{
    // valid input value : 0bx100xxxx, 0bx101xxxx, 0bx110xxxx, 0bx111xxxx
    char data[2];
    error = I2C_Read(ADD_ADS, CFG_ADS, &data[0], 2);
    data[0] &= MUX_n_MASK;
    cfg &= MUX_MASK;
    data[0] |= cfg;  
    error = I2C_Write(ADD_ADS, CFG_ADS, &data[0], 2);  
}

int Getconv(void)
{
    // return conversion value 
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

void SetTempMux(void)
{
    ADC_MUX = temperature;
}

void SetVbattMux(void)
{
    ADC_MUX = voltage;   
}

int GetADC(void)
{
    int conv;
    ADC_CONV = 1;
    while (ADC_CONV);
    conv = ADC_H;
    conv = conv << 8;
    conv |= ADC_L;
    return conv;
}



void Startconv(void)
{
    // valid input value : AIN0,AIN1,AIN2,AIN3 
    char data[2];
    data[0] = 0x80;
    while (data[0] > 0x80) {
        error = I2C_Read(ADD_ADS, CFG_ADS, &data[0], 2);
    }
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
        if (RX_BUFF[0] == 'C')
        {
            switch(RX_BUFF[1])  {
                case    'A': // "CA\n" set precharge mode
                    SetCharge(PRE_CHG);
                  break;
                case    'B': // "CB\n" set tail charge mode
                    SetCharge(TAIL_CHG);
                  break;
                case    'C': // "CC\n" set fast charge mode
                    SetCharge(FAST_CHG);
                  break;
                case    'D':  // "CD*value*\n" set led, valid value: 0bxxxx00xx 
                    SetLed(RX_BUFF[2]);
                  break;
                case    'E':  // "CE*value*\n" set ADS1115 Mux, valid value: AIN0(0x40), AIN1(0x50), AIN2(0x60), AIN3(0x70) 
                    SetMux(RX_BUFF[2]);
                  break;
                case    'F': // "CF\n" set pic16 temperature mux 
                    SetTempMux();
                  break;
                case    'G': // "CG\n" set pic16 battery voltage mux 
                    SetVbattMux();
                  break;
                
                    
                  
                  
                    
            }
        }
        RX_BUFF[0] = 0;
        RX_BUFF[1] = 0;
        RX_BUFF[2] = 0;
        flag.RxUart = 0;
    }
        SetMux(0x70);
        Getconv();
        Startconv();
        SetTempMux();
        SetVbattMux();
        GetADC();     
        SetLed(0xAA);
    
error = I2C_Read(0x49, 0x01, &test[0], 2);
    
}

