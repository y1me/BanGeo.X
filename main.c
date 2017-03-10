
// PIC16F18323 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FEXTOSC = OFF    // FEXTOSC External Oscillator mode Selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT32 // Power-up default value for COSC bits (HFINTOSC with 2x PLL (32MHz))
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; I/O or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR/VPP pin function is MCLR; Weak pull-up enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable bits (WDT disabled; SWDTEN is ignored)
#pragma config LPBOREN = OFF    // Low-power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bits (Brown-out Reset enabled, SBOREN bit ignored)
#pragma config BORV = LOW       // Brown-out Reset Voltage selection bit (Brown-out voltage (Vbor) set to 2.45V)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (The PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a Reset)
#pragma config DEBUG = ON       // Debugger enable bit (Background debugger enabled)

// CONFIG3
#pragma config WRT = OFF        // User NVM self-write protection bits (Write protection off)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored.)

// CONFIG4
#pragma config CP = OFF         // User NVM Program Memory Code Protection bit (User NVM code protection disabled)
#pragma config CPD = OFF        // Data NVM Memory Code Protection bit (Data NVM code protection disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <pic16f18323.h>

#include <stdio.h>
#include <string.h>
#include "./h/pic_init.h"
#include "./h/hwprofile.h"



volatile struct chbits{
						unsigned spi:1; 
						unsigned tim0:1; 
						unsigned int1:1; 
						unsigned tim1:1; 
						unsigned Dtime:1; 
						unsigned Data1:1; 
						unsigned Data2:1; 
						unsigned bit7:1;
		
					}flag ;
                    
volatile struct chbitsalt{
						unsigned bit0:1;
						unsigned bit1:1;
						unsigned info:1;
						unsigned strinfo:1;
						unsigned bit4:1;
						unsigned bit5:1;
						unsigned free3:1;
						unsigned bit7:1;

					}flagalt ;

char i2c_state = S_MASTER_IDLE;
                    
void interrupt ISR(void)
{
    if (I2C_INT_F)
    {
        I2C_INT_F = 0;
        switch (i2c_state) {
            case S_MASTER_IDLE:
                break;
            case S_MASTER_START:
                break;
            case S_MASTER_RESTART:
                break;
            case S_MASTER_SEND_DATA:
                break;
            case S_MASTER_RCV_DATA:
                break;
            case S_MASTER_STOP:
                break;
        }
    }
}

void I2C_Master_Wait()
{
  while (SSPSTATbits.R_nW || (SSP1CON2 & 0x1F)); //Transmit is in progress
}                   

void I2C_Master_Start()
{
  I2C_Master_Wait();    
  SSP1CON2bits.SEN = 1;             //Initiate start condition
}

void I2C_Master_RepeatedStart()
{
  I2C_Master_Wait();
  SSP1CON2bits.RSEN = 1;           //Initiate repeated start condition
}

void I2C_Master_Write(unsigned d)
{
  I2C_Master_Wait();
  SSP1BUF = d;         //Write data to SSPBUF
}

void I2C_Master_Stop()
{
  I2C_Master_Wait();
  SSP1CON2bits.PEN = 1;           //Initiate stop condition
}

volatile int i,z;

void main(void)
{
    // initialize the device
    i=0;
    VOFFCHG = 0; //Shutdown charger
    pps_init();
    Port_Init();
    I2C_Init();
    //if (PIR1bits.BCL1IF) PIR1bits.BCL1IF = 0;    
    flag.spi = 0; 
	flag.tim0 = 0; 
	flag.int1 = 0; 
	flag.tim1 = 0; 
	flag.Dtime = 0; 
	flag.Data1 = 0; 
	flag.Data2 = 0; 
	flag.bit7 = 0;

	flagalt.bit0 = 0;
	flagalt.bit1 = 0;
	flagalt.info = 0;
	flagalt.strinfo = 0;
	flagalt.bit4 = 0;
	flagalt.bit5 = 0;
	flagalt.free3 = 0;
	flagalt.bit7 = 0;
 
    I2C_Master_Wait();
    while (1) {
        
    I2C_Master_Start();
    I2C_Master_Write(0x91); // Add your application code
    I2C_Master_Stop();
    I2C_Master_Start();
    I2C_Master_Write(0x12); // Add your application code
    I2C_Master_Stop();
    I2C_Master_Start();
    I2C_Master_Write(0x93); // Add your application code
    I2C_Master_Stop();

        
    }

}