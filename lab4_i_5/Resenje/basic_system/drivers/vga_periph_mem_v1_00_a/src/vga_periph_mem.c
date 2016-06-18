/*****************************************************************************
* Filename:          D:\work\lprs2\2013_2014\Resene_vezbe\lab56\basic_system/drivers/vga_periph_mem_v1_00_a/src/vga_periph_mem.c
* Version:           1.00.a
* Description:       vga_periph_mem Driver Source File
* Date:              Wed Mar 05 10:25:21 2014 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#define GRAPHICS_MEM_OFF 0x2000000
#define TEXT_MEM_OFF 0x1000000
#define VGA_PERIPH_MEM_mWriteMemory(Address, Data) \
 	Xil_Out32(Address, (Xuint32)(Data))


/***************************** Include Files *******************************/

#include "vga_periph_mem.h"
Xuint32 cursor_position;
/************************** Function Definitions ***************************/

void set_cursor(Xuint32 new_value){
	cursor_position = new_value;
}

void clear_text_screen(Xuint32 BaseAddress){
	int i;
	for (i = 0; i < 4800; i++){
		VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + i*4, 0x20);
	}
}

void print_string(Xuint32 BaseAddress, unsigned char string_s[], int lenght){
	int i;
	for (i = 0; i < lenght; i++){
		VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + cursor_position + i*4, (string_s[i]-0x40));
	}
}

void clear_graphics_screen(Xuint32 BaseAddress){
	int i;
	for (i = 0; i < 9600; i++){
	    VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0x0);
	}
}

void draw_square(Xuint32 BaseAddress){
	int i, j, k;
		for (j = 0; j < 480; j++){
			for (k = 0; k<(640/32); k++){
				i = j*(640/32) + k;
				if ((j > 200) && (j < 280) && (k > 8) && (k < 12)) {
					VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + 4, 0xFFFFFFFF);
				}
				else{
					VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + 4, 0x00000000);
				}
			}
		}
}

void clear_screen(Xuint32 BaseAddress)
{
	clear_text_screen(BaseAddress);
	clear_graphics_screen(BaseAddress);
}

void print_char(Xuint32 BaseAddress)
{
	char str[] = { 'N' };
	print_string(BaseAddress, str, 1);
}

void set_foreground_color(Xuint32 BaseAddress, int color)
{
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + 0x10, color);
}

void set_background_color(Xuint32 BaseAddress, int color)
{
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + 0x14, color);
}

void set_font_size(Xuint32 BaseAddress, int size)
{
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + 0x0C, size);
}

void draw_circle(Xuint32 BaseAddress)
{
	int i, j, k;
	for (j = 0; j < 480; j++)
	{
		for (k = 0; k<20; k++)
		{
			/*i = j*(640/32) + k;
			if ((j > 200) && (j < 280) && (k > 8) && (k < 12))
			{
				VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0xFFFFFFFF);
			}
			else
			{
				VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0x0);
			}*/
			i = j*(640/32) + k;
			if ((j > 200) && (j<328) && (k > 8) && (k < 12))
				VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0xFFFFFFFF);
		}
	}
}

void get_resolution(Xuint32 BaseAddress, Xuint32 *w, Xuint32 *h)
{
	*w = 640;
	*h = 480;
}

void print_side_string(Xuint32 BaseAddress, int cursor_position)
{
	int i;
	char s = 'I';

	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + cursor_position,(s /*- 0x40*/));
}

void print_score_string(Xuint32 BaseAddress, int cursor_position)
{
	int i;
	char s = 'S';
	char c = 'C';
	char o = 'O';
	char r = 'R';
	char e = 'E';

	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + cursor_position, s);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + (cursor_position + 4), (c));
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + (cursor_position + 8), (o));
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + (cursor_position + 12), (r));
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + (cursor_position + 16), (e));

}


void print_NUMSCORE_string(Xuint32 BaseAddress,int *CheckScore,int *Deca)
{
	int score = (*CheckScore);
	int deca = (*Deca);

	if(score > 9)
	{
		deca++;
		score = 0;
		*CheckScore = score;
		*Deca = deca;
		VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 650, (48+deca));
	}
	else
		VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 650, (48+deca));

	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 654, (48+score));
}

void print_matrix(Xuint32 BaseAddress, u8* matrix)
{
	int i;
	
	for(i = 0; i < 30*40; i++){
		VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + i*4, matrix[i]);
	}
	
}

void print_tetris_string(Xuint32 BaseAddress)
{
	char e = 'E';
	char t = 'T';
	char r = 'R';
	char i = 'I';
	char s = 'S';
	char k = 'K';

	char z1 = '(';
	char z2 = ')';
	char c = 'C';
	char line = '-';
	char n1 = '2';
	char n2 = '0';
	char n3 = '1';
	char n4 = '6';


	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 105, z1);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 109, c);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 113, z2);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 121, r);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 125, t);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 129, line);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 133, r);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 137, k);


	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 117 + 320, n1);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 121 + 320, n2);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 125 + 320, n3);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 129 + 320, n4);


	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + (2178 + 320), t);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + (2178 + 320 + 8), e);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + (2178 + 320 + 16), t);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + (2178 + 320 + 24), r);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + (2178 + 320 + 32), i);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + (2178 + 320 + 40), s);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + (2178 + 320 + 48), n1);

}


void print_gameover_string(Xuint32 BaseAddress)
{
	char g = 'G';
	char a = 'A';
	char m = 'M';
	char e = 'E';
	char o = 'O';
	char v = 'V';
	char r = 'R';


	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 2275, g);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 2275 + 16, a);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 2275 + 32, m);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 2275 + 48, e);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 2275 + 320 + 48, o);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 2275 + 320 + 64, v);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 2275 + 320 + 80, e);
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + 2275 + 320 + 96, r);



}