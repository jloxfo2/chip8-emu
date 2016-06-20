#ifndef _CPU_H_
#define _CPU_H_

#include "stdint.h"
#include "stdio.h"


#define PROGRAM_START   0x0200
#define FLAG_REG        15
#define MAX_INTEGER_8BIT     255


/*
 *  CHIP-8 registers
 */
typedef struct registers {
	// 16 data registers -- Note: V[15] is the flag register
	uint8_t V[16];

	uint16_t I;   // address register - involved in mem. operations
	uint16_t pc;   // program counter
	uint16_t sp;   // stack pointer
} Chip8;


/*
 *  Stack used to store the return addresses from subroutines
 */
 uint16_t stack[16];


 /*
  *  Memory - CHIP-8 has 4KB of RAM 
  */
unsigned char memory[4096];


void fde_cycle(void);
void initialize_cpu(Chip8 * cpu_reg);


/****************************************************************/
/*************          Chip-8 Intructions          *************/
/****************************************************************/
void SYS_addr(uint16_t opcode, Chip8 * cpu_reg);
void CLS(uint16_t opcode, Chip8 * cpu_reg);
void RET(uint16_t opcode, Chip8 * cpu_reg);
void JP_addr(uint16_t opcode, Chip8 * cpu_reg);
void CALL_addr(uint16_t opcode, Chip8 * cpu_reg);
void SE_VX_byte(uint16_t opcode, Chip8 * cpu_reg);
void SNE_VX_byte(uint16_t opcode, Chip8 * cpu_reg);
void SE_VX_VY(uint16_t opcode, Chip8 * cpu_reg);
void LD_VX_byte(uint16_t opcode, Chip8 * cpu_reg);
void ADD_VX_byte(uint16_t opcode, Chip8 * cpu_reg);
void LD_VX_VY(uint16_t opcode, Chip8 * cpu_reg);
void OR_VX_VY(uint16_t opcode, Chip8 * cpu_reg);
void AND_VX_VY(uint16_t opcode, Chip8 * cpu_reg);
void XOR_VX_VY(uint16_t opcode, Chip8 * cpu_reg);
void ADD_VX_VY(uint16_t opcode, Chip8 * cpu_reg);
void SUB_VX_VY(uint16_t opcode, Chip8 * cpu_reg);


#endif