#include "./h/user.h"
#include "./h/i2c.h"
#include "./h/hwprofile.h"

extern volatile struct chbits{
						unsigned RxUart:1; 
						unsigned TxUart:1; 
						unsigned Sys_Init:1; 
						unsigned tim100u:1; 
						unsigned Button:1; 
						unsigned Data1:1; 
						unsigned Data2:1; 
						unsigned cont:1;
		
					}flag ;

volatile char AINMux = 0x04, Led = 0, error;  
volatile char draft[10]; 
volatile int AIN[4];
volatile int * volatile pAIN;
static int blink = 50, loopBlink = 0, zeroTilt;
int eeAddr;
volatile int ADSValue, Vbatt = 0, Tbatt = 0, PrevVbatt = 0, PrevTbatt = 0, loop = 0, diff;

extern volatile char RX_BUFF[15];
extern volatile char TX_BUFF[15];
extern volatile unsigned char *pRX_W, *pTX_stop, *pTX_W;


/**
  @Summary
    Writes a data byte to Data EEPROM

  @Description
    This routine writes a data byte to given Data EEPROM location

  @Preconditions
    None

  @Param
    bAdd  - Data EEPROM location to which data to be written
    bData - Data to be written to Data EEPROM location
   Address F000h-F0FFh

  @Returns
    None

  @Example
    <code>
    uint16_t dataeeAddr = 0xF010;
    uint8_t dataeeData = 0x55;

    DATAEE_WriteByte(dataeeAddr, dataeeData);
    </code>
*/

void DATAEE_WriteByte(uint16_t bAdd, uint8_t bData)
{
    INTCONbits.GIE = 0;     // Disable interrupts
    INTCONbits.PEIE = 0;
    NVMCON1bits.NVMREGS = 1;
    NVMCON1bits.WREN = 1;
    
    NVMADRH = ((bAdd >> 8) & 0xFF);
    NVMADRL = (bAdd & 0xFF);
    NVMDATL = bData;      
    
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1;
    // Wait for write to complete
    while (NVMCON1bits.WR);
    {
    }

    NVMCON1bits.WREN = 0;
    INTCONbits.GIE = 1;   // restore interrupt enable
    INTCONbits.PEIE = 1;
}

/**
  @Summary
    Reads a data byte from Data EEPROM

  @Description
    This routine reads a data byte from given Data EEPROM location

  @Preconditions
    None

  @Param
    bAdd  - Data EEPROM location from which data has to be read
   Address F000h-F0FFh

  @Returns
    Data byte read from given Data EEPROM location

  @Example
    <code>
    uint16_t dataeeAddr = 0xF010;
    uint8_t readData;

    readData = DATAEE_ReadByte(dataeeAddr);
    </code>
*/

uint8_t DATAEE_ReadByte(uint16_t bAdd)
{
    NVMADRH = ((bAdd >> 8) & 0xFF);
    NVMADRL = (bAdd & 0xFF);
    NVMCON1bits.NVMREGS = 1;    
    NVMCON1bits.RD = 1;
    NOP();  // NOPs may be required for latency at high frequencies
    NOP();

    return (NVMDATL);
}

void InitI2cChip(void)
{
    char data[2];
    //MCP23008
    data[0] = 0x00;
    error = I2C_Write(ADD_IOEXP, IODIR_IOEXP, &data[0], 1);
    data[0] = 0x10;
    error = I2C_Write(ADD_IOEXP, GPIO_IOEXP, &data[0], 1);
    error = I2C_Read(ADD_IOEXP, IODIR_IOEXP, &data[0], 1);
    
    //ADS1115
    error = I2C_Read(ADD_ADS, CFG_ADS, &draft[0], 2);
    data[0] = 0x43;
    data[1] = 0x83;
    error = I2C_Write(ADD_ADS, CFG_ADS, &data[0], 2);
    error = I2C_Read(ADD_ADS, CFG_ADS, &draft[0], 2);
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
  
/*
    int getValue(const int * ip) {
return *ip;*/
    // return conversion value 
    char data[2];
    int conv = 0x8000;
    error = I2C_Read(ADD_ADS, CONV_ADS, &data[0], 2);
    if (data[0] > 128 ) return conv;
    
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
    data[0] = 0x00;
    while (data[0] < 0x80) {
        error = I2C_Read(ADD_ADS, CFG_ADS, &data[0], 2);
    }
    data[0] |= CONV_MASK; 
    error = I2C_Write(ADD_ADS, CFG_ADS, &data[0], 2); 
}

void StartTX(void)
{
    pTX_W = &TX_BUFF[0];
    UART_TX_EN = 1;
    while(TX_UART_INT_E);
    TX_UART_INT_E = 1;
}

void ProcessIO(void)
{
    
    if (!flag.Sys_Init)
    {
        InitI2cChip();
        //InitBLE();
        AINMux = 0x04;
        Startconv();
        SetVbattMux();
        flag.Sys_Init = 1;
        //Set zero tilt    
        zeroTilt = DATAEE_ReadByte(ZERO_TILT_ADDR + 1);
        zeroTilt <<= 8;
        zeroTilt |= DATAEE_ReadByte(ZERO_TILT_ADDR);
        pAIN = &AIN[0];
    }
    
    if (flag.RxUart)
    {
        if (RX_BUFF[0] == 'C')
        {
            switch(RX_BUFF[1])  {
                case    'A': // "CA\n" Device identification
                    TX_BUFF[0] = 'R';
                    TX_BUFF[1] = 'A';
                    TX_BUFF[2] = '3';
                    TX_BUFF[3] = 'H';// H : head, P : Plate 
                    TX_BUFF[4] = 'F';// F : front, B : back
                    TX_BUFF[5] = 'R';// R : right, L : left 
                    TX_BUFF[6] = '\n';
                    pTX_stop = &TX_BUFF[6];
                    
                    StartTX();
                  break;
                case    'B': // "CB\n" disable charge mode
                    CHG_ON = 0;
                  break;
                case    'C': // "CC\n" enable charge mode
                    CHG_ON = 1;
                  break;
                case    'D':  // "CD*value*\n" set led, valid value: 0bxxxx00xx 
                    Led |= RX_BUFF[2];
                    SetLed(Led);
                  break;
                case    'E':  // "CE*value*\n" set ADS1115 Mux, valid value: AIN0(0x40), AIN1(0x50), AIN2(0x60), AIN3(0x70) 
                    SetMux(RX_BUFF[2]);
                  break;
                case    'F': // "CF\n" get battery temperature and voltage 
                    TX_BUFF[0] = 'R';
                    TX_BUFF[1] = 'F';
                    TX_BUFF[2] = '4';
                    TX_BUFF[3] = Tbatt;
                    TX_BUFF[4] = (Tbatt >> 8);
                    TX_BUFF[5] = Vbatt;
                    TX_BUFF[6] = (Vbatt >> 8);
                    TX_BUFF[7] = '\n';
                    pTX_stop = &TX_BUFF[7];
                    
                    StartTX();
                  break;
                case    'G': // "CG\n" get sensors value
                    
                    TX_BUFF[0] = 'R';
                    TX_BUFF[1] = 'G';
                    TX_BUFF[2] = '8';
                    TX_BUFF[3] = AIN[0];
                    TX_BUFF[4] = (AIN[0] >> 8);
                    TX_BUFF[5] = AIN[1];
                    TX_BUFF[6] = (AIN[1] >> 8);
                    TX_BUFF[7] = AIN[2];
                    TX_BUFF[8] = (AIN[2] >> 8);
                    TX_BUFF[9] = AIN[3];
                    TX_BUFF[10] = (AIN[3] >> 8);
                    TX_BUFF[11] = '\n';
                    pTX_stop = &TX_BUFF[11];
                    
                    StartTX(); 
                    
                  break;
                case    'H': // "CH*value1*\n" get ADS value and write in eeprom, user must provide a valid tail address, Beware Overlap!!!!!   
                    
                    eeAddr = RX_BUFF[2];
                    eeAddr %= 4;
                    ADSValue = AIN[ eeAddr ];         
                    if (RX_BUFF[2] == ZERO_TILT_ASCII) {
                        zeroTilt = ADSValue;
                    }
                    eeAddr = 0xF000;
                    //RX_BUFF[2] &= EEPROM_ADDR_n_MASK; // mask to receive ascii character
                    RX_BUFF[2] += RX_BUFF[2];
                    eeAddr |= RX_BUFF[2];
                    DATAEE_WriteByte(eeAddr , ADSValue);
                    DATAEE_WriteByte(eeAddr + 1, ADSValue >> 8);
                 
                  break;
                case    'I': // "CI*value*\n clear led, valid value: 0bxxxx00xx 
                    Led &= RX_BUFF[2];
                    SetLed(Led);
                  break;
                case    'J': // "CI*value*\n 
                    /***********/
                  break;
                case    'K': // "CK*value*\n" return ZERO, upper bound, lower bound calib user must provide a valid tail address 
                    eeAddr = 0xF000; 
                    RX_BUFF[2] += RX_BUFF[2];
                    //RX_BUFF[2] &= EEPROM_ADDR_n_MASK;
                    eeAddr |= RX_BUFF[2];

                    TX_BUFF[4] = DATAEE_ReadByte(eeAddr);
                    TX_BUFF[3] = DATAEE_ReadByte(eeAddr + 1);

                    TX_BUFF[0] = 'R';
                    TX_BUFF[1] = 'K';
                    TX_BUFF[2] = '2';
                    TX_BUFF[5] = '\n';
                    pTX_stop = &TX_BUFF[5];
                    
                    StartTX();
                        
                  break;

            }
        }
        RX_BUFF[0] = 0;
        RX_BUFF[1] = 0;
        RX_BUFF[2] = 0;
        RX_BUFF[3] = 0;
        flag.RxUart = 0;
    } 
    
    if (flag.Button == 1) {
        TX_BUFF[0] = 'B';
        TX_BUFF[1] = 'P';
        TX_BUFF[2] = '\n';
        pTX_stop = &TX_BUFF[2];
        StartTX();

        flag.Button = 0;

    }

    if (blink <= loopBlink)
    {
        if ( (Led & 0x10) == 0x10 && !VOFFCHG) { 
            Led &= 0xEF;
            SetLed(Led);
        }
        else {
            Led |= 0x10;
            SetLed(Led);
        }
        if (VOFFCHG) {
            Led |= 0x10;
            SetLed(Led);
        }
        loopBlink = 0;
    }

    if (flag.tim100u) //every 100µs
    {
        
        loop++;
         if (loop == 100) {
            CLRWDT();
            Startconv();
            Vbatt = GetADC();//Dummy, lost after mux switch
            Vbatt = GetADC(); // ratio mesure 0.42
            SetTempMux();
            diff = zeroTilt;
            diff -= AIN[3];
            if (diff > LEFT_TILT) {
                Led &= 0x7C;
                Led |= 0x80;
                SetLed(Led);
            }
            else if (diff < RIGHT_TILT) { 
                Led &= 0x7C;
                Led |= 0x02;
                SetLed(Led);
            }   
            else{
                Led &= 0x7C;
                Led |= 0x01;
                SetLed(Led);
            }
         }
        if (loop == 200) //every 20ms
        {
            loopBlink++; 
            Tbatt = GetADC();//Dummy, lost after mux switch
            Tbatt = GetADC();// 0.847 = 0xd2
            SetVbattMux();
            
       
            if (Vbatt < V_PRE){
                SetCharge(PRE_CHG);
                blink = BLINK_PRE;
            }
            else if (Vbatt < V_TAIL) {
                SetCharge(FAST_CHG);
                PrevVbatt = Vbatt;
                PrevTbatt = Tbatt;
                blink = BLINK_FAST;
            }
            else {
               if (PrevVbatt < Vbatt) {
                    PrevVbatt = Vbatt;   
                }
               if (PrevTbatt > Tbatt) {
                    PrevTbatt = Tbatt;   
                }
                diff = Vbatt - PrevVbatt;
                if ( (diff < V_DIF_Tail) || (Vbatt > V_OVER) ) {
                    SetCharge(TAIL_CHG);
                    blink = BLINK_TAIL;
                }
                if (PrevTbatt < TEMP_MIN) {
                    PrevTbatt = TEMP_MIN;
                }
                diff = Tbatt - PrevTbatt;
                if ( (diff > T_DIF_Tail) || (Tbatt > TEMP_MAX) ) {
                    SetCharge(TAIL_CHG);
                    blink = BLINK_TAIL;
                }
                
            }
            loop = 0;

                while (Getconv() == 0x8000);
                
                *pAIN= Getconv();
                *pAIN++; 
                AINMux++;
                if (AINMux > 0x7)
                {
                    pAIN = &AIN[0];
              
                    AINMux = 0x04;
                }
                SetMux(AINMux << 4);
                Startconv();
            
        }
        flag.tim100u = 0;
    }
}

