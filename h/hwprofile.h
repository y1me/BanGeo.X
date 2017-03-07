#ifndef HARDWARE_PROFILE_H


    #define HARDWARE_PROFILE_H

    #define CLOCK_FREQ          32000000
    #define Time_info           20000
    #define Scan_time           70 //x * 0.01s
    #define Nb_Retry            50000
    #define SPI_DATA_SIZE       20
    #define DISPLAY_DATA_SIZE   16


/** DEBUG ***********************************************************/
	#define	TRIS_PORTTEST		TRISC7
	#define PORTTEST            PORTCbits.RC0
    #define PORTTEST2           PORTCbits.RC1

/** TRIS ***********************************************************/
	#define INPUT_PIN           1
	#define OUTPUT_PIN          0
	#define INPUT_PORT          0xFF
	#define OUTPUT_PORT         0x00

/** BATTERY CHARGER ***********************************************************/
	#define VOFFCHG             PORTAbits.RA5
    #define VTEMP               PORTAbits.RA4
    
    /** I2C ************************************************************/
	#define I2C_ACKEN		SSP1CON2bits.ACKEN
	#define I2C_ACKDT		SSP1CON2bits.ACKDT
    #define I2C_ACKSTAT		SSP1CON2bits.ACKSTAT
    #define I2C_GCEN		SSP1CON2bits.GCEN
    #define I2C_PEN         SSP1CON2bits.PEN
    #define I2C_RCEN		SSP1CON2bits.RCEN
    #define I2C_RSEN		SSP1CON2bits.RSEN
    #define I2C_SEN         SSP1CON2bits.SEN
	#define I2C_BUFF		SSP1BUF
	#define SPI_INT_F		SSP1IF
   
    /** EUSART ************************************************************/   
    #define UART_TX_INT_F   PIR3bits.TX2IF
    #define TX_UART_REG     TXREG2
    #define TX_UART_INT_E   PIE3bits.TX2IE
    #define UART_RX_INT_F   PIR3bits.RC2IF
    #define RX_UART_REG     RCREG2
    #define UART_RX_OERR    RCSTA2bits.OERR
    #define UART_RX_EN      RCSTA2bits.CREN

    /** Timer telecommande ************************************************************/

 	#define TIMIR_INT_F         TMR1IF
	#define TIMERIR_REGH        TMR1H
	#define TIMERIR_REG         TMR1L
	#define EN_TIMIR            TMR1ON

    /** Timer SubChannel ************************************************************/

 	#define TIMSUB_INT_F            TMR0IF
	#define TIMERSUB_REGH           TMR0H
	#define TIMERSUB_REG            TMR0L
	#define EN_TIMSUB               TMR0ON

    /** Timer Display ************************************************************/

 	#define TIMDSPY_INT_F           TMR2IF
	#define TIMERDSPY_REG           TMR2
	#define EN_TIMDSPY              TMR2ON

    /** External Int1 ****************************************************/

 	#define INT1_INT_F		INT1IF
	#define INPUT_IR		PORTBbits.RB1
	#define EN_INT1			INT1IE

 #endif  //HARDWARE_PROFILE_H
