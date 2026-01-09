
#include <ctosapi.h>
#include <stdio.h>
#include <string.h>
#include "../../Trans.h"
#include "File.h"
/*
Function	:inFILE_Close
Date&Time	:2015/8/5 下午 5:12
Describe	:關檔案
 */
int inFILE_Close(unsigned long *ulFileHandle) {
    unsigned short usRetVal;
    /* 關閉檔案Handle */
    usRetVal = CTOS_FileClose(*ulFileHandle);

    if (usRetVal != d_OK) {

        return (VS_ERROR);
    }
    //	else
    //	{
    //		inFile_Open_File_Cnt_Decrease();
    //	}

    *ulFileHandle = VS_HANDLE_NULL;
    return (VS_SUCCESS);
}

/*
Function	:inFILE_OpenReadOnly
Date&Time	:2015/9/16 上午 11:34
Describe	:輸入檔案名稱及Handle，開啟檔案，判斷檔案不存在直接Return不建立新檔
 */
int inFILE_OpenReadOnly(unsigned long *ulFileHandle, unsigned char *uszFileName) {
    unsigned long ulFileSize = 0;
    unsigned short usRetVal;

    if (ulFileHandle == NULL || strlen((char *) uszFileName) == 0) {
        return (VS_ERROR);
    }
    /* 取得檔案大小，得到回傳值來判斷檔案是否存在 */
    usRetVal = CTOS_FileGetSize((BYTE *) uszFileName, &ulFileSize);

    /* 判斷檔案不存在，直接Return Error不建立新檔 */
    if (usRetVal == d_FS_FILE_NOT_FOUND) {
        printf("%40s is not found\n", uszFileName);
        return (VS_ERROR);
    }
    /* 用輸入的檔名開啟檔案 */
    usRetVal = CTOS_FileOpen(uszFileName, d_STORAGE_FLASH, ulFileHandle);

    if (usRetVal != d_OK) {
        inFILE_Close(&(*ulFileHandle));
        return (VS_ERROR);
    }
    return (VS_SUCCESS);
}
/*
Function	:inFILE_Seek
Date&Time	:2015/8/5 下午 5:12
Describe	:尋找檔案，傳入Handle尋找位置及尋找模式_SEEK_BEGIN_或_SEEK_CURRENT_或_SEEK_END_
 */
int inFILE_Seek(unsigned long ulFileHandle, unsigned long ulOffset, int inSeekMode) {
    unsigned short usRetVal = VS_ERROR;

    if (inSeekMode == _SEEK_BEGIN_ || _SEEK_CURRENT_ || _SEEK_END_) {
        usRetVal = CTOS_FileSeek(ulFileHandle, ulOffset, inSeekMode);
    } else {
        return (VS_ERROR);
    }

    if (usRetVal != d_OK) {
        return (VS_ERROR);
    }
    return (VS_SUCCESS);
}
/*
Function	:inFILE_Read
Date&Time	:2015/8/5 下午 5:12
Describe	:讀檔案，傳入Handle接收的Buffer及讀取的size
 */
int inFILE_Read(unsigned long *ulFileHandle, unsigned char *uszReadData, unsigned long ulReadSize) {
    unsigned short usRetVal; /* 實際讀到的長度 */
    usRetVal = CTOS_FileRead(*ulFileHandle, uszReadData, &ulReadSize);

    if (usRetVal != d_OK) {
        return (VS_ERROR);
    }
    return (VS_SUCCESS);
}