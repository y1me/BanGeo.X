#ifndef HARDWARE_PROFILE_H


    #define HARDWARE_PROFILE_H
/** DEBUG ***********************************************************/
    #define I2CFreeBusFail      0x01        
    #define I2CStartFail        0x02
    #define I2COpcodeFail       0x03
    #define I2CRegaddFail       0x04
    #define I2CSdataFail        0x05
    #define I2CStopFail         0x06
    #define I2CRestartFail      0x07
    #define I2CReicvFail        0x08
    #define I2CAcknFail         0x10




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
    

           
    /** EUSART ************************************************************/   
    #define UART_TX_INT_F   PIR1bits.TXIF
    #define TX_UART_REG     TX1REG
    #define TX_UART_INT_E   PIE1bits.TXIE
    #define UART_RX_INT_F   PIR1bits.RCIF
    #define RX_UART_REG     RC1REG
    #define UART_RX_OERR    RC1STAbits.OERR
    #define UART_RX_EN      RC1STAbits.CREN

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

 	#define TIM_PWM_INT_F           TMR2IF
	#define TIM_PWM_REG             TMR2
	#define EN_TIM_PWM              TMR2ON

    /** External Int1 ****************************************************/

 	#define INT1_INT_F		INT1IF
	#define INPUT_IR		PORTBbits.RB1
	#define EN_INT1			INT1IE

 #endif  //HARDWARE_PROFILE_H
