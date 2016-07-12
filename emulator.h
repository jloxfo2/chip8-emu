#ifndef _EMULATOR_H_
#define _EMULATOR_H_


int load_program(const char *filename, uint8_t *mem_loc);
void hex_dump_ROM(int file_size);
void key_down(unsigned char key, int x, int y);
void key_up(unsigned char key, int x, int y);

void initGLUT(void);
void display(void);
void draw_screen(void);
void idle(void);

#endif