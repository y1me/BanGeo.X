#ifndef HARDWARE_PROFILE_H


    #define HARDWARE_PROFILE_H

    #define CLOCK_FREQ          32000000

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

/** DEBUG ***********************************************************/
	#define	TRIS_PORTTEST		
	#define PORTTEST          
    #define PORTTEST2           

/** TRIS ***********************************************************/
	#define INPUT_PIN           1
	#define OUTPUT_PIN          0
	#define INPUT_PORT          0xFF
	#define OUTPUT_PORT         0x00

/** I2C MCP23008 ***********************************************************/
	#define ADD_IOEXP           0x27
	#define IODIR_IOEXP         0x00
	#define GPIO_IOEXP          0x09
/** I2C ADS1115 ***********************************************************/
	#define ADD_ADS             0x49
	#define CFG_ADS             0x01
	#define CONV_ADS            0x00
/** ADC ***********************************************************/
    #define ADC_MUX             ADCON0
    #define ADC_CONV            ADCON0bits.ADGO
    #define ADC_H               ADRESH
    #define ADC_L               ADRESL
/** BATTERY CHARGER ***********************************************************/
	#define VOFFCHG             PORTAbits.RA5
    #define CHG_ON              TRISAbits.TRISA5
    #define VTEMP               PORTAbits.RA4       
/** EUSART ************************************************************/   
    #define UART_TX_INT_F   PIR1bits.TXIF
    #define TX_UART_REG     TX1REG
    #define TX_REG_FULL     TX1STAbits.TRMT
    #define TX_UART_INT_E   PIE1bits.TXIE
    #define UART_RX_INT_F   PIR1bits.RCIF
    #define RX_UART_REG     RC1REG
    #define UART_RX_OERR    RC1STAbits.OERR
    #define UART_RX_EN      RC1STAbits.CREN
    #define UART_TX_EN      TX1STAbits.TXEN
/** EUSART ************************************************************/
    #define INT_EXT_F       PIR0bits.INTF

    /** Timer Display ************************************************************/

 	#define TIM_PWM_INT_F           PIR1bits.TMR2IF
	#define TIM_PWM_REG             TMR2
	#define EN_TIM_PWM              TMR2ON

 #endif  //HARDWARE_PROFILE_H
