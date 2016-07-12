// CHIP-8 Emulator
#include "cpu.h"
#include "emulator.h"
#include "GL/glut.h"


Chip8 cpu_reg;

uint8_t memory[4096];
uint8_t video_buffer[WIDTH * HEIGHT];
uint16_t delay_timer;   // Used for timeing of game events
uint16_t sound_timer;   // Used for sound effects; beeps when nonzero
uint8_t keys[16];  // key states for hex keypad


int main(int argc, char **argv) {
	srand(time(NULL));
	
	initialize_cpu(&cpu_reg);

	// Attempt to load ROM file. If file fails to open, terminate program
	if (load_program("Tetris.ch8", memory + PROGRAM_START) == -1)
		exit(1);

	// Initialize GLUT and create the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(640, 320);
	glutCreateWindow("CHIP-8 Emulator");

	initGLUT();

	// Set callback functions
	glutKeyboardFunc(key_down);
	glutKeyboardUpFunc(key_up);
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}


/*
 *	display()
 *	Inputs: None
 *	Return Value: None
 *	Function: 
 */
void display() {
	// execute one cpu cycle
	fde_cycle(&cpu_reg);

	draw_screen();
}


/*
 *	draw_screen()
 *	Inputs: None
 *	Return Value: None
 *	Function: Draw to the window screen
 */
void draw_screen() {
	glClear(GL_COLOR_BUFFER_BIT);  // clear the screen
	glLoadIdentity();  // reset the view
	glColor3f(1.0, 1.0, 1.0);

	for (int i=0; i < HEIGHT; ++i) {
		for (int j=0; j < WIDTH; ++j) {
			if (video_buffer[j + i*WIDTH] == 1) {
				float x = j * 10;
				float y = i * 10;

				// draw the pixel
				glBegin(GL_QUADS);
					glVertex3f(x, y, 0.0);
					glVertex3f(x + 10, y, 0.0);
					glVertex3f(x + 10, y + 10, 0.0);
					glVertex3f(x, y + 10, 0.0);
				glEnd();
			}
		}
	}

	glutSwapBuffers();
}


/*
 *	idle()
 *	Inputs: None
 *	Return Value: None
 *	Function: Post a re-paint request to activate display()
 */
void idle() {
   glutPostRedisplay();
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


/*
 *	key_down()
 *	Inputs: key - ASCII char representing the key pressed in the window
 *	        x - Unused
 *	        y - Unused
 *	Return Value: None
 *	Function: Maps a key press to its corresponding hexpad value 
 */
void key_down(unsigned char key, int x, int y) {
	switch(key) {
	case '1':
		keys[0x0] = 1;
		break;
	case '2':
		keys[0x1] = 1;
		break;
	case '3':
		keys[0x2] = 1;
		break;
	case '4':
		keys[0x3] = 1;
		break;
	case 'q':
		keys[0x4] = 1;
		break;
	case 'w':
		keys[0x5] = 1;
		break;
	case 'e':
		keys[0x6] = 1;
		break;
	case 'r':
		keys[0x7] = 1;
		break;
	case 'a':
		keys[0x8] = 1;
		break;
	case 's':
		keys[0x9] = 1;
		break;
	case 'd':
		keys[0xA] = 1;
		break;
	case 'f':
		keys[0xB] = 1;
		break;
	case 'z':
		keys[0xC] = 1;
		break;
	case 'x':
		keys[0xD] = 1;
		break;
	case 'c':
		keys[0xE] = 1;
		break;
	case 'v':
		keys[0xF] = 1;
		break;
	default:
		break;
	}
}


/*
 *	key_up()
 *	Inputs: key - ASCII char representing the key pressed in the window
 *	        x - Unused
 *	        y - Unused
 *	Return Value: None
 *	Function: Maps a key release to its corresponding hexpad value 
 */
void key_up(unsigned char key, int x, int y) {
	switch(key) {
	case '1':
		keys[0x0] = 0;
		break;
	case '2':
		keys[0x1] = 0;
		break;
	case '3':
		keys[0x2] = 0;
		break;
	case '4':
		keys[0x3] = 0;
		break;
	case 'q':
		keys[0x4] = 0;
		break;
	case 'w':
		keys[0x5] = 0;
		break;
	case 'e':
		keys[0x6] = 0;
		break;
	case 'r':
		keys[0x7] = 0;
		break;
	case 'a':
		keys[0x8] = 0;
		break;
	case 's':
		keys[0x9] = 0;
		break;
	case 'd':
		keys[0xA] = 0;
		break;
	case 'f':
		keys[0xB] = 0;
		break;
	case 'z':
		keys[0xC] = 0;
		break;
	case 'x':
		keys[0xD] = 0;
		break;
	case 'c':
		keys[0xE] = 0;
		break;
	case 'v':
		keys[0xF] = 0;
		break;
	default:
		break;
	}
}


/*
 *	initGLUT()
 *	Inputs: None
 *	Return Value: None
 *	Function: Initializes GLUT for 2D drawing
 */
void initGLUT() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // black and opaque
	
	// sets up GLUT window for 2D drawing
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, WIDTH * 10, HEIGHT * 10, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
