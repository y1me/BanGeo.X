#ifndef USER_H
#define USER_H



/** Battery Charging ***********************************************************/
#define FAST_CHG        0x0C
#define TAIL_CHG        0x00
#define PRE_CHG         0x08
#define V_PRE           492 // Vbat*0.42 (ratio diviseur de tension) = Vadc 
#define V_TAIL          850 //8.09V
#define V_OVER          1020 // Vmax 
#define V_DIF_Tail      -15 // Delta V batterie 0.14V
#define TEMP_MIN        250 //  T min 30°C
#define TEMP_MAX        392 //  T min 30°C
#define T_DIF_Tail      70 // Delta T 15°C

#define AIN0            0x40
#define AIN1            0x50
#define AIN2            0x60
#define AIN3            0x70

#define temperature     0x41
#define voltage         0x45

#define CHG_n_MASK      0xF3
#define CHG_MASK        0x0C

#define BLINK_PRE       5
#define BLINK_FAST      10
#define BLINK_TAIL       20
/** ADC & ADS ***********************************************************/
#define MUX_n_MASK      0x8F
#define MUX_MASK        0x70
#define CONV_n_MASK     0x7F
#define CONV_MASK       0x80
#define CONV_ATTEMPT    20
/** EEPROM ***********************************************************/
#define EEPROM_ZERO_ADDR      0xF000
#define EEPROM_ADDR_n_MASK    0x1F
/***Tilt***************************************************************/
#define LEFT_TILT           30
#define RIGHT_TILT          -30
#define ZERO_TILT_ASCII     '3'
#define ZERO_TILT_ADDR      0xF066
         
/** P U B L I C  P R O T O T Y P E S *****************************************/
/***************** USER FUNCTIONS PROTOYPES *********************/
void ProcessIO(void);
/***********************************************************************/

/** S T R U C T U R E S ******************************************************/
typedef struct DataMSF DataMSF;
struct DataMSF
{
	unsigned char track;
	unsigned char min;
	unsigned char sec;
	unsigned char frame;
};

#endif //USER_H