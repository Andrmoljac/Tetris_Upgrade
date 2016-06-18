/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xio.h"
#include "vga_periph_mem.h"

#define TEXT_MEM_OFF 0x1000000
#define VGA_PERIPH_MEM_mWriteMemory(Address, Data) \
 	Xil_Out32(Address, (Xuint32)(Data))

void print(char *str);


#define HEIGHT 30
#define WIDTH 40
u8 matrix[HEIGHT][WIDTH];


void empty_matrix(u8 matrix[HEIGHT][WIDTH])
{
	int i, j;

	for(i = 0; i<HEIGHT; i++)
	{
		for(j = 0; j<WIDTH; j++)
		{
			matrix[i][j] = ' ';
		}
	}
}


void print_start_string(u8 matrix[HEIGHT][WIDTH])
{
	char t = 'T';
	char e = 'E';
	char r = 'R';
	char i = 'I';
	char s = 'S';
	char n1 = '2';

	empty_matrix(matrix);		/* Empty the matrix */

	matrix[13][12] = t;			/* Load the startup string */
	matrix[13][14] = e;
	matrix[13][16] = t;
	matrix[13][18] = r;
	matrix[13][20] = i;
	matrix[13][22] = s;
	matrix[13][26] = n1;

	print_matrix(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, matrix);		/* Print the whole matrix - update what's on the screen */
}


void print_new_game(u8 matrix[HEIGHT][WIDTH], int* score)
{
	char n = 'N';
	char e = 'E';
	char w = 'W';
	char g = 'G';
	char a = 'A';
	char m = 'M';

	*score = 0;

	wait();

	empty_matrix(matrix);		/* Empty the matrix */

	matrix[13][13] = n;			/* Load the new game string */
	matrix[13][15] = e;
	matrix[13][17] = w;

	matrix[13][21] = g;
	matrix[13][23] = a;
	matrix[13][25] = m;
	matrix[13][27] = e;

	print_matrix(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, matrix);		/* Print the whole matrix - update what's on the screen */

	empty_matrix(matrix);		/* Empty the matrix */

	wait();

	load_matrix(matrix);		/* Setup the matrix */
}


void print_game_over(u8 matrix[HEIGHT][WIDTH])
{
	char g = 'G';
	char a = 'A';
	char m = 'M';
	char e = 'E';
	char o = 'O';
	char v = 'V';
	char r = 'R';

	empty_matrix(matrix);		/* Empty the matrix */

	matrix[13][12] = g;			/* Load the game over string */
	matrix[13][14] = a;
	matrix[13][16] = m;
	matrix[13][18] = e;

	matrix[13][22] = o;
	matrix[13][24] = v;
	matrix[13][26] = e;
	matrix[13][28] = r;

	print_matrix(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, matrix);		/* Print the whole matrix - update what's on the screen */
}


void load_matrix(u8 matrix[HEIGHT][WIDTH])
{
	char s = 'S';
	char c = 'C';
	char o = 'O';
	char r = 'R';
	char e = 'E';
	char t = 'T';
	char i = 'I';
	char k = 'K';

	char z1 = '(';
	char z2 = ')';
	char line = '-';

	char n1 = '2';
	char n2 = '0';
	char n3 = '1';
	char n4 = '6';
	int p;

	empty_matrix(matrix);		/* Empty the matrix */

	matrix[0][1] = s;			/* Load and initialize the score */
	matrix[0][2] = c;
	matrix[0][3] = o;
	matrix[0][4] = r;
	matrix[0][5] = e;

	matrix[2][2] = n2;
	matrix[2][3] = n2;

	matrix[0][27] = z1;			/* Load the RT-RK logo and year */
	matrix[0][28] = c;
	matrix[0][29] = z2;

	matrix[0][32] = r;
	matrix[0][33] = t;
	matrix[0][34] = line;
	matrix[0][35] = r;
	matrix[0][36] = k;

	matrix[2][32] = n1;
	matrix[2][33] = n2;
	matrix[2][34] = n3;
	matrix[2][35] = n4;

	matrix[10][30] = t;			/* Load the name of the game */
	matrix[10][31] = e;
	matrix[10][32] = t;
	matrix[10][33] = r;
	matrix[10][34] = i;
	matrix[10][35] = s;
	matrix[10][37] = n1;

	for (p = 0; p < 30; p++)	/* Load the game frame */
	{
		matrix[p][10] = i;
		matrix[p][21] = i;
	}

	print_matrix(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, matrix);		/* Print the whole matrix - update what's on the screen */
}


int check_game_over(u8 matrix[HEIGHT][WIDTH])
{
	if((matrix[0][14] != ' ') || (matrix[0][15] != ' ') || (matrix[0][16] != ' ') || (matrix[0][17] != ' '))
		return 1;
	else
		return 0;
}


void rand_shapes(int* shape, int *pos, int* i1, int* i2, int* i3, int* i4, int* j1, int* j2, int* j3, int* j4)
{
	int rnd = rand() % 7;

	if(rnd == 0)			/* I shape */
	{
		*i1 = 0;			/* block 1 coordinates */
		*j1 = 14;

		*i2 = 0;			/* block 2 coordinates */
		*j2 = 15;

		*i3 = 0;			/* block 3 coordinates */
		*j3 = 16;

		*i4 = 0;			/* block 4 coordinates */
		*j4 = 17;

		*shape = 0;			/* Set the shape and position */
		*pos = 0;
	}
	else if(rnd == 1)		/* T shape */
	{
		*i1 = 0;			/* block 1 coordinates */
		*j1 = 14;

		*i2 = 0;			/* block 2 coordinates */
		*j2 = 15;

		*i3 = 0;			/* block 3 coordinates */
		*j3 = 16;

		*i4 = 1;			/* block 4 coordinates */
		*j4 = 15;

		*shape = 1;			/* Set the shape and position */
		*pos = 0;
	}
	else if(rnd == 2)		/* L shape */
	{
		*i1 = 0;			/* block 1 coordinates */
		*j1 = 14;

		*i2 = 0;			/* block 2 coordinates */
		*j2 = 15;

		*i3 = 0;			/* block 3 coordinates */
		*j3 = 16;

		*i4 = 1;			/* block 4 coordinates */
		*j4 = 14;

		*shape = 2;			/* Set the shape and position */
		*pos = 0;
	}
	else if(rnd == 3)		/* J shape */
	{
		*i1 = 0;			/* block 1 coordinates */
		*j1 = 14;

		*i2 = 0;			/* block 2 coordinates */
		*j2 = 15;

		*i3 = 0;			/* block 3 coordinates */
		*j3 = 16;

		*i4 = 1;			/* block 4 coordinates */
		*j4 = 16;

		*shape = 3;			/* Set the shape and position */
		*pos = 0;
	}
	else if(rnd == 4)		/* Z shape */
	{
		*i1 = 0;			/* block 1 coordinates */
		*j1 = 14;

		*i2 = 0;			/* block 2 coordinates */
		*j2 = 15;

		*i3 = 1;			/* block 3 coordinates */
		*j3 = 15;

		*i4 = 1;			/* block 4 coordinates */
		*j4 = 16;

		*shape = 4;			/* Set the shape and position */
		*pos = 0;
	}
	else if(rnd == 5)		/* S shape */
	{
		*i1 = 0;			/* block 1 coordinates */
		*j1 = 15;

		*i2 = 0;			/* block 2 coordinates */
		*j2 = 16;

		*i3 = 1;			/* block 3 coordinates */
		*j3 = 14;

		*i4 = 1;			/* block 4 coordinates */
		*j4 = 15;

		*shape = 5;			/* Set the shape and position */
		*pos = 0;
	}
	else if(rnd == 6)		/* O shape */
	{
		*i1 = 0;			/* block 1 coordinates */
		*j1 = 14;

		*i2 = 0;			/* block 2 coordinates */
		*j2 = 15;

		*i3 = 1;			/* block 3 coordinates*/
		*j3 = 14;

		*i4 = 1;			/* block 4 coordinates*/
		*j4 = 15;

		*shape = 6;			/* Set the shape and position */
		*pos = 0;
	}
}


void empty_row(u8 matrix[HEIGHT][WIDTH], int i)
{
	int j;

	for(j = 11; j<21; j++)
	{
		matrix[i][j] = ' ';
	}
}


void move_row(u8 matrix[HEIGHT][WIDTH], int k1, int k2)
{
	int j;

	for(j = 11; j<21; j++)
	{
		matrix[k1][j] = matrix[k2][j];
	}
}


void empty_shape(u8 matrix[HEIGHT][WIDTH], int i1, int i2, int i3, int i4, int j1, int j2, int j3, int j4)
{
	matrix[i1][j1] = ' ';
	matrix[i2][j2] = ' ';
	matrix[i3][j3] = ' ';
	matrix[i4][j4] = ' ';
}


void draw_shape(u8 matrix[HEIGHT][WIDTH], int i1, int i2, int i3, int i4, int j1, int j2, int j3, int j4)
{
	matrix[i1][j1] = 0x80;
	matrix[i2][j2] = 0x80;
	matrix[i3][j3] = 0x80;
	matrix[i4][j4] = 0x80;

	print_matrix(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, matrix);
}


void rotate_shapes(u8 matrix[HEIGHT][WIDTH], int shape, int *pos, int *i1, int *i2, int *i3, int *i4, int *j1, int *j2, int *j3, int *j4)
{
	int i1n = *i1;					/* Acquire the current coordinates*/
	int i2n = *i2;
	int i3n = *i3;
	int i4n = *i4;

	int j1n = *j1;
	int j2n = *j2;
	int j3n = *j3;
	int j4n = *j4;

	if(shape == 0)		/* Is the I shape being rotated? */
	{
		if(*pos == 0)		/* Check in which position it is */
		{
			if((matrix[i1n+3][j1n+1] == ' ') && (matrix[i2n+2][j2n] == ' ') && (matrix[i3n+1][j3n-1] == ' '))	/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 3;														/* Set the new coordinates */
				j1n = j1n + 1;

				i2n = i2n + 2;
				//j2n = j2n;

				i3n = i3n + 1;
				j3n = j3n - 1;

				//i4n = i4n;
				j4n = j4n - 2;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 1;															/* Set the next position */
			}
		}
		else if(*pos == 1)	/* Check in which position it is */
		{
			if((matrix[i1n-3][j1n-1] == ' ') && (matrix[i3n-1][j3n+1] == ' ') && (matrix[i4n][j4n+2] == ' '))	/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n - 3;														/* Set the new coordinates */
				j1n = j1n - 1;

				i2n = i2n - 2;
				//j2n = j2n;

				i3n = i3n - 1;
				j3n = j3n + 1;

				//i4n = i4n;
				j4n = j4n + 2;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 0;															/* Set the next position */
			}
		}
	}
	else if(shape == 1)		/* Is the T shape being rotated? */
	{
		if(*pos == 0)		/* Check in which position it is */
		{
			if(matrix[i3n-1][j3n-1] == ' ')		/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				j1n = j1n + 1;

				//i2n = i2n;
				//j2n = j2n;

				i3n = i3n - 1;
				j3n = j3n - 1;

				i4n = i4n - 1;
				j4n = j4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 1;															/* Set the next position */
			}
		}
		else if(*pos == 1)		/* Check in which position it is */
		{
			if(matrix[i3n+1][j1n-1] == ' ')			/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n - 1;														/* Set the new coordinates */
				j1n = j1n + 1;

				//i2n = i2n;
				//j2n = j2n;

				i3n = i3n + 1;
				j3n = j3n - 1;

				i4n = i4n - 1;
				j4n = j4n - 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 2;
			}
		}
		else if(*pos == 2)	/* Check in which position it is */
		{
			if(matrix[i3n+1][j1n+1] == ' ')		/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n - 1;														/* Set the new coordinates */
				j1n = j1n - 1;

				//i2n = i2n;
				//j2n = j2n;

				i3n = i3n + 1;
				j3n = j3n + 1;

				i4n = i4n + 1;
				j4n = j4n - 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 3;															/* Set the next position */
			}
		}
		else if(*pos == 3)	/* Check in which position it is */
		{
			if(matrix[i3n-1][j1n+1] == ' ')		/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				j1n = j1n - 1;

				//i2n = i2n;
				//j2n = j2n;

				i3n = i3n - 1;
				j3n = j3n + 1;

				i4n = i4n + 1;
				j4n = j4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 0;															/* Set the next position */
			}
		}
	}
	else if(shape == 2)		/* Is the L shape being rotated? */
	{
		if(*pos == 0)		/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n+1] == ' ') && (matrix[i3n-1][j3n-1] == ' ') && (matrix[i4n][j4n+2] == ' '))	/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				j1n = j1n + 1;

				//i2n = i2n;
				//j2n = j2n;

				i3n = i3n - 1;
				j3n = j3n - 1;

				//i4n = i4n;
				j4n = j4n + 2;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 1;															/* Set the next position */
			}
		}
		else if(*pos == 1)	/* Check in which position it is */
		{
			if((matrix[i1n-1][j1n+1] == ' ') && (matrix[i3n+1][j3n-1] == ' ') && (matrix[i4n-2][j4n] == ' '))	/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n - 1;														/* Set the new coordinates */
				j1n = j1n + 1;

				//i2n = i2n;
				//j2n = j2n;

				i3n = i3n + 1;
				j3n = j3n - 1;

				i4n = i4n - 2;
				//j4n = j4n;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 2;															/* Set the next position */
			}
		}
		else if(*pos == 2)	/* Check in which position it is */
		{
			if((matrix[i1n-1][j1n-1] == ' ') && (matrix[i3n+1][j3n+1] == ' ') && (matrix[i4n][j4n-2] == ' '))	/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n - 1;														/* Set the new coordinates */
				j1n = j1n - 1;

				//i2n = i2n;
				//j2n = j2n;

				i3n = i3n + 1;
				j3n = j3n + 1;

				//i4n = i4n;
				j4n = j4n - 2;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 3;															/* Set the next position */
			}
		}
		else if(*pos == 3)	/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n-1] == ' ') && (matrix[i3n-1][j3n+1] == ' ') && (matrix[i4n+2][j4n] == ' '))	/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				j1n = j1n - 1;

				//i2n = i2n;
				//j2n = j2n;

				i3n = i3n - 1;
				j3n = j3n + 1;

				i4n = i4n + 2;
				//j4n = j4n;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 0;															/* Set the next position */
			}
		}
	}
	else if(shape == 3)		/* Is the J shape being rotated? */
	{
		if(*pos == 0)		/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n+1] == ' ') && (matrix[i3n-1][j3n-1] == ' ') && (matrix[i4n-2][j4n] == ' '))	/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				j1n = j1n + 1;

				//i2n = i2n;
				//j2n = j2n;

				i3n = i3n - 1;
				j3n = j3n - 1;

				i4n = i4n - 2;
				//j4n = j4n;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 1;															/* Set the next position */
			}
		}
		else if(*pos == 1)	/* Check in which position it is */
		{
			if((matrix[i1n-1][j1n+1] == ' ') && (matrix[i3n+1][j3n-1] == ' ') && (matrix[i4n][j4n-2] == ' '))	/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n - 1;														/* Set the new coordinates */
				j1n = j1n + 1;

				//i2n = i2n;
				//j2n = j2n;

				i3n = i3n + 1;
				j3n = j3n - 1;

				//i4n = i4n;
				j4n = j4n - 2;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 2;															/* Set the next position */
			}
		}
		else if(*pos == 2)	/* Check in which position it is */
		{
			if((matrix[i1n-1][j1n-1] == ' ') && (matrix[i3n+1][j3n+1] == ' ') && (matrix[i4n+2][j4n] == ' '))	/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);

				i1n = i1n - 1;														/* Set the new coordinates */
				j1n = j1n - 1;

				//i2n = i2n;
				//j2n = j2n;

				i3n = i3n + 1;
				j3n = j3n + 1;

				i4n = i4n + 2;
				//j4n = j4n;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 3;															/* Set the next position */
			}
		}
		else if(*pos == 3)	/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n-1] == ' ') && (matrix[i3n-1][j3n+1] == ' ') && (matrix[i4n][j4n+2] == ' '))	/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				j1n = j1n - 1;

				//i2n = i2n;
				//j2n = j2n;

				i3n = i3n - 1;
				j3n = j3n + 1;

				//i4n = i4n;
				j4n = j4n + 2;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 0;															/* Set the next position */
			}
		}
	}
	else if(shape == 4)		/* Is the Z shape being rotated? */
	{
		if(*pos == 0)		/* Check in which position it is */
		{
			if((matrix[i1n+2][j1n] == ' ') && (matrix[i2n+1][j2n-1] == ' '))	/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 2;														/* Set the new coordinates */
				//j1n = j1n;

				i2n = i2n + 1;
				j2n = j2n - 1;

				//i3n = i3n;
				//j3n = j3n;

				i4n = i4n - 1;
				j4n = j4n - 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 1;															/* Set the next position */
			}
		}
		else if(*pos == 1)	/* Check in which position it is */
		{
			if((matrix[i1n-2][j1n] == ' ') && (matrix[i4n+1][j4n+1] == ' '))	/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n - 2;														/* Set the new coordinates */
				//j1n = j1n;

				i2n = i2n - 1;
				j2n = j2n + 1;

				//i3n = i3n;
				//j3n = j3n;

				i4n = i4n + 1;
				j4n = j4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 0;															/* Set the next position */
			}
		}
	}
	else if(shape == 5)		/* Is the S shape being rotated? */
	{
		if(*pos == 0)		/* Check in which position it is */
		{
			if((matrix[i2n][j2n-2] == ' ') && (matrix[i3n+1][j3n+1] == ' '))	/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				j1n = j1n - 1;

				//i2n = i2n;
				j2n = j2n - 2;

				i3n = i3n + 1;
				j3n = j3n + 1;

				//i4n = i4n;
				//j4n = j4n;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 1;															/* Set the next position */
			}
		}
		else if(*pos == 1)	/* Check in which position it is */
		{
			if((matrix[i1n-1][j1n+1] == ' ') && (matrix[i2n][j2n+2] == ' '))	/* Is there free space around the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n - 1;														/* Set the new coordinates */
				j1n = *j1 + 1;

				//i2n = i2n;
				j2n = j2n + 2;

				i3n = i3n - 1;
				j3n = j3n - 1;

				//i4n = i4n;
				//j4n = j4n;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;

				*j1 = j1n;
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;

				*pos = 0;															/* Set the next position */
			}
		}
	}
}


void move_shapes(u8 matrix[HEIGHT][WIDTH], int shape, int pos, int* draw, int* move, int *i1, int *i2, int *i3, int *i4, int *j1, int *j2, int *j3, int *j4)
{
	int i1n = *i1;					/* Acquire the current coordinates*/
	int i2n = *i2;
	int i3n = *i3;
	int i4n = *i4;

	int j1n = *j1;
	int j2n = *j2;
	int j3n = *j3;
	int j4n = *j4;

	if(shape == 0)		/* Is the I shape being moved? */
	{
		if(pos == 0)	/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n] == ' ') && (matrix[i2n+1][j2n] == ' ') && (matrix[i3n+1][j3n] == ' ') && (matrix[i4n+1][j4n] == ' '))	/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
		else if(pos == 1)	/* Check in which position it is */
		{
			if(matrix[i1n+1][j1n] == ' ')		/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
	}
	else if(shape == 1)		/* Is the T shape being moved? */
	{
		if(pos == 0)		/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n] == ' ') && (matrix[i3n+1][j3n] == ' ') && (matrix[i4n+1][j4n] == ' '))	/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
		else if(pos == 1)	/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n] == ' ') && (matrix[i4n+1][j4n] == ' '))		/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
		else if(pos == 2)	/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n] == ' ') && (matrix[i2n+1][j2n] == ' ') && (matrix[i3n+1][j3n] == ' '))	/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
		else if(pos == 3)	/* Check in which position it is */
		{
			if((matrix[i3n+1][j3n] == ' ') && (matrix[i4n+1][j4n] == ' '))		/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
	}
	else if(shape == 2)		/* Is the L shape being moved? */
	{
		if(pos == 0)		/* Check in which position it is */
		{
			if((matrix[i2n+1][j2n] == ' ') && (matrix[i3n+1][j3n] == ' ') && (matrix[i4n+1][j4n] == ' '))	/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
		else if(pos == 1)	/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n] == ' ') && (matrix[i4n+1][j4n] == ' '))		/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
		else if(pos == 2)	/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n] == ' ') && (matrix[i2n+1][j2n] == ' ') && (matrix[i3n+1][j3n] == ' '))	/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
		else if(pos == 3)	/* Check in which position it is */
		{
			if((matrix[i3n+1][j3n] == ' ') && (matrix[i4n+1][j4n] == ' '))		/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
	}
	else if(shape == 3)		/* Is the J shape being moved? */
	{
		if(pos == 0)		/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n] == ' ') && (matrix[i2n+1][j2n] == ' ') && (matrix[i4n+1][j4n] == ' '))	/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
		else if(pos == 1)	/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n] == ' ') && (matrix[i4n+1][j4n] == ' '))		/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
		else if(pos == 2)	/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n] == ' ') && (matrix[i2n+1][j2n] == ' ') && (matrix[i3n+1][j3n] == ' '))	/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
		else if(pos == 3)	/* Check in which position it is */
		{
			if((matrix[i3n+1][j3n] == ' ') && (matrix[i4n+1][j4n] == ' '))		/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
	}
	else if(shape == 4) 	/* Is the Z shape being moved? */
	{
		if(pos == 0)		/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n] == ' ') && (matrix[i3n+1][j3n] == ' ') && (matrix[i4n+1][j4n] == ' '))	/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
		else if(pos == 1)	/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n] == ' ') && (matrix[i3n+1][j3n] == ' '))		/* Is there free space below the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
	}
	else if(shape == 5)		/* Is the S shape being moved? */
	{
		if(pos == 0)		/* Check in which position it is */
		{
			if((matrix[i2n+1][j2n] == ' ') && (matrix[i3n+1][j3n] == ' ') && (matrix[i4n+1][j4n] == ' '))	// Is there free space below the shape?
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
		else if(pos == 1)	/* Check in which position it is */
		{
			if((matrix[i1n+1][j1n] == ' ') && (matrix[i3n+1][j3n] == ' '))	// Is there free space below the shape?
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				i1n = i1n + 1;														/* Set the new coordinates */
				i2n = i2n + 1;
				i3n = i3n + 1;
				i4n = i4n + 1;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*i1 = i1n;															/* Update the current coordinates */
				*i2 = i2n;
				*i3 = i3n;
				*i4 = i4n;
			}
			else
			{
				*move = 0;															/* Set the enables */
				*draw = 0;
			}
		}
	}
	else if(shape == 6)		/* Is the O shape being moved? */
	{
		if((matrix[i3n+1][j3n] == ' ') && (matrix[i4n+1][j4n] == ' '))		/* Is there free space below the shape? */
		{
			empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);			/* Empty the current shape position */

			i1n = i1n + 1;															/* Set the new coordinates */
			i2n = i2n + 1;
			i3n = i3n + 1;
			i4n = i4n + 1;

			draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);				/* Draw the shape in the next position */

			*i1 = i1n;																/* Update the current coordinates */
			*i2 = i2n;
			*i3 = i3n;
			*i4 = i4n;
		}
		else
		{
			*move = 0;																/* Set the enables */
			*draw = 0;
		}
	}
}


void move_shapes_sideways(u8 matrix[HEIGHT][WIDTH], int shape, int pos, int *i1, int *i2, int *i3, int *i4, int *j1, int *j2, int *j3, int *j4, int side)
{
	int i1n = *i1;					/* Acquire the current coordinates*/
	int i2n = *i2;
	int i3n = *i3;
	int i4n = *i4;

	int j1n = *j1;
	int j2n = *j2;
	int j3n = *j3;
	int j4n = *j4;


	if(shape == 0)			/* Is the I shape being moved? */
	{
		if(pos == 0)		/* Check in which position it is */
		{
			if((matrix[i4n][j4n+side] == ' ') || (matrix[i1n][j1n+side] == ' '))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
		else if(pos == 1)	/* Check in which position it is */
		{
			if(((matrix[i1n][j1n+side] == ' ') && (matrix[i2n][j1n+side] == ' ') &&
					(matrix[i3n][j1n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
	}
	else if(shape == 1)		/* Is the T shape being moved? */
	{
		if(pos == 0)		/* Check in which position it is */
		{
			if(((matrix[i3n][j3n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')) ||
					((matrix[i1n][j1n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
		else if(pos == 1)	/* Check in which position it is */
		{
			if(((matrix[i1n][j1n+side] == ' ') && (matrix[i3n][j3n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')) ||
					((matrix[i1n][j1n+side] == ' ') && (matrix[i2n][j2n+side] == ' ') && (matrix[i3n][j3n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
		else if(pos == 2)	/* Check in which position it is */
		{
			if(((matrix[i1n][j1n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')) ||
					((matrix[i3n][j3n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
		else if(pos == 3)	/* Check in which position it is */
		{
			if(((matrix[i1n][j1n+side] == ' ') && (matrix[i2n][j2n+side] == ' ') && (matrix[i3n][j3n+side] == ' ')) ||
					((matrix[i1n][j1n+side] == ' ') && (matrix[i3n][j3n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}

	}
	else if(shape == 2)		/* Is the L shape being moved? */
	{
		if(pos == 0)		/* Check in which position it is */
		{
			if(((matrix[i3n][j3n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')) ||
					((matrix[i1n][j1n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
		else if(pos == 1)	/* Check in which position it is */
		{
			if(((matrix[i2n][j2n+side] == ' ') && (matrix[i3n][j3n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')) ||
					((matrix[i1n][j1n+side] == ' ') && (matrix[i2n][j2n+side] == ' ') && (matrix[i3n][j3n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
		else if(pos == 2)	/* Check in which position it is */
		{
			if(((matrix[i1n][j1n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')) ||
					((matrix[i3n][j3n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
		else if(pos == 3)	/* Check in which position it is */
		{
			if(((matrix[i1n][j1n+side] == ' ') && (matrix[i2n][j2n+side] == ' ') && (matrix[i3n][j3n+side] == ' ')) ||
					((matrix[i2n][j2n+side] == ' ') && (matrix[i3n][j3n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;


				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
	}
	else if(shape == 3)		/* Is the J shape being moved? */
	{
		if(pos == 0)		/* Check in which position it is */
		{
			if(((matrix[i3n][j3n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')) ||
					((matrix[i1n][j1n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
		else if(pos == 1)	/* Check in which position it is */
		{
			if(((matrix[i1n][j1n+side] == ' ') && (matrix[i2n][j2n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')) ||
					((matrix[i1n][j1n+side] == ' ') && (matrix[i2n][j2n+side] == ' ') && (matrix[i3n][j3n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
		else if(pos == 2)	/* Check in which position it is */
		{
			if(((matrix[i1n][j1n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')) ||
					((matrix[i3n][j3n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
		else if(pos == 3)	/* Check in which position it is */
		{
			if(((matrix[i1n][j1n+side] == ' ') && (matrix[i2n][j2n+side] == ' ') && (matrix[i3n][j3n+side] == ' ')) ||
					((matrix[i1n][j1n+side] == ' ') && (matrix[i2n][j2n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
	}
	else if(shape == 4) 	/* Is the Z shape being moved? */
	{
		if(pos == 0)		/* Check in which position it is */
		{
			if(((matrix[i2n][j2n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')) ||
					((matrix[i1n][j1n+side] == ' ') && (matrix[i3n][j3n+side] == ' ')))	  /* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
		else if(pos == 1)	/* Check in which position it is */
		{
			if(((matrix[i1n][j1n+side] == ' ') && (matrix[i3n][j3n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')) ||
					((matrix[i1n][j1n+side] == ' ') && (matrix[i2n][j2n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}

	}
	else if(shape == 5)		/* Is the S shape being moved? */
	{
		if(pos == 0)		/* Check in which position it is */
		{
			if(((matrix[i2n][j2n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')) ||
					((matrix[i1n][j1n+side] == ' ') && (matrix[i3n][j3n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
		else if(pos == 1)	/* Check in which position it is */
		{
			if(((matrix[i2n][j2n+side] == ' ') && (matrix[i3n][j3n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')) ||
					((matrix[i1n][j1n+side] == ' ') && (matrix[i2n][j2n+side] == ' ') && (matrix[i3n][j3n+side] == ' ')))	/* Is there free space right or left of the shape? */
			{
				empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

				j1n = j1n + side;													/* Set the new coordinates */
				j2n = j2n + side;
				j3n = j3n + side;
				j4n = j4n + side;

				draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

				*j1 = j1n;															/* Update the current coordinates */
				*j2 = j2n;
				*j3 = j3n;
				*j4 = j4n;
			}
		}
	}
	else if(shape == 6)		/* Is the O shape being moved? */
	{
		if(((matrix[i2n][j2n+side] == ' ') && (matrix[i4n][j4n+side] == ' ')) ||
				((matrix[i1n][j1n+side] == ' ') && (matrix[i3n][j3n+side] == ' ')))	/* Is there free space right or left of the shape? */
		{
			empty_shape(matrix, i1n, i2n,  i3n, i4n, j1n, j2n, j3n, j4n);		/* Empty the current shape position */

			j1n = j1n + side;													/* Set the new coordinates */
			j2n = j2n + side;
			j3n = j3n + side;
			j4n = j4n + side;

			draw_shape(matrix, i1n, i2n, i3n, i4n, j1n, j2n, j3n, j4n);			/* Draw the shape in the next position */

			*j1 = j1n;															/* Update the current coordinates */
			*j2 = j2n;
			*j3 = j3n;
			*j4 = j4n;
		}
	}
}


void check_score(u8 matrix[HEIGHT][WIDTH], int *score)
{
	int i;
	int k;

	for(i = 29; i > -1; i--)
	{
		while((matrix[i][11] == 0x80) && (matrix[i][12] == 0x80) && (matrix[i][13] == 0x80) &&
			  (matrix[i][14] == 0x80) && (matrix[i][15] == 0x80) && (matrix[i][16] == 0x80) &&
			  (matrix[i][17] == 0x80) && (matrix[i][18] == 0x80) && (matrix[i][19] == 0x80) && (matrix[i][20] == 0x80))		/* While there is a full row, empty it and move the rest above it down*/
		{
			*score += 1;

			empty_row(matrix, i);				/* Empty the current row */

			for(k = i; k > 0; k--)
			{
				move_row(matrix, k, k-1);		/* Move the above row */

				empty_row(matrix, k-1);			/* Empty the above row */
			}
			print_score1(matrix, *score);		/* Update the score */
			print_score2(matrix, *score);
		}

	}
}


void print_score1(u8 matrix[HEIGHT][WIDTH], int score)
{
	char n0 = '0';
	char n1 = '1';
	char n2 = '2';
	char n3 = '3';
	char n4 = '4';
	char n5 = '5';
	char n6 = '6';
	char n7 = '7';
	char n8 = '8';
	char n9 = '9';


	if(score == 0)
	{
		matrix[2][2] = n0;
	}
	else if(score == 10)
	{
		matrix[2][2] = n1;
	}
	else if(score == 20)
	{
		matrix[2][2] = n2;
	}
	else if(score == 30)
	{
		matrix[2][2] = n3;
	}
	else if(score == 40)
	{
		matrix[2][2] = n4;
	}

	else if(score == 50)
	{
		matrix[2][3] = n5;
	}
	else if(score == 60)
	{
		matrix[2][3] = n6;
	}
	else if(score == 70)
	{
		matrix[2][3] = n7;
	}
	else if(score == 80)
	{
		matrix[2][3] = n8;
	}
	else if(score == 90)
	{
		matrix[2][3] = n9;
	}
}


void print_score2(u8 matrix[HEIGHT][WIDTH], int score)
{
	char n0 = '0';
	char n1 = '1';
	char n2 = '2';
	char n3 = '3';
	char n4 = '4';
	char n5 = '5';
	char n6 = '6';
	char n7 = '7';
	char n8 = '8';
	char n9 = '9';


	if((score % 10) == 0)
	{
		matrix[2][3] = n0;
	}
	else if((score % 10) == 1)
	{
		matrix[2][3] = n1;
	}
	else if((score % 10) == 2)
	{
		matrix[2][3] = n2;
	}
	else if((score % 10) == 3)
	{
		matrix[2][3] = n3;
	}
	else if((score % 10) == 4)
	{
		matrix[2][3] = n4;
	}
	else if((score % 10) == 5)
	{
		matrix[2][3] = n5;
	}
	else if((score % 10) == 6)
	{
		matrix[2][3] = n6;
	}
	else if((score % 10) == 7)
	{
		matrix[2][3] = n7;
	}
	else if((score % 10) == 8)
	{
		matrix[2][3] = n8;
	}
	else if((score % 10) == 9)
	{
		matrix[2][3] = n9;
	}
}


void wait()
{
	int i;
	for (i = 0; i < 6000000; i++)  /* 6mil */
	{
	}
}


void waitSlower()
{
	int i;
	for (i = 0; i < 3000000; i++)  /* 3mil */
	{
	}
}


void waitSlower2()
{
	int i;
	for (i = 0; i < 600000; i++)  /* 600thsnd */
	{
	}
}

int main()
{
	init_platform();

	int x;
	int key;
	int i1, i2, i3, i4;
	int j1, j2, j3, j4;

	int shape = 0;
	int draw_en = 0;
	int move_en = 0;
	int side = 0;
	int score = 0;
	int pos = 0;
	int go = 0;


	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x00, 0x0);			/* direct mode      0 */
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x04, 0x3);			/* display_mode     1 */
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x08, 0x0);			/* show frame       2 */
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x0C, 0x1);			/* font size        3 */
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x10, 0xFFFFFF);	/* foreground       4 */
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x14, 0x000000);	/* background color 5 */
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x18, 0xFF0000);	/* frame color      6 */


	clear_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);									/* Clear the screen */
	set_background_color(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 0x000000);					/* Set the background color*/
	set_foreground_color(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 0xFFFFFF);					/* Set the foreground color*/


	empty_matrix(matrix);			/* Empty the matrix */

	wait();

	print_start_string(matrix);		/* Print the start up string */

	wait();
	wait();

	load_matrix(matrix);			/* Setup the matrix */

	wait();

	while(1)
	{
		x = Xil_In8(XPAR_MY_PERIPHERAL_0_BASEADDR);		/* Read what button has been pressed */
		key = x & 0b00011111;

		go = check_game_over(matrix);					/* Check if the game is over */

		if(go == 0)
		{
			if(move_en == 0)							 /* Check the score when the current shape has stopped moving */
				check_score(matrix, &score);

			if(draw_en == 0)									/* If there is no shape being moved draw a new one */
			{
				rand_shapes(&shape, &pos, &i1, &i2, &i3, &i4, &j1, &j2, &j3, &j4);		  /* Pick the next shape */

				draw_shape(matrix, i1, i2, i3, i4, j1, j2, j3, j4);						/* Draw the new shape */

				draw_en = 1;
				move_en = 1;

				wait();
			}

			if(move_en == 1)		/* Move the current shape */
			{
				move_shapes(matrix, shape, pos, &move_en, &draw_en, &i1, &i2, &i3, &i4, &j1, &j2, &j3, &j4);
				waitSlower();
			}

			if (key == 30) 			/* New game - reset */
			{
				print_new_game(matrix, &score);
				draw_en = 0;
				move_en = 0;
			}
			else if(key == 27)		/* Lower the shape faster */
			{
				if(move_en == 1)
				{
					move_shapes(matrix, shape, pos, &move_en, &draw_en, &i1, &i2, &i3, &i4, &j1, &j2, &j3, &j4);
				}
			}
			else if(key == 23)		/* Move the shape to the right */
			{
				side = 1;
				move_shapes_sideways(matrix, shape, pos, &i1, &i2, &i3, &i4, &j1, &j2, &j3, &j4, side);
			}
			else if(key == 29)		/* Move the shape to the left */
			{
				side = -1;
				move_shapes_sideways(matrix, shape, pos, &i1, &i2, &i3, &i4, &j1, &j2, &j3, &j4, side);
			}
			else if(key == 15)		/* Rotate the shape */
			{
				rotate_shapes(matrix, shape, &pos, &i1, &i2, &i3, &i4, &j1, &j2, &j3, &j4);
			}
		}
		else
		{
			print_game_over(matrix); 	/* Print the game over string and turn off enables */
			draw_en = 1;
			move_en = 0;
		}
	}

    return 0;
}
