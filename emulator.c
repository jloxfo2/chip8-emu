// CHIP-8 Emulator
#include "cpu.h"
#include "emulator.h"


uint16_t memory[4096];   // CHIP-8 has 4KB of RAM


int main() {
	Chip8 cpu_reg;

	srand(time(NULL));
	initialize_cpu(&cpu_reg);
	load_program("Tetris.ch8", (void*)(memory + PROGRAM_START));

	/*while (1) {
		fde_cycle(&cpu_reg);
	}
*/
}



/*
 *	load_program()
 *	Inputs: filename - Name of the ROM file to be loaded
 *	        mem_loc - Location in RAM where the ROM is to be read into
 *	Return Value: None
 *	Function: Attempts to open the given filename and load it into RAM
 */
void load_program(const char *filename, void *mem_loc) {
	FILE * f;
	f = fopen(filename,"r");

	if (f != NULL) {
		if (fseek(f, 0, SEEK_END) == 0) {
			uint32_t file_size = ftell(f);
			fseek(f, 0, SEEK_SET);   // go back to beginning of file
			//fread(mem_loc, sizeof(uint16_t), file_size, f);
			printf("File size: %d\n", file_size);
			fclose(f);
		}
	}
}
