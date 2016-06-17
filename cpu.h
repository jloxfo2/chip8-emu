#ifndef _CPU_H_
#define _CPU_H_


#define PROGRAM_START   0x200


/*
 *  CHIP-8 registers
 */
typedef struct registers {
	// 16 data registers
	unsigned char V0;
	unsigned char V1;
	unsigned char V2;
	unsigned char V3;
	unsigned char V4;
	unsigned char V5;
	unsigned char V6;
	unsigned char V7;
	unsigned char V8;
	unsigned char V9;
	unsigned char VA;
	unsigned char VB;
	unsigned char VC;
	unsigned char VD;
	unsigned char VE;
	unsigned char VF;   // flag register

	unsigned short I;   // addr reg - involved in mem. operations
	unsigned short pc;   // program counter
} reg;


void fde_cycle(void);


#endif