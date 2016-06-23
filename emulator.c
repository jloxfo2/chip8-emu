// CHIP-8 Emulator
#include "cpu.h"
#include "emulator.h"


uint8_t memory[4096];   // CHIP-8 has 4KB of RAM


int main() {
	Chip8 cpu_reg;
	srand(time(NULL));
	
	initialize_cpu(&cpu_reg);
	if (load_program("Tetris.ch8", memory + PROGRAM_START) == -1)
		exit(1);   // if file fails to open, terminate program
	
	/*while (1) {
		fde_cycle(&cpu_reg);
	}
*/
}



/*
 *	load_program()
 *	Inputs: filename - Name of the ROM file to be loaded
 *	        mem_loc - Location in RAM where the ROM is to be read into
 *	Return Value: Returns 0 if file is read into ROM successfully;
 *	              returns -1 on failure
 *	Function: Attempts to open the given filename and load it into RAM
 */
int load_program(const char *filename, uint8_t *mem_loc) {
	FILE * f;
	f = fopen(filename,"r");

	if (f != NULL) {
		if (fseek(f, 0, SEEK_END) == 0) {
			uint32_t file_size = ftell(f);
			fseek(f, 0, SEEK_SET);   // go back to beginning of file
			fread(mem_loc, sizeof(uint16_t), file_size, f);
			fclose(f);
			return 0;
		}
	}

	return -1;
}




/*****************************************************************/
/**************       Helpful debugging tools       **************/
/*****************************************************************/


/*
 *	hex_dump_ROM()
 *	Inputs: file_size - Size of the *.ch8 file read into ROM
 *	Return Value: None
 *	Function: Prints the ROM which is in RAM at location 0x0200
 */
void hex_dump_ROM(int file_size) {
	int s = PROGRAM_START;

	for (int i=0; i < file_size; i+=2)
		printf("%02X%02X\n",memory[s+i],memory[s+i+1]);
}
