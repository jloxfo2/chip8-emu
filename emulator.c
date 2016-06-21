// CHUIP-8 Emulator
#include "cpu.h"


int main() {
	Chip8 cpu_reg;

	srand(time(NULL));
	initialize_cpu(&cpu_reg);

	while (1) {
		fde_cycle(&cpu_reg);
	}

}
