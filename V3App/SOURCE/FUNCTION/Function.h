#ifndef FUNCTION_H
#define FUNCTION_H

#ifdef __cplusplus
extern "C" {
#endif
#include "../../PrtMsg.h"
#include "../../Trans.h" // FOR VS_ERROR/VS_SUCCESS
#include <stdio.h> // FOR NULL
#include <string.h>
#include <stdlib.h>
#include "../CREDIT/CreditptrByBuffer.h"
#include "../EVENT/MenuMsg.h"
#include "../DISPLAY/DisTouch.h"
#include "../INCLUDE/Define_1.h"
#define _PADDING_RIGHT_         0  /* 靠左右補字元 */
#define _PADDING_LEFT_          1  /* 靠右左補字元 */
#define _SIGNED_NONE_		0
#define	_SIGNED_MINUS_		1
typedef struct
{
	unsigned char    uszSecond;
	unsigned char    uszMinute;
	unsigned char    uszHour;
	unsigned char    uszDay;
	unsigned char    uszMonth;
	unsigned char    uszYear;
	unsigned char    uszDoW;	/* Day of Week （星期幾）*/
} RTC_NEXSYS;
int inFunc_ASCII_to_BCD(unsigned char *, char *, int );
int inFunc_BCD_to_ASCII(char *, unsigned char *, int );
int inFunc_ShellCommand_System(char *);
int inFunc_ls(char* , char* );
int inFunc_PAD_ASCII(char *, char *, char , int , int );
int inFunc_Amount_Comma(char *szAmt, char *szCurSymbol, char szPad_char, int inSigned, int inWide, int inAlign);
int inFunc_GetMenuNum_NewUI(TRANSACTION_OBJECT *pobTran,char menuText[][49],int menuCount,int* inKey);
void vdFunc_Display_Ethernet_Status(void);
int inETHERNET_IsPhysicalOnine(void);
int inFunc_Sync_BRec_Date_Time(TRANSACTION_OBJECT *pobTran, RTC_NEXSYS *srRTC);
int inFunc_SetEDCDateTime(char *szDate, char *szTime);
int inFunc_GetSystemDateAndTime(RTC_NEXSYS *srRTC);
#ifdef __cplusplus
}
#endif

#endif 