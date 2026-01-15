#include "../print.h"
#include "../Trans.h"
#include  <ctoserr.h>
#include  <string.h>
#include  <stdio.h>
#include "DISPLAY/Display.h"
#define _PROCESS_           "./fs_data/PROCESS.bmp"
//#define _COORDINATE_Y_LINE_8_7_  (_LCD_YSIZE_ / 8) * 6
//#define _LCD_YSIZE_   480


int inPRINT_TTF_SetFont_Style(int);
int inPRINT_Buffer_Initial(unsigned char*, int , FONT_ATTRIB*, BufferHandle*);
int inPRINT_Buffer_PutGraphic(unsigned char *, unsigned char* , BufferHandle*, int , int );
int inPRINT_Buffer_PutIn(char* , int , unsigned char*, FONT_ATTRIB*, BufferHandle*, int , unsigned char);
int inPRINT_Buffer_OutPut(unsigned char *, BufferHandle *);