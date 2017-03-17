#ifndef USER_H
#define USER_H



/** Battery Charging ***********************************************************/
#define FAST_CHG        0x0C
#define TAIL_CHG        0x00
#define PRE_CHG         0x04
#define CHG_n_MASK    0xF3
#define CHG_MASK     0x0C
         
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