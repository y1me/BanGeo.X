#ifndef USER_H
#define USER_H



/** Battery Charging ***********************************************************/
#define FAST_CHG        0x0C
#define TAIL_CHG        0x00
#define PRE_CHG         0x04
#define V_PRE           3500
#define V_TAIL          25000

#define AIN0            0x40
#define AIN1            0x50
#define AIN2            0x60
#define AIN3            0x70

#define temperature     0x41
#define voltage         0x45

#define CHG_n_MASK      0xF3
#define CHG_MASK        0x0C
#define MUX_n_MASK      0x8F
#define MUX_MASK        0x70
#define CONV_n_MASK     0x7F
#define CONV_MASK       0x80
         
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