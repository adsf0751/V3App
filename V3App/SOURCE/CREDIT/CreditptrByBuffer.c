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
        pString = "1314006807";
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
        pString = "13992020";
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
    int inPrinttype_ByBuffer = 0; //寫死 跑直式
    if (inPrinttype_ByBuffer) {
//        printf("列印直式-------\n");
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
    else
    {
        /* 橫式 */
        /* 城市別(City) */
        inRetVal = inPRINT_Buffer_PutIn("城市別(City)", _PRT_NORMAL2_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);

        memset(szPrintBuf1, 0x00, sizeof(szPrintBuf1));
        strcpy(szPrintBuf1,"NEW TAIPEI CITY");
        sprintf(szPrintBuf, "%s", szPrintBuf1);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);

        /* 卡別 檢查碼 */
        inRetVal = inPRINT_Buffer_PutIn("卡別(Card Type)", _PRT_NORMAL2_, uszBuffer, srFont_Attrib, srBhandle, _CURRENT_LINE_, _PRINT_LEFT_);
        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);

        /* 大高卡只能顯示有效期 */
        if (pobTran->srBRec.uszTakaTransBit == VS_TRUE)
        {
                inRetVal = inPRINT_Buffer_PutIn_Specific_X_Position("有效期(Exp. Date)", _PRT_NORMAL2_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_DEFINE_X_01_);
                if (inRetVal != VS_SUCCESS)
                    return (VS_ERROR);
        }
        else
        {
                inRetVal = inPRINT_Buffer_PutIn_Specific_X_Position("檢查碼(Check No.)", _PRT_NORMAL2_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_DEFINE_X_01_);
                if (inRetVal != VS_SUCCESS)
                    return (VS_ERROR);
        }

        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
        /* 卡別 */
        /* 【需求單 - 106349】自有品牌判斷需求 */
        /* unpack電文就存存CardLabel modify by LingHsiung 2020/2/14 上午 11:07 */
        inFunc_PAD_ASCII(szPrintBuf, pobTran->srBRec.szCardLabel, ' ', 12, _PADDING_RIGHT_);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _CURRENT_LINE_, _PRINT_LEFT_);
        /* 檢查碼 */
        memset(szTemplate1, 0x00, sizeof(szTemplate1));
        memset(szPrintBuf1, 0x00, sizeof(szPrintBuf1));
        strcpy(szTemplate1, pobTran->srBRec.szCheckNO);
        memcpy(szPrintBuf1, szTemplate1, strlen(szTemplate1));
        inRetVal = inPRINT_Buffer_PutIn_Specific_X_Position(szPrintBuf1, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_DEFINE_X_01_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
        /* 卡號 */
        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
        strcpy(szPrintBuf, pobTran->srBRec.szPAN);
        inRetVal = inPRINT_Buffer_PutIn("卡號(Card No.)", _PRT_NORMAL2_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
        strcpy(szPrintBuf, pobTran->srBRec.szPAN);
        /* 過卡方式 */
        strcat(szPrintBuf, "(W)");
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
        inRetVal = inPRINT_Buffer_PutIn("主機別/交易類別(Host/Trans.Type)", _PRT_NORMAL2_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
        sprintf(szPrintBuf, "%s %s %s", "NCCC", "00" ,"一般交易SALE");
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
        /* 批次號碼、授權碼 */
        inRetVal = inPRINT_Buffer_PutIn("批次號碼(Batch No.)", _PRT_NORMAL2_, uszBuffer, srFont_Attrib, srBhandle, _CURRENT_LINE_, _PRINT_LEFT_);
        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);

        inRetVal = inPRINT_Buffer_PutIn_Specific_X_Position("授權碼(Auth Code)", _PRT_NORMAL2_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_DEFINE_X_01_);
        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
        /* Batch Num */
        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
        sprintf(szPrintBuf, "%03ld", pobTran->srBRec.lnBatchNum);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _CURRENT_LINE_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
        /* Auth Code */
        memset(szPrintBuf1, 0x00, sizeof(szPrintBuf1));
        memcpy(szPrintBuf1, pobTran->srBRec.szAuthCode, _AUTH_CODE_SIZE_);
        inRetVal = inPRINT_Buffer_PutIn_Specific_X_Position(szPrintBuf1, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_DEFINE_X_01_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
        inRetVal = inPRINT_Buffer_PutIn("回覆碼(Resp. Code)", _PRT_NORMAL2_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
		
        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);

        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
        memcpy(szPrintBuf, pobTran->srBRec.szRespCode, 2);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
        /* 日期時間 */
        inRetVal = inPRINT_Buffer_PutIn("日期/時間(Date/Time)", _PRT_NORMAL2_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);

        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
        sprintf(szPrintBuf, "%.4s/%.2s/%.2s", &pobTran->srBRec.szDate[0], &pobTran->srBRec.szDate[4], &pobTran->srBRec.szDate[6]);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _CURRENT_LINE_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
	memset(szPrintBuf1, 0x00, sizeof(szPrintBuf1));
        sprintf(szPrintBuf1, "%.2s:%.2s",  &pobTran->srBRec.szTime[0], &pobTran->srBRec.szTime[2]);

        inRetVal = inPRINT_Buffer_PutIn_Specific_X_Position(szPrintBuf1, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_DEFINE_X_01_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
        /* 序號 調閱編號 */
        inRetVal = inPRINT_Buffer_PutIn("序號(Ref. No.)", _PRT_NORMAL2_, uszBuffer, srFont_Attrib, srBhandle, _CURRENT_LINE_, _PRINT_LEFT_);
        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
        inRetVal = inPRINT_Buffer_PutIn_Specific_X_Position("調閱編號(Inv.No)", _PRT_NORMAL2_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_DEFINE_X_01_);
        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
        /* 雖然電文RRN送12個byte，但RRN最後一碼是0x00，所以只看到11碼 */
        inFunc_PAD_ASCII(szPrintBuf, pobTran->srBRec.szRefNo, ' ', 12, _PADDING_RIGHT_);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _CURRENT_LINE_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
        memset(szPrintBuf1, 0x00, sizeof(szPrintBuf1));
        sprintf(szPrintBuf1, "%06ld", pobTran->srBRec.lnOrgInvNum);
        inRetVal = inPRINT_Buffer_PutIn_Specific_X_Position(szPrintBuf1, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_DEFINE_X_01_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
        inRetVal = inPRINT_Buffer_PutIn("櫃號(Store ID)", _PRT_NORMAL2_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
        inRetVal = inPRINT_Buffer_PutIn(pobTran->srBRec.szStoreID, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
        /* 斷行 */
        inRetVal = inPRINT_Buffer_PutIn("", _PRT_NORMAL2_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
    }
    return VS_SUCCESS;
}

/*
Function        :inCREDIT_PRINT_AMOUNT_ByBuffer
Date&Time       :2015/8/10 上午 10:24
Describe        :列印AMOUNT
*/
int inCREDIT_PRINT_Amount_ByBuffer(TRANSACTION_OBJECT *pobTran, unsigned char *uszBuffer, FONT_ATTRIB *srFont_Attrib, BufferHandle *srBhandle)
{
	int	i;
        char    szPrintBuf[84 + 1], szTemplate[42 + 1];

        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
        memset(szTemplate, 0x00, sizeof(szTemplate));
        int inPrinttype_ByBuffer = 0;
        if (inPrinttype_ByBuffer)
        {
                /* 直式 */
                /* 金額 */
                if(pobTran->srBRec.inCode == _TIP_)
                {
                        /* 金額 */
                        /* 初始化 */
                        memset(szTemplate, 0x00, sizeof(szTemplate));
                        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));

                        /* 將NT$ ＋數字塞到szTemplate中來inpad */
                        sprintf(szTemplate, "NT$ %ld", pobTran->srBRec.lnTxnAmount);
                        inFunc_PAD_ASCII(szTemplate, szTemplate, ' ', 14, _PADDING_LEFT_ );

                        /* 把前面的字串和數字結合起來 */
                        sprintf(szPrintBuf, "金額(Amount):%s", szTemplate);
                        inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);


                        /* 小費 */
                        /* 初始化 */
                        memset(szTemplate, 0x00, sizeof(szTemplate));
                        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));

                        /* 將NT$ ＋數字塞到szTemplate中來inpad */
                        sprintf(szTemplate, "NT$ %ld", pobTran->srBRec.lnTipTxnAmount);
                        inFunc_PAD_ASCII(szTemplate, szTemplate, ' ', 14, _PADDING_LEFT_);

                        /* 把前面的字串和數字結合起來 */
                        sprintf(szPrintBuf, "小費(Tips)  :%s", szTemplate);
                        inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);


                        /* 總計 */
                        /* 初始化 */
                        memset(szTemplate, 0x00, sizeof(szTemplate));
                        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));

                        /* 將NT$ ＋數字塞到szTemplate中來inpad */
                        sprintf(szTemplate, "NT$ %ld", (pobTran->srBRec.lnTxnAmount + pobTran->srBRec.lnTipTxnAmount));
                        inFunc_PAD_ASCII(szTemplate, szTemplate, ' ' , 14, _PADDING_LEFT_ );

                        /* 把前面的字串和數字結合起來 */
                        sprintf(szPrintBuf,"總計(Total) :%s", szTemplate);
                }
                else if ((pobTran->srBRec.uszVOIDBit == VS_TRUE	&& 
			 (pobTran->srBRec.inOrgCode != _REFUND_ && pobTran->srBRec.inOrgCode != _INST_REFUND_ && pobTran->srBRec.inOrgCode != _REDEEM_REFUND_)) ||
			  pobTran->srBRec.inCode == _REFUND_		|| 
			  pobTran->srBRec.inCode == _INST_REFUND_	|| 
			  pobTran->srBRec.inCode == _REDEEM_REFUND_)
                {
                        /* 初始化 */
                        memset(szTemplate, 0x00, sizeof(szTemplate));
                        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));

                        /* 將NT$ ＋數字塞到szTemplate中來inpad */
                        sprintf(szTemplate, "NT$ %ld", 0 - pobTran->srBRec.lnTxnAmount);
                        inFunc_PAD_ASCII(szTemplate , szTemplate, ' ' , 14, _PADDING_LEFT_ );

                        /* 把前面的字串和數字結合起來 */
                        sprintf(szPrintBuf, "金額(Amount):%s", szTemplate);
                }
                else if (pobTran->srBRec.inCode == _ADJUST_)
                {
                        /* 初始化 */
                        memset(szTemplate, 0x00, sizeof(szTemplate));
                        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));

                        /* 將NT$ ＋數字塞到szTemplate中來inpad */
                        sprintf(szTemplate, "NT$ %ld", pobTran->srBRec.lnAdjustTxnAmount);
                        inFunc_PAD_ASCII(szTemplate , szTemplate, ' ' , 14, _PADDING_LEFT_ );

                        /* 把前面的字串和數字結合起來 */
                        sprintf(szPrintBuf, "金額(Amount):%s", szTemplate);
                }
                else
                {
                        /* 初始化 */
                        memset(szTemplate, 0x00, sizeof(szTemplate));
                        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));

                        /* 將NT$ ＋數字塞到szTemplate中來inpad */
                        sprintf(szTemplate, "NT$ %ld", pobTran->srBRec.lnTxnAmount);
                        inFunc_PAD_ASCII(szTemplate , szTemplate, ' ' , 14, _PADDING_LEFT_ );

                        /* 把前面的字串和數字結合起來 */
                        sprintf(szPrintBuf, "金額(Amount):%s", szTemplate);
                }
                inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
                
		for (i = 0; i < 2; i++)
		{
			inPRINT_Buffer_PutIn("", _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
		}
        }
        /* 橫式 */
        /* 負向交易 */
	else if(pobTran->srBRec.uszVOIDBit == VS_TRUE)
	{
		/* 橫式 */
                /* 金額 */
                /* 取消退貨是正數 */
                if (pobTran->srBRec.inOrgCode == _REFUND_ || pobTran->srBRec.inOrgCode == _INST_REFUND_ || pobTran->srBRec.inOrgCode == _REDEEM_REFUND_)
                {
                        /* 初始化 */
                        memset(szTemplate, 0x00, sizeof(szTemplate));
                        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));

                        /* 將NT$ ＋數字塞到szTemplate中來inpad */
			sprintf(szTemplate, "%ld",  pobTran->srBRec.lnTxnAmount);
			inFunc_Amount_Comma(szTemplate, "NT$ " , '\x00', _SIGNED_NONE_,  17, _PADDING_RIGHT_);
			memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
			sprintf(szPrintBuf, "%s", szTemplate);
			inPRINT_Buffer_PutIn(szPrintBuf, _PRT_AMOUNT_, uszBuffer, srFont_Attrib, srBhandle, _CURRENT_LINE_, _PRINT_RIGHT_);

                        /* 把前面的字串和數字結合起來 */
			memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
			sprintf(szPrintBuf, "%s", "總計(Total) :");
			inPRINT_Buffer_PutIn(szPrintBuf, _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
                }
		else
		{
			/* 初始化 */
                        memset(szTemplate, 0x00, sizeof(szTemplate));
                        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));

                        /* 將NT$ ＋數字塞到szTemplate中來inpad */
			sprintf(szTemplate, "%ld",  (0 - pobTran->srBRec.lnTxnAmount));
			inFunc_Amount_Comma(szTemplate, "NT$ " , '\x00', _SIGNED_NONE_,  17, _PADDING_RIGHT_);
			memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
			sprintf(szPrintBuf, "%s", szTemplate);
			inPRINT_Buffer_PutIn(szPrintBuf, _PRT_AMOUNT_, uszBuffer, srFont_Attrib, srBhandle, _CURRENT_LINE_, _PRINT_RIGHT_);

                        /* 把前面的字串和數字結合起來 */
                        memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
			sprintf(szPrintBuf, "%s", "總計(Total) :");
			inPRINT_Buffer_PutIn(szPrintBuf, _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
		}
	}
	else
	{
		memset(szTemplate, 0x00, sizeof(szTemplate));
		szTemplate[6] = 'N';
		if (szTemplate[6] == 'Y')	/* 檢查是否有開小費 */
		{
			/* 退貨金額為負數 */
			if (pobTran->srBRec.inCode == _REFUND_ || pobTran->srBRec.inCode == _INST_REFUND_ || pobTran->srBRec.inCode == _REDEEM_REFUND_)
			{
				 /* 初始化 */
				memset(szTemplate, 0x00, sizeof(szTemplate));
				memset(szPrintBuf, 0x00, sizeof(szPrintBuf));

				/* 將NT$ ＋數字塞到szTemplate中來inpad */
				sprintf(szTemplate, "%ld",  (0 - pobTran->srBRec.lnTxnAmount));
				inFunc_Amount_Comma(szTemplate, "NT$ " , '\x00', _SIGNED_NONE_,  17, _PADDING_RIGHT_);
				memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
				sprintf(szPrintBuf, "%s", szTemplate);
				inPRINT_Buffer_PutIn(szPrintBuf, _PRT_AMOUNT_, uszBuffer, srFont_Attrib, srBhandle, _CURRENT_LINE_, _PRINT_RIGHT_);

				/* 把前面的字串和數字結合起來 */
				memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
				sprintf(szPrintBuf, "%s", "總計(Total) :");
				inPRINT_Buffer_PutIn(szPrintBuf, _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
			}
			/* 預授不會有小費，所以拉出來 */
			else if (pobTran->srBRec.inCode == _PRE_AUTH_ || pobTran->srBRec.inCode == _PRE_COMP_)
			{
				/* 初始化 */
				memset(szTemplate, 0x00, sizeof(szTemplate));
				memset(szPrintBuf, 0x00, sizeof(szPrintBuf));

				/* 將NT$ ＋數字塞到szTemplate中來inpad */
				sprintf(szTemplate, "%ld",  pobTran->srBRec.lnTxnAmount);
				inFunc_Amount_Comma(szTemplate, "NT$ " , '\x00', _SIGNED_NONE_,  17, _PADDING_RIGHT_);
				memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
				sprintf(szPrintBuf, "%s", szTemplate);
				inPRINT_Buffer_PutIn(szPrintBuf, _PRT_AMOUNT_, uszBuffer, srFont_Attrib, srBhandle, _CURRENT_LINE_, _PRINT_RIGHT_);

				/* 把前面的字串和數字結合起來 */
				memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
				sprintf(szPrintBuf, "%s", "總計(Total) :");
				inPRINT_Buffer_PutIn(szPrintBuf, _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
			}
			else
			{
				/* 金額 */
				/* 初始化 */
				memset(szTemplate, 0x00, sizeof(szTemplate));
				memset(szPrintBuf, 0x00, sizeof(szPrintBuf));

				/* 將NT$ ＋數字塞到szTemplate中來inpad */
				sprintf(szTemplate, "%ld",  pobTran->srBRec.lnTxnAmount);
				inFunc_Amount_Comma(szTemplate, "NT$" , '\x00', _SIGNED_NONE_, 17, _PADDING_RIGHT_);
				memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
				sprintf(szPrintBuf, "%s", szTemplate);
				inPRINT_Buffer_PutIn(szPrintBuf, _PRT_AMOUNT_, uszBuffer, srFont_Attrib, srBhandle, _CURRENT_LINE_, _PRINT_RIGHT_);
				
				/* 把前面的字串和數字結合起來 */
				memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
				sprintf(szPrintBuf, "%s", "金額(Amount):");
				inPRINT_Buffer_PutIn(szPrintBuf, _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
				
				/* lnTipAmount為0表示非小費 */
				if (pobTran->srBRec.lnTipTxnAmount == 0L)
				{
					/* 小費 */
					inPRINT_Buffer_PutIn("小費(Tips)  :__________________________", _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

					/* 總計 */
					inPRINT_Buffer_PutIn("總計(Total) :__________________________", _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
				}
				else
				{
					/* 小費 */
					/* 初始化 */
					memset(szTemplate, 0x00, sizeof(szTemplate));
					memset(szPrintBuf, 0x00, sizeof(szPrintBuf));

					/* 將NT$ ＋數字塞到szTemplate中來inpad */
					sprintf(szTemplate, "%ld",  pobTran->srBRec.lnTipTxnAmount);
					inFunc_Amount_Comma(szTemplate, "NT$" , '\x00', _SIGNED_NONE_, 17, _PADDING_RIGHT_);
					memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
					sprintf(szPrintBuf, "%s", szTemplate);
					inPRINT_Buffer_PutIn(szPrintBuf, _PRT_AMOUNT_, uszBuffer, srFont_Attrib, srBhandle, _CURRENT_LINE_, _PRINT_RIGHT_);

					/* 把前面的字串和數字結合起來 */
					memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
					sprintf(szPrintBuf, "%s", "小費(Tips)  :");
					inPRINT_Buffer_PutIn(szPrintBuf, _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);


					/* 總計 */
					/* 初始化 */
					memset(szTemplate, 0x00, sizeof(szTemplate));
					memset(szPrintBuf, 0x00, sizeof(szPrintBuf));

					/* 將NT$ ＋數字塞到szTemplate中來inpad */
					sprintf(szTemplate, "%ld",  (pobTran->srBRec.lnTxnAmount + pobTran->srBRec.lnTipTxnAmount));
					inFunc_Amount_Comma(szTemplate, "NT$" , '\x00', _SIGNED_NONE_, 17, _PADDING_RIGHT_);
					memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
					sprintf(szPrintBuf, "%s", szTemplate);
					inPRINT_Buffer_PutIn(szPrintBuf, _PRT_AMOUNT_, uszBuffer, srFont_Attrib, srBhandle, _CURRENT_LINE_, _PRINT_RIGHT_);

					/* 把前面的字串和數字結合起來 */
					memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
					sprintf(szPrintBuf, "%s", "總計(Total) :");
					inPRINT_Buffer_PutIn(szPrintBuf, _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
				}
				
			}
			
		}
		/* 小費沒開時 */
		else
		{
			 /* 初始化 */
			memset(szTemplate, 0x00, sizeof(szTemplate));
			memset(szPrintBuf, 0x00, sizeof(szPrintBuf));

			/* 將NT$ ＋數字塞到szTemplate中來inpad，退貨要負數 */
			if (pobTran->srBRec.inCode == _REFUND_ || pobTran->srBRec.inCode == _INST_REFUND_ || pobTran->srBRec.inCode == _REDEEM_REFUND_)
			{
				sprintf(szTemplate, "%ld",  (0 - pobTran->srBRec.lnTxnAmount));
			}
			else
			{
				sprintf(szTemplate, "%ld",  pobTran->srBRec.lnTxnAmount);
			}
			
			inFunc_Amount_Comma(szTemplate, "NT$" , '\x00', _SIGNED_NONE_, 17, _PADDING_RIGHT_);
			memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
			sprintf(szPrintBuf, "%s", szTemplate);
			inPRINT_Buffer_PutIn(szPrintBuf, _PRT_AMOUNT_, uszBuffer, srFont_Attrib, srBhandle, _CURRENT_LINE_, _PRINT_RIGHT_);
			

			memset(szPrintBuf, 0x00, sizeof(szPrintBuf));
			sprintf(szPrintBuf, "%s", "總計(Total) :");
			inPRINT_Buffer_PutIn(szPrintBuf, _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
		}
		
	}
	/* 斷行 */
	inPRINT_Buffer_PutIn("", _PRT_NORMAL2_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
	
        return (VS_SUCCESS);
}

/*
Function        :inCREDIT_PRINT_ReceiptEND_ByBuffer
Date&Time       :2015/8/10 上午 10:24
Describe        :列印結尾
*/
int inCREDIT_PRINT_ReceiptEND_ByBuffer(TRANSACTION_OBJECT *pobTran, unsigned char *uszBuffer, FONT_ATTRIB *srFont_Attrib, BufferHandle *srBhandle)
{
	int	i = 0;
	int	inRetVal = VS_ERROR;
	char	szTemplate[42 + 1]= {0};
	char	szSignature[30 + 1] = {0};
	char	szSignaturePath[80 + 1] = {0};
	char	szDemoMode[2 + 1] = {0};
	int inPrinttype_ByBuffer = 0;
        if (inPrinttype_ByBuffer)
        {
                /* 直式 */
                inPRINT_Buffer_PutIn("簽名欄:_____________________", _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

                if (pobTran->srBRec.inPrintOption == _PRT_MERCH_ ||
		    pobTran->srBRec.inPrintOption == _PRT_MERCH_DUPLICATE_)
                {
                        inPRINT_Buffer_PutIn("*** 商店收據 Merchant Copy ***", _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
                        pobTran->srBRec.inPrintOption = _PRT_CUST_;

                }
                else if (pobTran->srBRec.inPrintOption == _PRT_CUST_)
                {
                        inPRINT_Buffer_PutIn("*** 持卡人收據 Customer Copy ***", _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
                        pobTran->srBRec.inPrintOption = _PRT_MERCH_;
                }

                inPRINT_Buffer_PutIn("I AGREE TO PAY TOTAL AMOUNT", _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
                inPRINT_Buffer_PutIn("ACCORDING TO CARD ISSUER AGREEMENT", _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
                for (i = 0; i < 8; i++)
		{
			inPRINT_Buffer_PutIn("", _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
		}
		
        }
        else
        {
                /* 橫式 */
//                if (pobTran->srBRec.inPrintOption == _PRT_MERCH_)
//                {
//			/* 教育訓練模式 */
//			memset(szDemoMode, 0x00, sizeof(szDemoMode));
//			inGetDemoMode(szDemoMode);
//			if (memcmp(szDemoMode, "Y", strlen("Y")) == 0)
//			{
//				if (inPRINT_Buffer_PutGraphic((unsigned char*)_NCCC_DEMO_, uszBuffer, srBhandle, 50, _APPEND_) != VS_SUCCESS)
//				{
//					if (ginDebug == VS_TRUE)
//					{
//						inLogPrintf(AT, "inPRINT_PutGraphic(_NCCC_DEMO_) failed");
//					}
//
//				}
//			}
			
                        /* 簽名欄 */
			/* 免簽名 */
//			if (pobTran->srBRec.uszNoSignatureBit == VS_TRUE && pobTran->srBRec.inCode != _TIP_)
//			{
//				inPRINT_Buffer_PutIn("免      簽      名", _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_CENTER_);
//			}
			/* 要簽名 */
//			else
//			{
//				/* 藉由TRT_FileName比對來組出bmp的檔名 */
//				inLoadHDPTRec(pobTran->srBRec.inHDTIndex);
//				
//				memset(szSignature, 0x00, sizeof(szSignature));
//				/* 因為用invoice所以不用inFunc_ComposeFileName */
//				inFunc_ComposeFileName_InvoiceNumber(pobTran, szSignature, _PICTURE_FILE_EXTENSION_, 6);
//				memset(szSignaturePath, 0x00, sizeof(szSignaturePath));
//				sprintf(szSignaturePath, "./fs_data/%s", szSignature);
//				/* 圖檔存在、有在signpad簽名、且非重印（重印不出簽名）（目前簽名狀態存不了Batch，先把&& pobTran->srBRec.inSignStatus == _SIGN_SIGNED_此條件拿掉） */
//				if (inFILE_Check_Exist((unsigned char *)szSignature) == VS_SUCCESS && pobTran->inRunOperationID != _OPERATION_REPRINT_)
//				{
//					/* 電子簽名 */
//					inPRINT_Buffer_PutGraphic((unsigned char *)szSignaturePath, uszBuffer, srBhandle, _SIGNEDPAD_WIDTH_, _APPEND_);
//				}
//				/* 手簽 */
//				else
//				{
//					/* a space 2 line */
//					for (i = 0; i < 2; i++)
//					{
//						inPRINT_Buffer_PutIn("", _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
//					}
//					
//				}	
//			}
                        
//                        inPRINT_Buffer_PutIn("X:________________________________", _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
//			/* 持卡人姓名 */
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			sprintf(szTemplate, "%s", pobTran->srBRec.szCardHolder);
//			inPRINT_Buffer_PutIn(szTemplate, _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
//                        inPRINT_Buffer_PutIn("持卡人簽名(特店存根聯)", _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_CENTER_);
//                }
//		else if (pobTran->srBRec.inPrintOption == _PRT_MERCH_DUPLICATE_)
//                {
//			/* 教育訓練模式 */
//			memset(szDemoMode, 0x00, sizeof(szDemoMode));
//			inGetDemoMode(szDemoMode);
//			if (memcmp(szDemoMode, "Y", strlen("Y")) == 0)
//			{
//				if (inPRINT_Buffer_PutGraphic((unsigned char*)_NCCC_DEMO_, uszBuffer, srBhandle, 50, _APPEND_) != VS_SUCCESS)
//				{
//					if (ginDebug == VS_TRUE)
//					{
//						inLogPrintf(AT, "inPRINT_PutGraphic(_NCCC_DEMO_) failed");
//					}
//
//				}
//			}
//
//			if (pobTran->srBRec.uszNoSignatureBit == VS_TRUE && pobTran->srBRec.inCode != _TIP_)
//			{
//				/* 免簽名 */
//				inRetVal = inPRINT_Buffer_PutIn("免      簽      名", _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_CENTER_);
//				if (inRetVal != VS_SUCCESS)
//					return (VS_ERROR);
//			}
//			inPRINT_Buffer_PutIn("", _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
//                        inPRINT_Buffer_PutIn("　　　　　　　　　 商店存根", _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
//			inPRINT_Buffer_PutIn("--------------------------------------------------------------------------------------------------------------", _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
//			/* 持卡人姓名 */
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			sprintf(szTemplate,"%s",pobTran->srBRec.szCardHolder);
//			inPRINT_Buffer_PutIn(szTemplate, _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
//		}
//                else
//                {
//			/* 教育訓練模式 */
//			memset(szDemoMode, 0x00, sizeof(szDemoMode));
//			inGetDemoMode(szDemoMode);
//			if (memcmp(szDemoMode, "Y", strlen("Y")) == 0)
//			{
//				if (inPRINT_Buffer_PutGraphic((unsigned char*)_NCCC_DEMO_, uszBuffer, srBhandle, 50, _APPEND_) != VS_SUCCESS)
//				{
//					if (ginDebug == VS_TRUE)
//					{
//						inLogPrintf(AT, "inPRINT_PutGraphic(_NCCC_DEMO_) failed");
//					}
//
//				}
//			}
//
			if (pobTran->srBRec.uszNoSignatureBit == VS_TRUE && pobTran->srBRec.inCode != _TIP_)
			{
				/* 免簽名 */
				inRetVal = inPRINT_Buffer_PutIn("免      簽      名", _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_CENTER_);
				if (inRetVal != VS_SUCCESS)
					return (VS_ERROR);
			}
			inPRINT_Buffer_PutIn("", _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
                        inPRINT_Buffer_PutIn("　　　　　　　　　 持卡人存根", _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
			inPRINT_Buffer_PutIn("　　　　　　　  Card holder stub", _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
			inPRINT_Buffer_PutIn("--------------------------------------------------------------------------------------------------------------", _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
			/* 持卡人姓名 */
			memset(szTemplate, 0x00, sizeof(szTemplate));
			sprintf(szTemplate,"%s",pobTran->srBRec.szCardHolder);
			inPRINT_Buffer_PutIn(szTemplate, _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
//                }
//		
//		if (pobTran->inRunOperationID == _OPERATION_REPRINT_)
//		{
//			inPRINT_Buffer_PutIn("重印 REPRINT", _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_CENTER_);
//		}
		
                /* 列印警示語 */
                inPRINT_Buffer_PutIn("            I AGREE TO PAY TOTAL AMOUNT", _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
                inPRINT_Buffer_PutIn("        ACCORDING TO CARD ISSUER AGREEMENT", _PRT_NORMAL_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
		
		/* Print Notice */
//		if (inCREDIT_PRINT_Notice(pobTran, uszBuffer, srBhandle) != VS_SUCCESS)
//			return (VS_ERROR);
//		
//		/* Print Slogan */
//		if (pobTran->srBRec.inPrintOption == _PRT_CUST_)
//		{
//			if (inCREDIT_PRINT_MarchantSlogan(pobTran, _NCCC_SLOGAN_PRINT_DOWN_, uszBuffer, srBhandle) != VS_SUCCESS)
//				return (VS_ERROR);
//			
//			if (pobTran->srBRec.uszRewardL1Bit == VS_TRUE	|| 
//			    pobTran->srBRec.uszRewardL2Bit == VS_TRUE	||
//			    pobTran->srBRec.uszRewardL5Bit == VS_TRUE)
//			{
//				if (inCREDIT_PRINT_RewardAdvertisement(pobTran, uszBuffer, srFont_Attrib, srBhandle) != VS_SUCCESS)
//					return (VS_ERROR);
//			}
//			
//		}
		
                for (i = 0; i < 8; i++)
		{
			inPRINT_Buffer_PutIn("", _PRT_HEIGHT_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);
		}
		
        }

        return (VS_SUCCESS);
}