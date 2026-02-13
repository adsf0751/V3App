/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   print.h
 * Author: 0092020
 *
 * Created on 2025年12月24日, 上午 11:26
 */

#ifndef PRINT_H
#define PRINT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ctosapi.h>

typedef enum
{
	_BAUL_FONT_STYLE_UNCHANGE_ = 0,	/* 代表字體不做更改 */
	_BAUL_FONT_STYLE_REGULAR_,
	_BAUL_FONT_STYLE_ITALIC_,
	_BAUL_FONT_STYLE_BOLD_,
	_BAUL_FONT_STYLE_REVERSE_,
	_BAUL_FONT_STYLE_UNDERLINE_,
}BAUL_FONT;

#define FONT_ATTRIB			CTOS_FONT_ATTRIB

/* Print Options */
#define _PRT_CUST_			0
#define _PRT_MERCH_			1
#define _PRT_MERCH_DUPLICATE_		2


#define	_MAX_Y_LENGTH_			32
#define _BUFFER_MAX_LINE_		800	/* 自行定義足夠大的Buffer */


#define _PRT_DOUBLE_HEIGHT_WIDTH_       d_FONT_16x30


/* Print buffer 使用 START */
#define _CURRENT_LINE_			0
#define _LAST_ENTRY_			1
/* inCover */
#define _COVER_				1	/* 用於以圖做的帳單，handle不下移 */
#define _APPEND_			2	/* 印完圖片，接著印下面*/

#define	_MAX_Y_LENGTH_			32
#define _BUFFER_MAX_LINE_		800	/* 自行定義足夠大的Buffer */

/* Aligned */
#define _PRINT_LEFT_			d_PRINTER_ALIGNLEFT
#define _PRINT_CENTER_			d_PRINTER_ALIGNCENTER
#define _PRINT_RIGHT_			d_PRINTER_ALIGNRIGHT
#define _PRINT_BY_X_			127


#define _PRT_HEIGHT_                    d_FONT_12x24


/* 定義字型Style */
#define _FONT_PRINT_REGULAR_		d_FONT_STYLE_NORMAL
#define _FONT_PRINT_ITALIC_		d_FONT_STYLE_ITALIC
#define _FONT_PRINT_BOLD_		d_FONT_STYLE_BOLD
#define _FONT_PRINT_REVERSE_		d_FONT_STYLE_REVERSE
#define _FONT_PRINT_UNDERLINE_		d_FONT_STYLE_UNDERLINE

#define _PRT_NORMAL_            d_FONT_8x16
#define d_FONT_8x16             0x0810

typedef struct					
{
	int	inXbound;	/* 用來紀錄buffer的寬度，inXcurrent不應超過它 */
	int	inYbound;	/* 用來紀錄buffer的高度，inXcurrent不應超過它 */
	int	inXcurrent;	/* handle目前的X位置 */
	int	inYcurrent;	/* handle目前的Y位置 */
	int	inYcurrentMAX;	/* 紀錄當前這一行，最下面的位置(當同一行有印大字小字，用來表示底線在哪) */
	int	inYcover;	//如果有底圖，則要與inYcurrent比較那一個比較下面
	unsigned char	uszDetailPrint;	/* 列印明細顯示畫面用 避免多筆誤以為當機 */
}BufferHandle;



/* 用來控制PutIn在同一底線上，所以先把該行要Putin的字先存起來 */
typedef struct					
{	BufferHandle		srBhandle;
	char			szString[300];		/* 暫放的字句 (因優惠兌換資訊，所以上調到300) */
	FONT_ATTRIB		srFont_Attrib;		/* 字型資訊 */
	unsigned char		uszPrintPosition;	/* 靠左或靠右 */
	int			intXPosition;		/* X軸位置 */
	int			inFontStyle;		/* 字體字型 */
}BufferArrangeUnderLine;    
    




#ifdef __cplusplus
}
#endif

#endif /* PRINT_H */

