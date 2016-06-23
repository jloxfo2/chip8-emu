#ifndef _EMULATOR_H_
#define _EMULATOR_H_


int load_program(const char *filename, uint8_t *mem_loc);
void hex_dump_ROM(int file_size);

#endif