#include "./h/pic_init.h"
#include "./h/hwprofile.h"
#include <xc.h>




/************************************************************************
*						PIC INIT        								*
************************************************************************/
void Port_Init(void)  //init i/o PIC for enable DAC
{
    //Set/Unset analog pin

    ANSELAbits.ANSA5 = 0;
    
    TRISAbits.TRISA5 = OUTPUT_PIN;
    TRISAbits.TRISA4 = INPUT_PIN;

}

void pps_init(void) {
  GIE = 0;
  PPSLOCK = 0x55;
  PPSLOCK = 0xaa;
  PPSLOCKbits.PPSLOCKED = 0;		// unlock PPS

  RA2PPSbits.RA2PPS = 0x18; //SCK
  RA4PPSbits.RA4PPS = 0x19; //SDA
  SSP1CLKPPSbits.SSP1CLKPPS = 0x02; //
  SSP1DATPPSbits.SSP1DATPPS = 0x04;
  
  PPSLOCK = 0x55;
  PPSLOCK = 0xaa;
  PPSLOCKbits.PPSLOCKED = 1;		// lock PPS
}

void I2C_Init(void)  //init SPI Bus
{
//I2C Bus register
    SSP1CON1bits.SSPEN = 0;
    SSP1STAT &= 0x3F;
    SSP1ADD = 0x4F; // 100KHz
    SSP1CON1bits.SSPM = 8;
    SSP1CON1bits.CKP = 1;
    SSP1CON3bits.SCIE = 1;
    SSP1CON3bits.PCIE = 1;
    //I2C Bus pin
    ANSELAbits.ANSA2 = 0;
    ANSELAbits.ANSA4 = 0;
    TRISAbits.TRISA2 = INPUT_PIN;
    TRISAbits.TRISA4 = INPUT_PIN;
    ODCONAbits.ODCA2 = 1;
    ODCONAbits.ODCA4 = 1;
    
    SSP1CON1bits.SSPEN = 1;
}

void USART_Init(void)  //init USART
{
    /*
    RCSTA2bits.SPEN = 1;
    TRISGbits.RG2 = 1;
    TRISGbits.RG1 = 0;
    
    TXSTA2bits.TX9 = 0;
    TXSTA2bits.SYNC = 0;
    TXSTA2bits.SENDB = 0;
    TXSTA2bits.BRGH = 1;
    
    RCSTA2bits.RX9 = 0;
    BAUDCON2bits.BRG16 = 1;
    SPBRGH2 = 0;
    SPBRG2 = 86;
    
    RCSTA2bits.CREN = 1;
    TXSTA2bits.TXEN = 1;
    */
}

void INT_Init(void)  //init Interrupt
{
    //I2C interrupt
    PIE1bits.SSP1IE = 0;
    
    // INT Reg control
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    /*
	RCONbits.IINTCONPEN = 1;
	
	INTCONbits.GIE_GIEH = 1;
	INTCONbits.PEIE_GIEL = 1;

	//INT1 interrupt

	INTCON3bits.INT1IE = 1;
	INTCON3bits.INT1IP = 1;
	INTCON2bits.INTEDG1 = 0;
	
	//SPI interrupt
	
	IPR1bits.SSP1IP = 1;
	PIR1bits.SSP1IF = 0;
	PIE1bits.SSP1IE = 1;
    
    //UART interrupt
    
    PIE3bits.TX2IE = 0;
    IPR3bits.TX2IP = 0;
    
    PIE3bits.RC2IE = 1;
    IPR3bits.RC2IP = 1;   
*/
}

void Timer0_Init(void)  //init timer0
{
	/*
	T0CONbits.T08BIT = 0;
	T0CONbits.T0CS = 0;
	T0CONbits.PSA = 0;
	T0CONbits.T0PS0 = 1;
	T0CONbits.T0PS1 = 1;
	T0CONbits.T0PS2 = 1;
	//TMR0L = 75;
	INTCONbits.TMR0IE = 1;
	INTCON2bits.TMR0IP = 0;

	T0CONbits.TMR0ON = 1;
     */ 
}

void Timer1_Init(void)  //init timer1
{
	/*
	T1CONbits.RD16 = 1;
	T1CONbits.TMR1CS = 0;
	T1CONbits.T1CKPS1 = 0;
	T1CONbits.T1CKPS0 = 0;

	//TMR0L = 75;
	PIE1bits.TMR1IE = 1;
	IPR1bits.TMR1IP = 1;

	T1CONbits.TMR1ON = 0;
     */ 
}

void Timer2_Init(void)  //init timer2
{
/*
	T2CONbits.T2OUTPS0 = 1;
        T2CONbits.T2OUTPS1 = 0;
        T2CONbits.T2OUTPS2 = 0;
        T2CONbits.T2OUTPS3 = 0;
	T2CONbits.T2CKPS = 1;

        PR2 = 125;
        TMR2 = 0;

	PIE1bits.TMR2IE = 1;
	IPR1bits.TMR2IP = 1;

	T2CONbits.TMR2ON = 1;
*/
}