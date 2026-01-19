#include "Print.h"

int ginBAUL_Index = 0;
BufferArrangeUnderLine gsrBAUL[5];
int inPRINT_Buffer_Initial(unsigned char *uszBuffer, int inYLength, FONT_ATTRIB *srFont_Attrib, BufferHandle *srBhandle) {
    char szDebugMsg[100 + 1];
    unsigned short usRetVal = 0x00;

    memset(uszBuffer, 0x00, PB_CANVAS_X_SIZE * 8 * inYLength);
    memset(srFont_Attrib, 0x00, sizeof (FONT_ATTRIB));
    memset(srBhandle, 0x00, sizeof (BufferHandle));

    srFont_Attrib->X_Zoom = 1; /* 1, it means normal size, and 2 means double size. 0 means print nothing . */
    srFont_Attrib->Y_Zoom = 1; /* 1, it means normal size, and 2 means double size. 0 means print nothing . */
    srFont_Attrib->X_Space = 0; /* The space in dot to insert between each character in x coordinate. */
    srFont_Attrib->Y_Space = 0; /* The space in dot to insert between each character in y coordinate. */
    srFont_Attrib->FontSize = _PRT_HEIGHT_;

    srBhandle->inXcurrent = 0;
    srBhandle->inYcurrent = 0;
    srBhandle->inYcurrentMAX = 0;
    srBhandle->inYcover = 0;
    srBhandle->inXbound = PB_CANVAS_X_SIZE; /* 紙張最寬到哪裡 */
    srBhandle->inYbound = 8 * inYLength; /* buffer最底部宣告到哪裡 */

    /* 這隻是DefaultBuffer用的Initial，QRcode、Barcode會用到，一定要call */
    CTOS_PrinterBufferEnable();

    /* 這隻是字串用的Initial */
    usRetVal = CTOS_PrinterBufferInit(uszBuffer, srBhandle->inYbound);

    if (usRetVal != d_OK) {
        return (VS_ERROR);
    }

    CTOS_PrinterBufferSelectActiveAddress(uszBuffer);
    return (VS_SUCCESS);
}
/*
Function	:inPRINT_TTF_SetFont_Style
Date&Time	:2022/4/20 上午 11:11
Describe	:語言選擇列印的TTF
 */
int inPRINT_TTF_SetFont_Style(int inFontStyle) {
    char szTemplate[44 + 1];
    unsigned short usRetVal = 0;

    usRetVal = CTOS_FontTTFSelectStyle(d_FONT_DEVICE_PRINTER, inFontStyle);

    if (usRetVal != d_OK) {
        return (VS_ERROR);
    }
    return (VS_SUCCESS);
}
int inPRINT_Buffer_PutGraphic(unsigned char *uszFilename, unsigned char* uszBuffer1, BufferHandle *srBhandle, int inGraphicYLength, int inCover) {
    int inRetVal;
    char szDebugMsg[100 + 1];
    unsigned short usReturnValue;
    /* 輸入高度不合法 */
    if (inGraphicYLength <= 0) {
        return (-66);
    }
    if ((srBhandle->inYcurrent + inGraphicYLength) > srBhandle->inYbound) {
        do {
            inRetVal = inPRINT_Buffer_OutPut(uszBuffer1, srBhandle);
        } while (inRetVal != VS_SUCCESS);

        memset(uszBuffer1, 0x00, srBhandle->inXbound * srBhandle->inYbound);

        srBhandle->inXcurrent = 0;
        srBhandle->inYcurrent = 0;
    }
    /* 圖單獨印，所以X位置直接從0 */
    usReturnValue = CTOS_PrinterBufferBMPPic((unsigned char*) uszBuffer1, 0, srBhandle->inYcurrent, uszFilename);

    if (usReturnValue == d_OK) {
        //printf("d_OK\n");
        srBhandle->inXcurrent = 0;
        /* 2016/3/21 下午 12:05加入的feature，為了做出以圖片為模板的帳單 */
        if (inCover == _COVER_) {
            /* 當底圖handle不下移，但紀錄最低印到哪 */
            srBhandle->inYcover += srBhandle->inYcurrent + inGraphicYLength;
        } else {
            /* APPEND handle下移 */
            srBhandle->inYcurrent += inGraphicYLength;
        }
        return (VS_SUCCESS);
    } else {
        printf("%s got usReturnValue is 0x%Xh \n", uszFilename, usReturnValue);
        return (usReturnValue);
    }
}
int inPRINT_Buffer_OutPut(unsigned char *uszBuffer, BufferHandle *srBhandle) {
    char szTemplate[44 + 1];
    unsigned short usReturnValue;
   
    /* Q:為什麼要除以8 A:列印的高度以8為單位 Ex:12x24的字需用3行來印(24 = 8 * 3) 12x36需用5行來印(36 < 40 = 8 * 5 )*/
    if (srBhandle->inYcurrent >= srBhandle->inYcover)
        usReturnValue = CTOS_PrinterBufferOutput(uszBuffer, ((srBhandle->inYcurrent) / 8 + 1));
    else
        usReturnValue = CTOS_PrinterBufferOutput(uszBuffer, ((srBhandle->inYcover) / 8) + 1);
    return usReturnValue;
    if (usReturnValue == d_OK) {
        memset(uszBuffer, 0x00, srBhandle->inXbound * srBhandle->inYbound);
        srBhandle->inXcurrent = 0;
        srBhandle->inYcurrent = 0;
        srBhandle->inYcurrentMAX = 0;

        return (VS_SUCCESS);
    } else {
        if (usReturnValue == d_PRINTER_PAPER_OUT) {
            printf("printer paper out\n");
            return (VS_PRINTER_PAPER_OUT);
        } else if (usReturnValue == d_PRINTER_HEAD_OVERHEAT) {
            memset(szTemplate, 0x00, sizeof(szTemplate));	
            if (usReturnValue == d_PRINTER_HEAD_OVERHEAT)
            {
                sprintf(szTemplate, "印表機頭過熱");	/* 錯誤代碼 */
            }
            else if (usReturnValue == d_PRINTER_MOTOR_OVERHEAT)
            {
                sprintf(szTemplate, "印表機馬達過熱");	/* 錯誤代碼 */
            }

            printf("%s\n",szTemplate);
            return (VS_PRINTER_OVER_HEAT);
        } else {
            memset(szTemplate, 0x00, sizeof(szTemplate));	
            sprintf(szTemplate, "代碼：0x%04X", usReturnValue);	/* 錯誤代碼 */
            printf("%s\n",szTemplate);
            return (VS_ERROR);
        }
    }
}
int inPRINT_Buffer_Sync_UnderLine(char* szString, unsigned char *uszBuffer, FONT_ATTRIB *srFont_Attrib, BufferHandle *srBhandle, int inNextLine, unsigned char uszPrintPosition, int intXPosition) {
    int i;
    int inMaxYLength = 0; /* 所有中最大的高度 */
    int inYcurrentMAX_Old; /* 印失敗要回復原該行底線 */
    int inFontYLength; /* 字型高度 */
    char szDebugMsg[100 + 1]; /* DebugMsg */
    unsigned short usReturnValue; /* ReturnValue */

    memset(&gsrBAUL[ginBAUL_Index].srBhandle, 0x00, sizeof (BufferHandle));
    memcpy(&gsrBAUL[ginBAUL_Index].srBhandle, srBhandle, sizeof (BufferHandle));

    memset(&gsrBAUL[ginBAUL_Index].szString, 0x00, sizeof (szString));
    memcpy(&gsrBAUL[ginBAUL_Index].szString, szString, strlen(szString));

    memset(&gsrBAUL[ginBAUL_Index].srFont_Attrib, 0x00, sizeof (FONT_ATTRIB));
    memcpy(&gsrBAUL[ginBAUL_Index].srFont_Attrib, srFont_Attrib, sizeof (FONT_ATTRIB));

    if (uszPrintPosition != _PRINT_BY_X_) {
        /* Aligned */
        gsrBAUL[ginBAUL_Index].uszPrintPosition = uszPrintPosition;
    } else {
        /* By x位置 */
        gsrBAUL[ginBAUL_Index].uszPrintPosition = uszPrintPosition;
        gsrBAUL[ginBAUL_Index].intXPosition = intXPosition;
    }
    ginBAUL_Index++;

    if (inNextLine == _CURRENT_LINE_) {
    }/* Last Entry */
    else {
        /* Find Max Y length */
        for (i = 0; i < ginBAUL_Index; i++) {
            inFontYLength = gsrBAUL[i].srFont_Attrib.FontSize % 0x0100;
            if (inMaxYLength < inFontYLength)
                inMaxYLength = inFontYLength;
        }

        /* 紀錄當前行底線 */
        /* 萬一失敗拿來還原用 */
        inYcurrentMAX_Old = srBhandle->inYcurrentMAX;
        /* 該行底線設為該行字型 */
        srBhandle->inYcurrentMAX = srBhandle->inYcurrent + inMaxYLength;

        for (i = 0; i < ginBAUL_Index; i++) {
            inFontYLength = gsrBAUL[i].srFont_Attrib.FontSize % 0x0100;

            /* 改變字體形式用 */
            if (gsrBAUL[i].inFontStyle != _BAUL_FONT_STYLE_UNCHANGE_) {
                if (gsrBAUL[i].inFontStyle == _BAUL_FONT_STYLE_REGULAR_) {
                    inPRINT_TTF_SetFont_Style(_FONT_PRINT_REGULAR_);
                } else if (gsrBAUL[i].inFontStyle == _BAUL_FONT_STYLE_ITALIC_) {
                    inPRINT_TTF_SetFont_Style(_FONT_PRINT_ITALIC_);
                } else if (gsrBAUL[i].inFontStyle == _BAUL_FONT_STYLE_BOLD_) {
                    inPRINT_TTF_SetFont_Style(_FONT_PRINT_BOLD_);
                } else if (gsrBAUL[i].inFontStyle == _BAUL_FONT_STYLE_REVERSE_) {
                    inPRINT_TTF_SetFont_Style(_FONT_PRINT_REVERSE_);
                } else if (gsrBAUL[i].inFontStyle == _BAUL_FONT_STYLE_UNDERLINE_) {
                    inPRINT_TTF_SetFont_Style(_FONT_PRINT_UNDERLINE_);
                }
            }

            if (gsrBAUL[i].uszPrintPosition != _PRINT_BY_X_) {
                usReturnValue = CTOS_PrinterBufferPutStringAligned(uszBuffer, srBhandle->inYcurrentMAX - inFontYLength, (unsigned char*) &gsrBAUL[i].szString, &gsrBAUL[i].srFont_Attrib, gsrBAUL[i].uszPrintPosition);
            } else {
                usReturnValue = CTOS_PrinterBufferPutString(uszBuffer, gsrBAUL[i].intXPosition, srBhandle->inYcurrentMAX - inFontYLength, (unsigned char*) &gsrBAUL[i].szString, &gsrBAUL[i].srFont_Attrib);
            }

            if (usReturnValue == d_OK) {
                /* 寫成功 */
            } else {
                /* 寫失敗，拿來還原用 */
                srBhandle->inYcurrentMAX = inYcurrentMAX_Old;

                inPRINT_TTF_SetFont_Style(_FONT_PRINT_BOLD_);

                return (VS_ERROR);
            }
        }

        /* 換下一行 */
        srBhandle->inXcurrent = 0;
        srBhandle->inYcurrent = srBhandle->inYcurrentMAX;

        /* 清空資料暫存 */
        memset(gsrBAUL, 0x00, sizeof (BufferArrangeUnderLine) * ginBAUL_Index);
        ginBAUL_Index = 0;
    }

    return (VS_SUCCESS);
}
int inPRINT_Buffer_PutIn(char* szString, int inFontSize, unsigned char *uszBuffer, FONT_ATTRIB *srFont_Attrib, BufferHandle *srBhandle, int inNextLine, unsigned char uszPrintPosition) {
    int inRetVal; /* 若回傳沒紙要重印 */

    /* 設定列印字型大小 */
    srFont_Attrib->FontSize = inFontSize;
    /* 放超過Buffer高度，回傳錯誤(直接假設該行印最大字型，若會超出bound就直接印出來) */
    if ((srBhandle->inYcurrent + _MAX_Y_LENGTH_) > srBhandle->inYbound) {
        /* 多筆明細報表列印做畫面切換 避免誤認當機 */
        if (srBhandle->uszDetailPrint == VS_TRUE) {
            //			inDISP_PutGraphic(_PRT_RECEIPT_, 0, _COORDINATE_Y_LINE_8_7_);	/* 帳單列印中 */
        }
        printf("當放超過高度 直接OutPut並清空Buffer(保險起見)\n");
        /* 當放超過高度 直接OutPut並清空Buffer(保險起見) */
        inRetVal = inPRINT_Buffer_OutPut(uszBuffer, srBhandle);

        /* 多筆明細報表列印做畫面切換 避免誤認當機 */
        if (srBhandle->uszDetailPrint == VS_TRUE) {
            inDISP_PutGraphic(_PROCESS_, 0, _COORDINATE_Y_LINE_8_7_); /* 處理中... */
        }
    }

    inRetVal = inPRINT_Buffer_Sync_UnderLine(szString, uszBuffer, srFont_Attrib, srBhandle, inNextLine, uszPrintPosition, 0);
    return (inRetVal);
}
