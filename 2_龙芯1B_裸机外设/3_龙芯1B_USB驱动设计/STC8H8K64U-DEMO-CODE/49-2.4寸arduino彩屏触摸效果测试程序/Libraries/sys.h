#ifndef _sys_h_
#define _sys_h_
#include "STC8HX.h"

#define     MAIN_Fosc       24000000L   //定义主时钟

typedef unsigned char   uint8;	//  8 bits 
typedef unsigned int  	uint16;	// 16 bits 
typedef unsigned long   uint32;	// 32 bits 

typedef unsigned char   uint8_t;	//  8 bits 
typedef unsigned int  	uint16_t;	// 16 bits 
typedef unsigned long   uint32_t;	// 32 bits 

typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

typedef signed char     int8;	//  8 bits 
typedef signed int      int16;	// 16 bits 

typedef signed long     int32;	// 32 bits 

typedef volatile int8   vint8;	//  8 bits 
typedef volatile int16  vint16;	// 16 bits 
typedef volatile int32  vint32;	// 32 bits 

typedef volatile uint8  vuint8;	//  8 bits 
typedef volatile uint16 vuint16;	// 16 bits 
typedef volatile uint32 vuint32;	// 32 bits 
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;
typedef unsigned long UINT32;


#endif