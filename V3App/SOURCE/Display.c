#include <ctosapi.h>
#include <stdio.h>
#include <string.h>
#include "../Trans.h"
#include "Display.h"
/*
Function	:inPutGraphic
Date&Time	:2015/6/7 下午 5:09
Describe	:顯示LCD BMP圖檔
 */
int inDISP_PutGraphic(char *szFileName, int inX, int inY) {
    int inY_Modify;
    char szDebugMsg[100 + 1];
    unsigned short usRetVal = 0;
    //	if (ginHalfLCD == VS_TRUE)
    //		inY_Modify = inY / 2;
    //	else
    inY_Modify = inY;

    usRetVal = CTOS_LCDGShowBMPPic(inX, inY_Modify, (BYTE *) szFileName);
    ;
    if (usRetVal == d_OK) {
        printf(" CTOS_LCDGShowBMPPic == d_OK\n");
    } else {
        printf("VS_ERROR\n");
        return (VS_ERROR);
    }
    return (VS_SUCCESS);
}

/*
Function	:inPRINT_Buffer_Get_Height
Date&Time	:2016/3/16 下午 2:15
Describe	:用來預先獲取圖檔的高度，以便利用
 */
int inPRINT_Buffer_GetHeight(unsigned char* uszFileName, int* inHeight) {
    int i;
    int inCalulateHeight = 0;
    char szAscii[8 + 1];
    char szDebugMsg[100 + 1];
    unsigned char uszHex[4 + 1];
    unsigned char uszHeight[4 + 1];
    unsigned long ulHandle;

    memset(&ulHandle, 0x00, sizeof (ulHandle));
    memset(uszHeight, 0x00, sizeof (uszHeight));
    memset(uszHex, 0x00, sizeof (uszHex));
    memset(szAscii, 0x00, sizeof (szAscii));
    /* 初始為0 若因錯誤而跳出則高度為0 */
    *inHeight = 0;

    if (inFILE_OpenReadOnly(&ulHandle, uszFileName) != VS_SUCCESS) {
        return (VS_ERROR);
    }
    /* 16 in hex = 22 in decimal 圖檔放高度資訊的位置 */
    //參數三傳入0  >>  inSeekMode == _SEEK_BEGIN_
    if (inFILE_Seek(ulHandle, 22, 0) != VS_SUCCESS) {
        printf("inFILE_Seek\n");
        return (VS_ERROR);
    }

    if (inFILE_Read(&ulHandle, uszHeight, 4) != VS_SUCCESS) {
        return (VS_ERROR);
    }

    /* height資料只有四個bytes，因為是Little-Endian，所以要先反過來 */
    for (i = 0; i < 4; i++) {
        memcpy(&uszHex[i], &uszHeight[3 - i], 1);
    }
    /* 16進制轉10進制，以byte為單位，從高位乘上256加到低位 */
    for (i = 0; i < 4; i++) { // x*256 = x <<8(256 == 2^8) 
        //uszHex[i] - 0 是為了把字元強制轉整數?
        inCalulateHeight *= 256;
        inCalulateHeight += uszHex[i] - 0;
    }

    /* 加到變數上 */
    *inHeight = inCalulateHeight;
    /* 關閉檔案 */
    inFILE_Close(&ulHandle);
    return (VS_SUCCESS);
}
int inDISP_Timer_Start(int inTimerNumber, long lnDelay) {
//    int inEnterTimeout = 0;
//    char szEnterTimeout[3 + 1];

//    if (lnDelay == _EDC_TIMEOUT_) {
//        memset(szEnterTimeout, 0x00, sizeof (szEnterTimeout));
//        //inGetEnterTimeout(szEnterTimeout); //to fix
//        inEnterTimeout = atoi(szEnterTimeout);
//        if (inEnterTimeout != 0) {
//            inTimerStart(inTimerNumber, (long) inEnterTimeout);
//        } else {
//            return (VS_ERROR);
//        }
//    } 
//    else {
        inTimerStart(inTimerNumber, lnDelay);
//    }

    return (VS_SUCCESS);
}
/*
Function        :inTimerGet
Date&Time       :2016/6/21 下午 1:54
Describe        :確認計時器是否TimeOut，若timeout會回傳VS_SUCCESS
*/
int inTimerGet(int inTimerNbr)
{
         if (CTOS_TimeOutCheck(inTimerNbr) == d_YES)
                return (VS_SUCCESS);
         else
                return (VS_ERROR);
}

int inTimerStart(int inTimerNbr, long lnDelay) {
    CTOS_TimeOutSet(inTimerNbr, lnDelay * 100);

    return (VS_SUCCESS);
}