#include "./h/pic_init.h"
#include "./h/hwprofile.h"
#include <xc.h>




/************************************************************************
*						PIC INIT        								*
************************************************************************/
void Port_Init(void)  //init i/o 
{
    //Set/Unset analog pin
    ANSELAbits.ANSA5 = 0;
    ANSELCbits.ANSC2 = 0;
    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC4 = 0;
    ANSELCbits.ANSC5 = 0;
    TRISCbits.TRISC2 = INPUT_PIN;
    TRISCbits.TRISC3 = OUTPUT_PIN;
    TRISAbits.TRISA5 = OUTPUT_PIN;
    TRISAbits.TRISA4 = INPUT_PIN;
    PORTAbits.RA5 = 0;
}

void pps_init(void) {
  GIE = 0;
  PPSLOCK = 0x55;
  PPSLOCK = 0xaa;
  PPSLOCKbits.PPSLOCKED = 0;		// unlock PPS
  /*****EUSART*****/
  RXPPSbits.RXPPS = 0x15;   //RC5->EUSART:RX;
  RC4PPSbits.RC4PPS = 0x14;   //RC4->EUSART:TX;
  /*****I2C*****/
  RA2PPSbits.RA2PPS = 0x18; //SCK
  RA4PPSbits.RA4PPS = 0x19; //SDA
  SSP1CLKPPSbits.SSP1CLKPPS = 0x02; //
  SSP1DATPPSbits.SSP1DATPPS = 0x04;
  /*****NCO*****/
  RC3PPSbits.RC3PPS = 0x1D; //RC3->NCO1:NCO;
  /*****INT EXT*****/
  INTPPSbits.INTPPS = 0x12; //RC2->INT EXT;
  
 
  PPSLOCK = 0x55;
  PPSLOCK = 0xaa;
  PPSLOCKbits.PPSLOCKED = 1;		// lock PPS
}

void I2C_Init(void)  //init I2C Bus
{
//I2C Bus register
    SSP1CON1bits.SSPEN = 0;
    SSP1STAT &= 0x3F;
    SSP1ADD = 0x13; // 400KHz
    SSP1CON1bits.SSPM = 8;
    SSP1CON1bits.CKP = 1;
    SSP1CON3bits.SCIE = 0;
    SSP1CON3bits.PCIE = 0;
    //I2C Bus pin
    ANSELAbits.ANSA2 = 0;
    ANSELAbits.ANSA4 = 0;
    TRISAbits.TRISA2 = INPUT_PIN;
    TRISAbits.TRISA4 = INPUT_PIN;
    ODCONAbits.ODCA2 = 1;
    ODCONAbits.ODCA4 = 1;
    
    SSP1CON1bits.SSPEN = 1;
}

void NCO_Init(void)  //init PWM
{

    // Set the NCO to the options selected in the GUI
    // N1EN disabled; N1POL active_hi; N1PFM PFM_mode; 
    NCO1CON = 0x01;
    // N1CKS HFINTOSC; N1PWS 16_clk; 
    NCO1CLK = 0x81;

    NCO1ACCU = 0x00;

    NCO1ACCH = 0x00;

    NCO1ACCL = 0x00;

    NCO1INCU = 0x00;
    NCO1INCH = 0x71;
    NCO1INCL = 0xAA;
    // Enable the NCO module
    NCO1CONbits.N1EN = 1;
    //ton = 500ns, toff = 626ns 
}

void ADC_Init(void)  //init ADC
{
    // Init Fixed Voltage Ref
    FVRCONbits.ADFVR = 3;
    FVRCONbits.FVREN = 1;
    
    //ADC Conf
    // ADFM left; ADNREF VSS; ADPREF FVR; ADCS FOSC/64; 
    ADCON1 = 0xE3;
    
    ADACT = 0x00;
    // ADGO stop; ADON enabled; CHS ANC0; 
    ADCON0 = 0x41;
}

void USART_Init(void)  //init USART
{
    
    RC1STAbits.SPEN = 1;
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 1;
    
    TX1STAbits.TX9 = 0;
    TX1STAbits.SYNC = 0;
    TX1STAbits.SENDB = 0;
    TX1STAbits.BRGH = 1; 
    
    RC1STAbits.RX9 = 0;
    BAUD1CONbits.BRG16 = 1;
    SP1BRGH = 0x00;// a voir
    SP1BRGL = 0x44;// 9600 : 0x340, 115200 : 0x44 
    
    RC1STAbits.CREN = 1;
    TX1STAbits.TXEN = 0;
    
}
void WDT_Init(void)  //init Interrupt
{
    WDTCONbits.WDTPS = 8; // max reset interval 256ms
    WDTCONbits.SWDTEN = 1;
}
void INT_Init(void)  //init Interrupt
{
    //I2C interrupt
    PIE1bits.SSP1IE = 0;
    
    // INT Reg control
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
    //UART interrupt
    PIE1bits.TXIE = 0;
    
    //INT1 interrupt

	INTCONbits.INTEDG = 0;
	PIE0bits.INTE = 1;
	PIR0bits.INTF = 0;
    
    //PIE1bits.RCIE = 1;
 
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
    
  
*/
}
/*
void Timer0_Init(void)  //init timer0
{
	
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
     
}

void Timer1_Init(void)  //init timer1
{
	
	T1CONbits.RD16 = 1;
	T1CONbits.TMR1CS = 0;
	T1CONbits.T1CKPS1 = 0;
	T1CONbits.T1CKPS0 = 0;

	//TMR0L = 75;
	PIE1bits.TMR1IE = 1;
	IPR1bits.TMR1IP = 1;

	T1CONbits.TMR1ON = 0;
     
}
*/
void Timer2_Init(void)  //init timer2
{
    T2CONbits.T2OUTPS = 0; // postscaler = 1/1
    T2CONbits.T2CKPS = 1; // prescaler = 1/4
    PR2 = 0xC8;
    TMR2 = 0x00;

    // Clearing IF flag before enabling the interrupt.
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;
    T2CONbits.TMR2ON = 1;

}
