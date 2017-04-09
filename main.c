
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
#include "./h/user.h"

volatile struct chbits{
						unsigned RxUart:1; 
						unsigned TxUart:1; 
						unsigned Sys_Init:1; 
						unsigned tim100u:1; 
						unsigned Button:1; 
						unsigned Data1:1; 
						unsigned Data2:1; 
						unsigned cont:1;
		
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

volatile char test[10];                    
extern int I2CTimeout;

volatile char RX_BUFF[32];
volatile char TX_BUFF[32];
volatile unsigned char error, *pRX_W, *pTX_stop, *pTX_W;

                    
void interrupt ISR(void)
{
    if(TIM_PWM_INT_F)
    {
        TIM_PWM_REG = 0x00;
        TIM_PWM_INT_F = 0;
        I2CTimeout++;
        flag.tim100u = 1;
    }

    if(UART_TX_INT_F && TX_UART_INT_E)
    {
        
        TX_UART_REG = *pTX_W;
        *pTX_W++;
        if ( pTX_W > pTX_stop ) {
            TX_UART_INT_E = 0;
            //UART_TX_EN = 0;
            pTX_W = &TX_BUFF[0];
        }
        UART_TX_INT_F = 0;
    }

    if(UART_RX_INT_F)
    {
        *pRX_W = RX_UART_REG;
        if (RX_BUFF[0] == 'O' || RX_BUFF[0] == 'C') 
        {
            *pRX_W++;
            if ( *(pRX_W - 1) == '\n' || (&RX_BUFF[31] == pRX_W) ) { 
                pRX_W = &RX_BUFF[0];
                flag.RxUart = 1;
            }
        }
        UART_RX_INT_F = 0;

    }
    if(INT_EXT_F) {
        flag.Button = 1;
        INT_EXT_F = 0;
    } 
    if(PIR1bits.TMR1GIF) {
        PIR1bits.TMR1GIF = 0;
    }
    if(PIR2bits.NVMIF) {
        PIR2bits.NVMIF = 0;
    }

}


    

volatile int i,z;

void main(void)
{
    VOFFCHG = 0; //Shutdown charger
    // initialize the device
    pps_init();
    Port_Init();
    INT_Init();
    I2C_Init();
    Timer2_Init();
    NCO_Init();
    ADC_Init();
    USART_Init();
    // initialize variables
    pRX_W = &RX_BUFF[0];
    pTX_stop = &TX_BUFF[0];
    pTX_W = &TX_BUFF[0]; 
    UART_TX_INT_F = 0;
    i=0;
    
    flag.RxUart = 0; 
	flag.TxUart = 0; 
	flag.Sys_Init = 0; 
	flag.tim100u = 0; 
	flag.Button = 0; 
	flag.Data1 = 0; 
	flag.Data2 = 0; 
	flag.cont = 0;

	flagalt.bit0 = 0;
	flagalt.bit1 = 0;
	flagalt.info = 0;
	flagalt.strinfo = 0;
	flagalt.bit4 = 0;
	flagalt.bit5 = 0;
	flagalt.free3 = 0;
	flagalt.bit7 = 0;
    CHG_ON = 1;
    while (1) {        
        ProcessIO();
    }

}
