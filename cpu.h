#ifndef _CPU_H_
#define _CPU_H_

#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"


#define PROGRAM_START        0x0200
#define FLAG_REG             15
#define MAX_INTEGER_8BIT     255
#define WIDTH                64
#define HEIGHT               32


uint16_t stack[16];   // Stack used to store the return addresses from subroutines
extern uint16_t memory[4096];   // CHIP-8 has 4KB of RAM


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


void fde_cycle(Chip8 * cpu_reg);
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
void SHR_VX_VY(uint16_t opcode, Chip8 * cpu_reg);
void SUBN_VX_VY(uint16_t opcode, Chip8 * cpu_reg);
void SHL_VX_VY(uint16_t opcode, Chip8 * cpu_reg);
void SNE_VX_VY(uint16_t opcode, Chip8 * cpu_reg);
void LD_I_addr(uint16_t opcode, Chip8 * cpu_reg);
void JP_V0_addr(uint16_t opcode, Chip8 * cpu_reg);
void RND_VX_byte(uint16_t opcode, Chip8 * cpu_reg);
void DRW_VX_VY_nibble(uint16_t opcode, Chip8 * cpu_reg);
void SKP_VX(uint16_t opcode, Chip8 * cpu_reg);
void SKNP_VX(uint16_t opcode, Chip8 * cpu_reg);
void LD_VX_DT(uint16_t opcode, Chip8 * cpu_reg);
void LD_VX_K(uint16_t opcode, Chip8 * cpu_reg);
void LD_DT_VX(uint16_t opcode, Chip8 * cpu_reg);
void LD_ST_VX(uint16_t opcode, Chip8 * cpu_reg);
void ADD_I_VX(uint16_t opcode, Chip8 * cpu_reg);
void LD_F_VX(uint16_t opcode, Chip8 * cpu_reg);
void LD_B_VX(uint16_t opcode, Chip8 * cpu_reg);
void LD_I_VX(uint16_t opcode, Chip8 * cpu_reg);
void LD_VX_I(uint16_t opcode, Chip8 * cpu_reg);


#endif