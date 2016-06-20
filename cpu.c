// CHIP-8 CPU
#include "cpu.h"


/*
 *	fde_cycle()
 *	Inputs: None
 *	Return Value: None
 *	Function: Reads in the opcode, decodes it, and then executes it
 */
void fde_cycle() {
	// Fetch
	uint16_t opcode = 0x0000;  // change this later - need to read 2 consecutive bytes

	// Decode the opcode and execute it by calling its function
	switch(opcode & 0xF000) {
	case 0x0000:
		// another switch case here
		break;
	case 0x1000:
		break;
	case 0x2000:
		break;
	case 0x3000:
		break;
	case 0x4000:
		break;
	case 0x5000:
		break;
	case 0x6000:
		break;
	case 0x7000:
		break;
	case 0x8000:
		// another switch case here
		break;
	case 0x9000:
		break;
	case 0xA000:
		break;
	case 0xB000:
		break;
	case 0xC000:
		break;
	case 0xD000:
		break;
	case 0xE000:
		// another switch case here
		break;
	case 0xF000:
		// another switch case here
		break;
	default:
		printf("Unknown opcode\n");
		break;
	}

}


/*
 *	initialize_cpu()
 *	Inputs: cpu_reg - Pointer to CPU register struct
 *	Return Value: None
 *	Function: Initializes all of the CPU register values
 */
void initialize_cpu(Chip8 * cpu_reg) {
	uint16_t pc = PROGRAM_START;
	uint16_t sp = 0;
	uint16_t I = 0;

	for (int i=0; i<16; ++i)
		cpu_reg->V[i] = 0;
}



/****************************************************************/
/*************          Chip-8 Intructions          *************/
/****************************************************************/
/* Note: All instructions take an opcode and a pointer to a CPU 
         register struct as their arguments */

/*
 *  0x0NNN - Jump to a machine code routine at NNN
 */
void SYS_addr(uint16_t opcode, Chip8 * cpu_reg) {
	cpu_reg->pc = 0x0FFF & opcode;
}


/*
 *  0x00E0 - Clear the display
 */
void CLS(uint16_t opcode, Chip8 * cpu_reg) {
	// clear the display
}


/*
 *  0x00EE - Return from a subroutine
 */
void RET(uint16_t opcode, Chip8 * cpu_reg) {
	cpu_reg->pc = stack[--cpu_reg->sp];
	cpu_reg->pc += 2;
}


/*
 *  0x1NNN - Jump to location NNN
 */
void JP_addr(uint16_t opcode, Chip8 * cpu_reg) {
	cpu_reg->pc = opcode & 0x0FFF;
}


/*
 *  0x2NNN - Call subroutine at NNN
 */
void CALL_addr(uint16_t opcode, Chip8 * cpu_reg) {
	stack[cpu_reg->sp] = cpu_reg->pc;
	cpu_reg->sp++;
	cpu_reg->pc = opcode & 0x0FFF;
}


/*
 *  0x3XKK - Skip next instruction if VX == KK
 */
void SE_VX_byte(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;

	if (cpu_reg->V[X] == (opcode & 0x00FF))
		cpu_reg->pc += 2;
}


/*
 *  0x4XKK - Skip next instruction if VX != KK
 */
void SNE_VX_byte(uint16_t opcode, Chip8 * cpu_reg) {
 	uint32_t X = (opcode & 0x0F00) >> 8;

 	if (cpu_reg->V[X] != (opcode & 0x00FF))
 		cpu_reg->pc += 2;
 }


/*
 *  0x5XY0 - Skip next instruction if VX == VY
 */
void SE_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	uint32_t Y = (opcode & 0x00F0) >> 4;

	if (cpu_reg->V[X] == cpu_reg->V[Y])
		cpu_reg->pc += 2;
}


/*
 *  0x6XKK - Put the value KK into register VX
 */
void LD_VX_byte(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	cpu_reg->V[X] = (opcode & 0x00FF);
}


/*
 *  0x7XKK - Add the value KK to the value of VX, then store result in VX
 */
void ADD_VX_byte(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	cpu_reg->V[X] += (opcode & 0x00FF);
}


/*
 *  0x8XY0 - Set VX = Vy
 */
void LD_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
 	uint32_t X = (opcode & 0x0F00) >> 8;
 	uint32_t Y = (opcode & 0x00F0) >> 4;
 	cpu_reg->V[X] = cpu_reg->V[Y];
}


/*
 *  0x8XY1 - Set VX = VX OR VY
 */
void OR_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	uint32_t Y = (opcode & 0x00F0) >> 4;
	cpu_reg->V[X] = cpu_reg->V[X] | cpu_reg->V[Y];
}


/*
 *  0x8XY2 - Set VX = VX AND VY
 */
void AND_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
  	uint32_t X = (opcode & 0x0F00) >> 8;
 	uint32_t Y = (opcode & 0x00F0) >> 4;
 	cpu_reg->V[X] = cpu_reg->V[X] & cpu_reg->V[Y];
}


/*
 *  0x8XY3 - Set VX = VX XOR VY
 */
void XOR_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
 	uint32_t X = (opcode & 0x0F00) >> 8;
 	uint32_t Y = (opcode & 0x00F0) >> 4;
 	cpu_reg->V[X] = cpu_reg->V[X] ^ cpu_reg->V[Y];
}


/*
 *  0x8XY4 - Set VX = VX + VY, set VF = carry
 */
void ADD_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
 	uint32_t Y = (opcode & 0x00F0) >> 4;

 	// check for overflow 
 	if (cpu_reg->V[Y] > (MAX_INTEGER_8BIT - cpu_reg->V[X]))
 		cpu_reg->V[FLAG_REG] = 1;
 	else
 		cpu_reg->V[FLAG_REG] = 0;

 	cpu_reg->V[X] = cpu_reg->V[X] + cpu_reg->V[Y];
}


/*
 *  0x8XY5 - Set VX = VX - VY, set VF = NOT borrow
 */
void SUB_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
 	uint32_t Y = (opcode & 0x00F0) >> 4;

 	// check for a borrow
 	if (cpu_reg->V[X] > cpu_reg->V[Y])
 		cpu_reg->V[FLAG_REG] = 1;
 	else
 		cpu_reg->V[FLAG_REG] = 0;

 	cpu_reg->V[X] = cpu_reg->V[X] - cpu_reg->V[Y];
}
