#ifndef USER_H
#define USER_H



/** Battery Charging ***********************************************************/
#define FAST_CHG        0x0C
#define TAIL_CHG        0x00
#define PRE_CHG         0x08
#define V_PRE           0x236
#define V_TAIL          0x3B0
#define V_OVER          0x3FF
#define V_DIF_Tail      -7
#define T_DIF_Tail      10

#define AIN0            0x40
#define AIN1            0x50
#define AIN2            0x60
#define AIN3            0x70

#define temperature     0x41
#define voltage         0x45

#define CHG_n_MASK      0xF3
#define CHG_MASK        0x0C
/** ADC & ADS ***********************************************************/
#define MUX_n_MASK      0x8F
#define MUX_MASK        0x70
#define CONV_n_MASK     0x7F
#define CONV_MASK       0x80
/** EEPROM ***********************************************************/
#define EEPROM_ZERO_ADDR      0xF000
#define EEPROM_ADDR_n_MASK    0x1F

         
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