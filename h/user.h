#ifndef USER_H
#define USER_H


/** P U B L I C  P R O T O T Y P E S *****************************************/


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