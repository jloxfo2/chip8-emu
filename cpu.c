// CHIP-8 CPU
#include "cpu.h"


/*
 *	fde_cycle()
 *	Inputs:  None
 *	Return Value: None
 *	Function: Reads in the opcode, decodes it, and then executes it
 */
void fde_cycle() {
	// Fetch
	unsigned short opcode = 0x0000  // change this later

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


/***************************************************************/
/*************               Opcodes               *************/
/***************************************************************/


void SYS_addr(unsigned short addr) {
	reg.pc = 0x0FFF addr
}
