// CHIP-8 CPU
#include "cpu.h"


static uint16_t stack[16];   // Stack used to store the return addresses from subroutines
static uint16_t delay_timer;   // Used for timeing of game events
static uint16_t sound_timer;   // Used for sound effects; beeps when nonzero


/*
 *  Hexadecimal sprite fonts (16 total digits, each 5 bytes long)
 */
static uint8_t fonts[80] = {
	0xF0,0x90,0x90,0x90,0xF0,  // '0'
	0x20,0x60,0x20,0x20,0x70,  // '1'
	0xF0,0x10,0xF0,0x80,0xF0,  // '2'
	0xF0,0x10,0xF0,0x10,0xF0,  // '3'
	0x90,0x90,0xF0,0x10,0x10,  // '4'
	0xF0,0x10,0x20,0x40,0x40,  // '5'
	0xF0,0x80,0xF0,0x90,0xF0,  // '6'
	0xF0,0x10,0x20,0x40,0x40,  // '7'
	0xF0,0x90,0xF0,0x90,0xF0,  // '8'
	0xF0,0x90,0xE0,0x90,0xE0,  // '9'
	0xF0,0x90,0xF0,0x90,0x90,  // 'A'
	0xE0,0x90,0xE0,0x90,0xE0,  // 'B'
	0xF0,0x80,0x80,0x80,0xF0,  // 'C'
	0xE0,0x90,0x90,0x90,0xE0,  // 'D'
	0xF0,0x80,0xF0,0x80,0xF0,  // 'E'
	0xF0,0x80,0xF0,0x80,0xF0   // 'F'
};


/*
 *	fde_cycle()
 *	Inputs: cpu_reg - Pointer to CPU register struct
 *	Return Value: None
 *	Function: Reads in the opcode, decodes it, and then executes it
 */
void fde_cycle(Chip8 * cpu_reg) {
	// Fetch
	uint16_t opcode = (memory[cpu_reg->pc] << 8) | memory[cpu_reg->pc+1];  // read 2 consecutive bytes
	
	// debugger(cpu_reg, opcode);
	
	// Decode the opcode and execute it by calling its function
	switch(opcode & 0xF000) {
	case 0x0000:
		// check lowest 8-bits
		switch(opcode & 0x00FF) {
		case 0x00E0:
			CLS(opcode, cpu_reg);
			break;
		case 0x00EE:
			RET(opcode, cpu_reg);
			break;
		default:
			JP_addr(opcode, cpu_reg);
			break;
		}
		break;
	case 0x1000:
		JP_addr(opcode, cpu_reg);
		break;
	case 0x2000:
		CALL_addr(opcode, cpu_reg);
		break;
	case 0x3000:
		SE_VX_byte(opcode, cpu_reg);
		break;
	case 0x4000:
		SNE_VX_byte(opcode, cpu_reg);
		break;
	case 0x5000:
		SE_VX_VY(opcode, cpu_reg);
		break;
	case 0x6000:
		LD_VX_byte(opcode, cpu_reg);
		break;
	case 0x7000:
		ADD_VX_byte(opcode, cpu_reg);
		break;
	case 0x8000:
		// check lowest 4-bits
		switch(opcode & 0x000F) {
		case 0x0000:
			LD_VX_VY(opcode, cpu_reg);
			break;
		case 0x0001:
			OR_VX_VY(opcode, cpu_reg);
			break;
		case 0x0002:
			AND_VX_VY(opcode, cpu_reg);
			break;
		case 0x0003:
			XOR_VX_VY(opcode, cpu_reg);
			break;
		case 0x0004:
			ADD_VX_VY(opcode, cpu_reg);
			break;
		case 0x0005:
			SUB_VX_VY(opcode, cpu_reg);
			break;
		case 0x0006:
			SHR_VX_VY(opcode, cpu_reg);
			break;
		case 0x0007:
			SUBN_VX_VY(opcode, cpu_reg);
			break;
		case 0x000E:
			SHL_VX_VY(opcode, cpu_reg);
			break;
		default:
			printf("Unknown opcode\n");
			break;
		}
		break;
	case 0x9000:
		SNE_VX_VY(opcode, cpu_reg);
		break;
	case 0xA000:
		LD_I_addr(opcode, cpu_reg);
		break;
	case 0xB000:
		JP_V0_addr(opcode, cpu_reg);
		break;
	case 0xC000:
		RND_VX_byte(opcode, cpu_reg);
		break;
	case 0xD000:
		DRW_VX_VY_nibble(opcode, cpu_reg);
		break;
	case 0xE000:
		// check lowest 8-bits
		switch(opcode & 0x00FF) {
		case 0x009E:
			SKP_VX(opcode, cpu_reg);
			break;
		case 0x00A1:
			SKNP_VX(opcode, cpu_reg);
			break;
		default:
			printf("Unknown opcode\n");
			break;
		}
		break;
	case 0xF000:
		// check lowest 8-bits
		switch (opcode & 0x00FF) {
		case 0x0007:
			LD_VX_DT(opcode, cpu_reg);
			break;
		case 0x000A:
			LD_VX_K(opcode, cpu_reg);
			break;
		case 0x0015:
			LD_DT_VX(opcode, cpu_reg);
			break;
		case 0x0018:
			LD_ST_VX(opcode, cpu_reg);
			break;
		case 0x001E:
			ADD_I_VX(opcode, cpu_reg);
			break;
		case 0x0029:
			LD_F_VX(opcode, cpu_reg);
			break;
		case 0x0033:
			LD_B_VX(opcode, cpu_reg);
			break;
		case 0x0055:
			LD_I_VX(opcode, cpu_reg);
			break;
		case 0x0065:
			LD_VX_I(opcode, cpu_reg);
			break;
		default:
			printf("Unknown opcode\n");
			break;
		}
		break;
	default:
		printf("Unknown opcode\n");
		break;
	}

	// Decrement the timers
	if (delay_timer > 0)
		delay_timer--;
	if (sound_timer > 0) {
		// Make beeping sound
		sound_timer--;
	}
}


/*
 *	initialize_cpu()
 *	Inputs: cpu_reg - Pointer to CPU register struct
 *	Return Value: None
 *	Function: Initializes all of the CPU register values
 */
void initialize_cpu(Chip8 * cpu_reg) {
	cpu_reg->pc = PROGRAM_START;
	cpu_reg->sp = 0;
	cpu_reg->I = 0;

	sound_timer = 0;
	delay_timer = 0;

	// initialize all memory/registers to 0
	memset(cpu_reg->V, 0, sizeof(cpu_reg->V));
	memset(stack, 0, sizeof(stack));
	memset(video_buffer, 0, sizeof(video_buffer));
	memset(memory, 0, sizeof(memory));

	// load sprite fonts into memory
	for (int i=0; i<80; ++i) {
		memory[i] = fonts[i];
	}
}


void debugger(Chip8* cpu_reg, uint16_t opcode) {
	printf("opcode = %02X\n", opcode);
	printf("V[0] = %d\n", cpu_reg->V[0]);
	printf("V[1] = %d\n", cpu_reg->V[1]);
	printf("V[2] = %d\n", cpu_reg->V[2]);
	printf("V[3] = %d\n", cpu_reg->V[3]);
	printf("V[4] = %d\n", cpu_reg->V[4]);
	printf("V[5] = %d\n", cpu_reg->V[5]);
	printf("V[6] = %d\n", cpu_reg->V[6]);
	printf("V[7] = %d\n", cpu_reg->V[7]);
	printf("V[8] = %d\n", cpu_reg->V[8]);
	printf("V[9] = %d\n", cpu_reg->V[9]);
	printf("V[10] = %d\n", cpu_reg->V[10]);
	printf("V[11] = %d\n", cpu_reg->V[11]);
	printf("V[12] = %d\n", cpu_reg->V[12]);
	printf("V[13] = %d\n", cpu_reg->V[13]);
	printf("V[14] = %d\n", cpu_reg->V[14]);
	printf("V[15] = %d\n", cpu_reg->V[15]);

	printf("pc = %d\n", cpu_reg->pc);
	printf("sp = %d\n", cpu_reg->sp);
	printf("I = %d\n", cpu_reg->I);

	getchar();  // Wait for user input before continuing
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
	// reset all array values to 0
	memset(video_buffer, 0, sizeof(video_buffer));
	cpu_reg->pc += 2;
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
	stack[cpu_reg->sp] = cpu_reg->pc;  // push current addr onto stack
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

	cpu_reg->pc += 2;
}


/*
 *  0x4XKK - Skip next instruction if VX != KK
 */
void SNE_VX_byte(uint16_t opcode, Chip8 * cpu_reg) {
 	uint32_t X = (opcode & 0x0F00) >> 8;

 	if (cpu_reg->V[X] != (opcode & 0x00FF))
 		cpu_reg->pc += 2;

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

	cpu_reg->pc += 2;
}


/*
 *  0x6XKK - Put the value KK into register VX
 */
void LD_VX_byte(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	cpu_reg->V[X] = (opcode & 0x00FF);
	cpu_reg->pc += 2;
}


/*
 *  0x7XKK - Add the value KK to the value of VX, then store result in VX
 */
void ADD_VX_byte(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	cpu_reg->V[X] += (opcode & 0x00FF);
	cpu_reg->pc += 2;
}


/*
 *  0x8XY0 - Set VX = Vy
 */
void LD_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
 	uint32_t X = (opcode & 0x0F00) >> 8;
 	uint32_t Y = (opcode & 0x00F0) >> 4;
 	cpu_reg->V[X] = cpu_reg->V[Y];
 	cpu_reg->pc += 2;
}


/*
 *  0x8XY1 - Set VX = VX OR VY
 */
void OR_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	uint32_t Y = (opcode & 0x00F0) >> 4;
	cpu_reg->V[X] = cpu_reg->V[X] | cpu_reg->V[Y];
	cpu_reg->pc += 2;
}


/*
 *  0x8XY2 - Set VX = VX AND VY
 */
void AND_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
  	uint32_t X = (opcode & 0x0F00) >> 8;
 	uint32_t Y = (opcode & 0x00F0) >> 4;
 	cpu_reg->V[X] = cpu_reg->V[X] & cpu_reg->V[Y];
 	cpu_reg->pc += 2;
}


/*
 *  0x8XY3 - Set VX = VX XOR VY
 */
void XOR_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
 	uint32_t X = (opcode & 0x0F00) >> 8;
 	uint32_t Y = (opcode & 0x00F0) >> 4;
 	cpu_reg->V[X] = cpu_reg->V[X] ^ cpu_reg->V[Y];
 	cpu_reg->pc += 2;
}


/*
 *  0x8XY4 - Set VX = VX + VY, set VF = carry
 */
void ADD_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
 	uint32_t Y = (opcode & 0x00F0) >> 4;

 	// check for overflow & set flag=1 if there is a carry
 	if (cpu_reg->V[Y] > (MAX_INTEGER_8BIT - cpu_reg->V[X]))
 		cpu_reg->V[FLAG_REG] = 1;
 	else
 		cpu_reg->V[FLAG_REG] = 0;

 	cpu_reg->V[X] = cpu_reg->V[X] + cpu_reg->V[Y];
 	cpu_reg->pc += 2;
}


/*
 *  0x8XY5 - Set VX = VX - VY, set VF = NOT borrow
 */
void SUB_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	uint32_t Y = (opcode & 0x00F0) >> 4;

	// check for a borrow; set flag=1 is there IS NOT a borrow
	if (cpu_reg->V[X] > cpu_reg->V[Y])
		cpu_reg->V[FLAG_REG] = 1;
	else
		cpu_reg->V[FLAG_REG] = 0;

	cpu_reg->V[X] = cpu_reg->V[X] - cpu_reg->V[Y];
	cpu_reg->pc += 2;
}


/*
 *  0x8XY6 - Set VX = VX SHR 1, set VF = least sig. bit of VX
 */
void SHR_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;

	// check least significant bit & set flag=1 if it's 1
	cpu_reg->V[FLAG_REG] = (cpu_reg->V[X] & 0x1);

	cpu_reg->V[X] = cpu_reg->V[X] >> 1;
	cpu_reg->pc += 2;
}


/*
 *  0x8XY7 - Set VX = VY - VX, set VF = NOT borrow
 */
void SUBN_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	uint32_t Y = (opcode & 0x00F0) >> 4;

	// check for a borrow; set flag=1 is there IS NOT a borrow
	if (cpu_reg->V[Y] > cpu_reg->V[X])
		cpu_reg->V[FLAG_REG] = 1;
	else
		cpu_reg->V[FLAG_REG] = 0;

	cpu_reg->V[X] = cpu_reg->V[Y] - cpu_reg->V[X];
	cpu_reg->pc += 2;
}


/*
 *  0x8XYE - Set VX = VX SHL 1, set VF = most sig. bit of VX
 */
void SHL_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;

	// check most significant bit & set flag=1 if it's 1
	cpu_reg->V[FLAG_REG] = (cpu_reg->V[X] & 0x8) >> 7;

	cpu_reg->V[X] = cpu_reg->V[X] << 1;
	cpu_reg->pc += 2;
}


/*
 *  0x9XY0 - Skip the next instruction if VX != VY
 */
void SNE_VX_VY(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	uint32_t Y = (opcode & 0x00F0) >> 4;

	if (cpu_reg->V[X] != cpu_reg->V[Y])
		cpu_reg->pc += 2;

	cpu_reg->pc += 2;
}


/*
 *  0xANNN - Set I = NNN
 */
void LD_I_addr(uint16_t opcode, Chip8 * cpu_reg) {
	cpu_reg->I = (opcode & 0x0FFF);
	cpu_reg->pc += 2;
}


/*
 *  0xBNNN - Jump to location (NNN + V0)
 */
void JP_V0_addr(uint16_t opcode, Chip8 * cpu_reg) {
	cpu_reg->pc = cpu_reg->V[0] + (opcode & 0x0FFF);
}


/*
 *  0xCXKK - Set VX = random byte AND KK
 */
void RND_VX_byte(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	uint32_t random = rand() % 256;   // generate a random number from 0 to 255

	cpu_reg->V[X] = random & (opcode & 0x00FF);
	cpu_reg->pc += 2;
}


/*
 *  0xDXYN - Display N-byte sprite starting at memory location I at (VX,VY), set VF = collison
 */
void DRW_VX_VY_nibble(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	uint32_t Y = (opcode & 0x00F0) >> 4;
	uint32_t N = (opcode & 0x000F);
	cpu_reg->V[0xF] = 0;  // clear collision flag

	// (x, y) position
	uint32_t x = cpu_reg->V[X];
	uint32_t y = cpu_reg->V[Y];

	for (int yVal=0; yVal < N; ++yVal) {
		uint8_t sprite_byte = memory[cpu_reg->I + yVal];

		for (int xVal=0; xVal < 8; ++xVal) {
			if (sprite_byte & (0x01 << xVal)) {
				// if the current pixel is set, check it's value after XOR'ing it with its sprite font pixel
				if (video_buffer[x + xVal + (y + yVal) * WIDTH] == 1)
					// if the pixel is turned off, set collision flag
					cpu_reg->V[0xF] = 1;
				
				video_buffer[x + xVal + (y + yVal) * WIDTH] ^= 1;
			}
			else {
				video_buffer[x + xVal + (y + yVal) * WIDTH] ^= 0;
			}
		}
	}

	cpu_reg->pc += 2;
}


/*
 *  0xEX9E - Skip next instruction if key with the value of VX is pressed
 */
void SKP_VX(uint16_t opcode, Chip8 * cpu_reg) {
	int X = (opcode & 0x0F00) >> 8;

	if (keys[cpu_reg->V[X]] == 1)
		cpu_reg->pc += 2;
	
	cpu_reg->pc += 2;
}


/*
 *  0xEXA1 - Skip next instruction if key with the value of VX is not pressed
 */
void SKNP_VX(uint16_t opcode, Chip8 * cpu_reg) {
	int X = (opcode & 0x0F00) >> 8;

	if (keys[cpu_reg->V[X]] == 0)
		cpu_reg->pc += 2;

	cpu_reg->pc += 2;
}


/*
 *  0xFX07 - Set VX = delay timer value
 */
void LD_VX_DT(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	cpu_reg->V[X] = delay_timer;
	cpu_reg->pc += 2;
}


/*
 * 0xFX0A - Wait for a key press, store the value of the key in VX
 */
void LD_VX_K(uint16_t opcode, Chip8 * cpu_reg) {
	int X = (opcode & 0x0F00) >> 8;

	for (int i=0; i<16; ++i) {
		if (keys[i] == 1) {
			cpu_reg->V[X] = i;
			cpu_reg->pc += 2;  // increment pc if key pressed; cpu will return here if not
			break;
		}
	}
}


/*
 *  0xFX15 - Set delay timer = VX
 */
void LD_DT_VX(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	delay_timer = cpu_reg->V[X];
	cpu_reg->pc += 2;
}


/*
 *  0xFX18 - Set sound timer = VX
 */
void LD_ST_VX(uint16_t opcode, Chip8 * cpu_reg) {
	// set ST = VX
	uint32_t X = (opcode & 0x0F00) >> 8;
	sound_timer = cpu_reg->V[X];
	cpu_reg->pc += 2;
}


/*
 *  0xFX1E - Set I = I + VX
 */
void ADD_I_VX(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	cpu_reg->I += cpu_reg->V[X];
	cpu_reg->pc += 2;
}


/*
 *  0xFX29 - Set I = location of sprite for digit VX
 */
void LD_F_VX(uint16_t opcode, Chip8 * cpu_reg) {
	uint32_t X = (opcode & 0x0F00) >> 8;
	cpu_reg->I = cpu_reg->V[X] * 5;  // multiplied by 5 because sprites are 5 bytes long
	cpu_reg->pc += 2;
}


/*
 *  0xFX33 - Store the BCD representation of VX in mem. locations I, I+1, and I+2
 */
void LD_B_VX(uint16_t opcode, Chip8 * cpu_reg) {
	// take decimal value of V[X] and store its hundreds digit at mem. loc. I,
	// its tens digit at I+1, and its ones digit at I+2
	int X = (opcode & 0x0F00) >> 8;

	memory[cpu_reg->I] = cpu_reg->V[X] / 100;
	memory[cpu_reg->I+1] = (cpu_reg->V[X] % 100) / 10;
	memory[cpu_reg->I+2] = cpu_reg->V[X] % 10;

	cpu_reg->pc += 2;
}


/*
 *  0xFX55 - Store registers V0 through VX in memory starting at location I
 */
void LD_I_VX(uint16_t opcode, Chip8 * cpu_reg) {
	int X = (opcode & 0x0F00) >> 8;

	for (int k=0; k <= X; ++k)
		memory[cpu_reg->I + k] = cpu_reg->V[k];

	cpu_reg->pc += 2;
}


/*
 *  0xFX65 - Read into registers V0 through VX from memory starting at location I
 */
void LD_VX_I(uint16_t opcode, Chip8 * cpu_reg) {
	int X = (opcode & 0x0F00) >> 8;

	for (int k=0; k <= X; ++k)
		cpu_reg->V[k] = memory[cpu_reg->I + k];

	cpu_reg->pc += 2;
}

