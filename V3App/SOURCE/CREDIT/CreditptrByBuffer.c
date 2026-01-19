#include "CreditptrByBuffer.h"
extern	BMPHeight	gsrBMPHeight;			/* 圖片高度 */
int     inPrinttype_ByBuffer = 0;        /* 0 = 橫式，1 = 直式 */
int inCREDIT_PRINT_MerchantLogo(TRANSACTION_OBJECT *pobTran, unsigned char *uszBuffer, BufferHandle *srBhandle,BMPHeight*	gsrBMPHeight) {
    if (inPRINT_Buffer_PutGraphic((unsigned char*) _MERCHANT_LOGO_, uszBuffer, srBhandle, gsrBMPHeight->inMerchantLogoHeight, _APPEND_) != VS_SUCCESS) {
        return (VS_ERROR);
    }
    return (VS_SUCCESS);
}

/*
Function        :inCREDIT_PRINT_MerchantName
Date&Time       :2016/9/7 下午 5:55
Describe        :用來決定要不要印商店表頭
 */
int inCREDIT_PRINT_MerchantName(TRANSACTION_OBJECT *pobTran, unsigned char *uszBuffer, BufferHandle *srBhandle,BMPHeight* gsrBMPHeight) {
    if (inPRINT_Buffer_PutGraphic((unsigned char*) _NAME_LOGO_, uszBuffer, srBhandle, gsrBMPHeight->inTitleNameHeight, _APPEND_) != VS_SUCCESS) {
        return (VS_ERROR);
    }
    return (VS_SUCCESS);
}

int inCREDIT_PRINT_Logo_ByBuffer(TRANSACTION_OBJECT *pobTran, unsigned char *uszBuffer, FONT_ATTRIB *srFont_Attrib, BufferHandle *srBhandle,BMPHeight* gsrBMPHeight) {
    if (inPRINT_Buffer_PutGraphic((unsigned char*) _BANK_LOGO_, uszBuffer, srBhandle, gsrBMPHeight->inBankLogoHeight, _APPEND_) != VS_SUCCESS) {
        return (VS_ERROR);
    }
  
    /* 印商店的LOGO */
    if (inCREDIT_PRINT_MerchantLogo(pobTran, uszBuffer, srBhandle,gsrBMPHeight) != VS_SUCCESS) {
        return (VS_ERROR);
    }
    //	/* 印商店名稱 */
    if (inCREDIT_PRINT_MerchantName(pobTran, uszBuffer, srBhandle,gsrBMPHeight) != VS_SUCCESS) {
        return (VS_ERROR);
    }
    return (VS_SUCCESS);
}

/*
Function        :inCREDIT_PRINT_TIDMID_ByBuffer
Date&Time       :2015/8/10 上午 10:24
Describe        :列印TID & MID
 */
int inCREDIT_PRINT_Tidmid_ByBuffer(TRANSACTION_OBJECT *pobTran, unsigned char *uszBuffer, FONT_ATTRIB *srFont_Attrib, BufferHandle *srBhandle) {
    int inRetVal;
    int szLEN = 84;
    char szPrintBuf[szLEN + 1], szTemplate[42 + 1];
    char *pString = NULL ;
    memset(szPrintBuf, 0x00, sizeof (szPrintBuf));
    memset(szTemplate, 0x00, sizeof (szTemplate));
    int inPrinttype_ByBuffer = 0;
    if (inPrinttype_ByBuffer) {
        /* 直式 */
        /* Get商店代號 */
        memset(szTemplate, 0x00, sizeof(szTemplate));
        pString = "0001";
        strncpy(szTemplate,pString,strlen(pString));
        //                inGetMerchantID(szTemplate);
        
        /* 列印商店代號 */
        memset(szPrintBuf, 0x00, sizeof (szPrintBuf));
        snprintf(szPrintBuf,szLEN, "商店代號：%s", szTemplate);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);

        /* Get端末機代號 */
        memset(szTemplate, 0x00, sizeof(szTemplate));
        pString = "1234";
        strncpy(szTemplate,pString,strlen(pString));
        //inGetTerminalID(szTemplate);

        /* 列印端末機代號 */
        memset(szPrintBuf, 0x00, sizeof (szPrintBuf));
        
        snprintf(szPrintBuf,szLEN, "端末機代號：%s", szTemplate);
 
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
    } else {
        /* 橫式 */
        memset(szTemplate, 0x00, sizeof(szTemplate));
        pString = "4567";
        strncpy(szTemplate,pString,strlen(pString));
        //inGetMerchantID(szTemplate);

        /* 列印商店代號 */
        inFunc_PAD_ASCII(szTemplate, szTemplate, ' ', 15, _PADDING_LEFT_);
        snprintf(szPrintBuf,szLEN, "商店代號 %s", szTemplate);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);

        /* Get端末機代號 */
        memset(szTemplate, 0x00, sizeof(szTemplate));
        pString = "121314151617";
        strncpy(szTemplate,pString,strlen(pString));
        //                inGetTerminalID(szTemplate);

        /* 列印端末機代號 */
        inFunc_PAD_ASCII(szTemplate, szTemplate, ' ', 13, _PADDING_LEFT_);
        snprintf(szPrintBuf,szLEN, "端末機代號 %s", szTemplate);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);

        inRetVal = inPRINT_Buffer_PutIn("================================================", _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
    }

    return (inRetVal);
}

int inCREDIT_PRINT_Data_ByBuffer(TRANSACTION_OBJECT *pobTran, unsigned char *uszBuffer, FONT_ATTRIB *srFont_Attrib, BufferHandle *srBhandle) {
    int i;
    int inRetVal;
    int szLEN = 100;
    char szPrintBuf[szLEN + 1], szPrintBuf1[42 + 1], szPrintBuf2[42 + 1], szTemplate1[42 + 1], szTemplate2[42 + 1];
    char szProductCodeEnable[1 + 1];
    char szStore_Stub_CardNo_Truncate_Enable[2 + 1];
    char szExamBit[10 + 1] = {0};
    char szCustomerIndicator[3 + 1] = {0};
    unsigned char uszChangeLineBit = VS_FALSE;

    //memset(szCustomerIndicator, 0x00, sizeof (szCustomerIndicator));
    //inGetCustomIndicator(szCustomerIndicator);

    memset(szPrintBuf , 0x00, sizeof (szPrintBuf));
    memset(szPrintBuf1, 0x00, sizeof (szPrintBuf1));
    memset(szPrintBuf2, 0x00, sizeof (szPrintBuf2));
    memset(szTemplate1, 0x00, sizeof (szTemplate1));
    memset(szTemplate2, 0x00, sizeof (szTemplate2));
    int inPrinttype_ByBuffer = 1; //寫死 跑直式
    if (inPrinttype_ByBuffer) {
        printf("列印直式-------\n");
        /* 直式 */
        /*卡別、卡號*/
        /* 【需求單 - 106349】自有品牌判斷需求 */
        /* unpack電文就存存CardLabel modify by LingHsiung 2020/2/14 上午 11:07 */
        snprintf(szPrintBuf,szLEN, "卡別　　：%s", pobTran->srBRec.szCardLabel);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        memset(szPrintBuf, 0x00, sizeof (szPrintBuf));
        snprintf(szPrintBuf,szLEN, "卡號　　：%s", pobTran->srBRec.szPAN);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        /*日期、時間*/
        memset(szPrintBuf, 0x00, sizeof (szPrintBuf));
        snprintf(szPrintBuf,szLEN, "日期　　：%s", pobTran->srBRec.szDate);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        memset(szPrintBuf, 0x00, sizeof (szPrintBuf));
        snprintf(szPrintBuf,szLEN, "時間　　：%s", pobTran->srBRec.szTime);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        /*調閱編號、批次號碼 */
        memset(szPrintBuf, 0x00, sizeof (szPrintBuf));
        sprintf(szPrintBuf, "調閱編號：%06ld", pobTran->srBRec.lnOrgInvNum);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        memset(szPrintBuf, 0x00, sizeof (szPrintBuf));
        sprintf(szPrintBuf, "批次號碼：%06ld", pobTran->srBRec.lnBatchNum);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        //                /*交易類別*/
        //                inFunc_GetTransType(pobTran, szPrintBuf1, szTemplate2);
        //                memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
        sprintf(szPrintBuf, "交易類別:testType");
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        //                /*授權碼、序號*/
        sprintf(szPrintBuf, "授權碼　：%s", pobTran->srBRec.szAuthCode);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
        //
        //                memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
        sprintf(szPrintBuf, "序號　　：%s", pobTran->srBRec.szRefNo);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        //                memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
    }
    return VS_SUCCESS;
}