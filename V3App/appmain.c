
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctosapi.h>
#include <fcntl.h>
#include "Trans.h"
#include "print.h"
#include "PrtMsg.h"

#include "SOURCE/FUNCTION/Function.h"
#include "SOURCE/FUNCTION/File.h"
#include "SOURCE/COMM/Ethernet.h"
#include "SOURCE/NCCC/NCCCats.h"
#include "SOURCE/Print.h"
#include "SOURCE/FUNCTION/ECR.h"
#define _AP_ROOT_PATH_   "./"
#define _SHORT_RECEIPT_U_   "U"
#define _NCCC_TEXT_LOGO_    "財團法人聯合信用卡處理中心"
#define _PRT_12X30_   0x0C1E
#define _SEEK_BEGIN_      d_SEEK_FROM_BEGINNING
#define _SEEK_CURRENT_    d_SEEK_FROM_CURRENT
#define _SEEK_END_        d_SEEK_FROM_EOF
#define d_SEEK_FROM_BEGINNING   0
#define d_SEEK_FROM_CURRENT     1
#define d_SEEK_FROM_EOF         2

#define _PRT_NORMAL_            d_FONT_8x16
#define d_FONT_8x16             0x0810

#define _EDC_TIMEOUT_           -1  /* 由EDC.dat控制 */
#define AF_INET                 PF_INET
#define	PF_INET                 2	/* IP protocol family.  */
#define SOL_SOCKET              1
#define SO_REUSEADDR            2
#define SOL_TCP                 6	/* TCP level */
#define	TCP_MAXSEG              2	/* Set maximum segment size  */

typedef struct {
    int inBankLogoHeight; /* 銀行LOGO高度 */
    int inMerchantLogoHeight; /* 商店LOGO高度 */
    int inTitleNameHeight; /* 表頭圖檔高度 */
    int inSloganHeight; /* 企業標語高度 */
    int inCupLegalHeight; /* CUP警語高度 */
    int inInstHeight; /* 分期警語高度 */
    int inNoticeHeight; /* 商店提示與高度 */
} BMPHeight;
typedef struct
{
	int	inRecordRowID;			/* SQLite使用，用於暫存Table的Rowid */
	char	szTMS_IP_Primary[15 + 1];
	char	szTMS_PortNo_Primary[5 + 1];
	char	szTMS_IP_Second[15 + 1];
	char	szTMS_PortNo_Second[5 + 1];
} TMSIPDT_REC;

int imgHeight     = 0;
BMPHeight gsrBMPHeight;
static  TMSIPDT_REC	srTMSIPDTRec;	/* construct TMSIPDT record */

int inCREDIT_PRINT_MerchantLogo(TRANSACTION_OBJECT *pobTran, unsigned char *uszBuffer, BufferHandle *srBhandle) {
    if (inPRINT_Buffer_PutGraphic((unsigned char*) _MERCHANT_LOGO_, uszBuffer, srBhandle, gsrBMPHeight.inMerchantLogoHeight, _APPEND_) != VS_SUCCESS) {
        return (VS_ERROR);
    }
    return (VS_SUCCESS);
}

/*
Function        :inCREDIT_PRINT_MerchantName
Date&Time       :2016/9/7 下午 5:55
Describe        :用來決定要不要印商店表頭
 */
int inCREDIT_PRINT_MerchantName(TRANSACTION_OBJECT *pobTran, unsigned char *uszBuffer, BufferHandle *srBhandle) {
    if (inPRINT_Buffer_PutGraphic((unsigned char*) _NAME_LOGO_, uszBuffer, srBhandle, gsrBMPHeight.inTitleNameHeight, _APPEND_) != VS_SUCCESS) {
        return (VS_ERROR);
    }
    return (VS_SUCCESS);
}

int inCREDIT_PRINT_Logo_ByBuffer(TRANSACTION_OBJECT *pobTran, unsigned char *uszBuffer, FONT_ATTRIB *srFont_Attrib, BufferHandle *srBhandle) {
    /* 印Slogan 384*180 */
    //	if (pobTran->srBRec.inPrintOption == _PRT_CUST_)
    //	{
    //		if (inCREDIT_PRINT_MarchantSlogan(pobTran, _NCCC_SLOGAN_PRINT_UP_, uszBuffer, srBhandle) != VS_SUCCESS)
    //			return (VS_ERROR);
    //	}

    //        if (!memcmp(szShort_Receipt_Mode, _SHORT_RECEIPT_U_, strlen(_SHORT_RECEIPT_U_)))
    //        {
    /* 印NCC的LOGO */
    //                if (inPRINT_Buffer_PutIn(_NCCC_TEXT_LOGO_, _PRT_12X30_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_CENTER_) != VS_SUCCESS)
    //                {
    //                        return (VS_ERROR);
    //                }
    //        }
    //        else
    //        {
    //                /* 印NCC的LOGO */

    if (inPRINT_Buffer_PutGraphic((unsigned char*) _BANK_LOGO_, uszBuffer, srBhandle, gsrBMPHeight.inBankLogoHeight, _APPEND_) != VS_SUCCESS) {
        return (VS_ERROR);
    }
    //        }
    //
    //        /* 印商店的LOGO */
    if (inCREDIT_PRINT_MerchantLogo(pobTran, uszBuffer, srBhandle) != VS_SUCCESS) {
        return (VS_ERROR);
    }
    //	
    //	/* 印商店名稱 */
    if (inCREDIT_PRINT_MerchantName(pobTran, uszBuffer, srBhandle) != VS_SUCCESS) {
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
    char szPrintBuf[84 + 1], szTemplate[42 + 1];

    memset(szPrintBuf, 0x00, sizeof (szPrintBuf));
    memset(szTemplate, 0x00, sizeof (szTemplate));
    int inPrinttype_ByBuffer = 0;
    if (inPrinttype_ByBuffer) {
        /* 直式 */
        /* Get商店代號 */
        //                memset(szTemplate, 0x00, sizeof(szTemplate));
        //                inGetMerchantID(szTemplate);

        /* 列印商店代號 */
        memset(szPrintBuf, 0x00, sizeof (szPrintBuf));
        //                sprintf(szPrintBuf, "商店代號：%s", szTemplate);
        sprintf(szPrintBuf, "商店代號：0001");
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);

        /* Get端末機代號 */
        //                memset(szTemplate, 0x00, sizeof(szTemplate));
        //                inGetTerminalID(szTemplate);

        /* 列印端末機代號 */
        memset(szPrintBuf, 0x00, sizeof (szPrintBuf));
        //                sprintf(szPrintBuf, "端末機代號：%s", szTemplate);
        sprintf(szPrintBuf, "端末機代號：1234");
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);
    } else {
        /* 橫式 */
        //                memset(szTemplate, 0x00, sizeof(szTemplate));
        //                inGetMerchantID(szTemplate);

        /* 列印商店代號 */
        //                inFunc_PAD_ASCII(szTemplate, szTemplate, ' ', 15, _PADDING_LEFT_);
        sprintf(szPrintBuf, "商店代號 4567");
        //                sprintf(szPrintBuf, "商店代號 %s", szTemplate);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        if (inRetVal != VS_SUCCESS)
            return (VS_ERROR);

        /* Get端末機代號 */
        //                memset(szTemplate, 0x00, sizeof(szTemplate));
        //                inGetTerminalID(szTemplate);

        /* 列印端末機代號 */
        //                inFunc_PAD_ASCII(szTemplate, szTemplate, ' ', 13, _PADDING_LEFT_);
        sprintf(szPrintBuf, "端末機代號 121314151617");
        //                sprintf(szPrintBuf, "端末機代號 %s", szTemplate);
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
    char szPrintBuf[100 + 1], szPrintBuf1[42 + 1], szPrintBuf2[42 + 1], szTemplate1[42 + 1], szTemplate2[42 + 1];
    char szProductCodeEnable[1 + 1];
    char szStore_Stub_CardNo_Truncate_Enable[2 + 1];
    char szExamBit[10 + 1] = {0};
    char szCustomerIndicator[3 + 1] = {0};
    unsigned char uszChangeLineBit = VS_FALSE;

    memset(szCustomerIndicator, 0x00, sizeof (szCustomerIndicator));
    //inGetCustomIndicator(szCustomerIndicator);

    memset(szPrintBuf, 0x00, sizeof (szPrintBuf));
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
        sprintf(szPrintBuf, "卡別　　：%s", pobTran->srBRec.szCardLabel);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        memset(szPrintBuf, 0x00, sizeof (szPrintBuf));
        sprintf(szPrintBuf, "卡號　　：%s", pobTran->srBRec.szPAN);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        /*日期、時間*/
        memset(szPrintBuf, 0x00, sizeof (szPrintBuf));
        sprintf(szPrintBuf, "日期　　：%s", pobTran->srBRec.szDate);
        inRetVal = inPRINT_Buffer_PutIn(szPrintBuf, _PRT_DOUBLE_HEIGHT_WIDTH_, uszBuffer, srFont_Attrib, srBhandle, _LAST_ENTRY_, _PRINT_LEFT_);

        memset(szPrintBuf, 0x00, sizeof (szPrintBuf));
        sprintf(szPrintBuf, "時間　　：%s", pobTran->srBRec.szTime);
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

/*
Function	:inPRINT_Buffer_GetHeightFlow
Date&Time	:2016/3/16 下午 2:48
Describe	:決定要加入幾張圖，不return error是因為當有缺圖，其他圖仍能運作正常
 */
int inPRINT_Buffer_GetHeightFlow(void) {
    int inPreLen = 0;
    int inPathLen = 0;
    char szPath[100 + 1];
    char szFileName[50 + 1];
    /* 初始化結構 */
    memset(&gsrBMPHeight, 0x00, sizeof (BMPHeight));

    inPreLen = strlen("./fs_data/");

    /* 銀行LOGO高度 */
    memset(szPath, 0x00, sizeof (szPath));
    inPathLen = strlen(_BANK_LOGO_);
    memcpy(szPath, _BANK_LOGO_, inPathLen);

    memset(szFileName, 0x00, sizeof (szFileName));
    memcpy(szFileName, &szPath[inPreLen], inPathLen - inPreLen);

    inPRINT_Buffer_GetHeight((unsigned char*) szFileName, &gsrBMPHeight.inBankLogoHeight);

    /* 商店LOGO高度 */
    memset(szPath, 0x00, sizeof (szPath));
    inPathLen = strlen(_MERCHANT_LOGO_);
    memcpy(szPath, _MERCHANT_LOGO_, inPathLen);

    memset(szFileName, 0x00, sizeof (szFileName));
    memcpy(szFileName, &szPath[inPreLen], inPathLen - inPreLen);

    inPRINT_Buffer_GetHeight((unsigned char*) szFileName, &gsrBMPHeight.inMerchantLogoHeight);

    /* 表頭圖檔高度 */
    memset(szPath, 0x00, sizeof (szPath));
    inPathLen = strlen(_NAME_LOGO_);
    memcpy(szPath, _NAME_LOGO_, inPathLen);

    memset(szFileName, 0x00, sizeof (szFileName));
    memcpy(szFileName, &szPath[inPreLen], inPathLen - inPreLen);

    inPRINT_Buffer_GetHeight((unsigned char*) szFileName, &gsrBMPHeight.inTitleNameHeight);

    /* 企業標語高度 */
    memset(szPath, 0x00, sizeof (szPath));
    inPathLen = strlen(_SLOGAN_LOGO_);
    memcpy(szPath, _SLOGAN_LOGO_, inPathLen);

    memset(szFileName, 0x00, sizeof (szFileName));
    memcpy(szFileName, &szPath[inPreLen], inPathLen - inPreLen);

    inPRINT_Buffer_GetHeight((unsigned char*) szFileName, &gsrBMPHeight.inSloganHeight);

    /* CUP警語高度 */
    memset(szPath, 0x00, sizeof (szPath));
    inPathLen = strlen(_CUP_LEGAL_LOGO_);
    memcpy(szPath, _CUP_LEGAL_LOGO_, inPathLen);

    memset(szFileName, 0x00, sizeof (szFileName));
    memcpy(szFileName, &szPath[inPreLen], inPathLen - inPreLen);

    inPRINT_Buffer_GetHeight((unsigned char*) szFileName, &gsrBMPHeight.inCupLegalHeight);

    /* 分期警語高度 */
    memset(szPath, 0x00, sizeof (szPath));
    inPathLen = strlen(_LEGAL_LOGO_);
    memcpy(szPath, _LEGAL_LOGO_, inPathLen);

    memset(szFileName, 0x00, sizeof (szFileName));
    memcpy(szFileName, &szPath[inPreLen], inPathLen - inPreLen);

    inPRINT_Buffer_GetHeight((unsigned char*) szFileName, &gsrBMPHeight.inInstHeight);

    /* 商店提示與高度*/
    memset(szPath, 0x00, sizeof (szPath));
    inPathLen = strlen(_NOTICE_LOGO_);
    memcpy(szPath, _NOTICE_LOGO_, inPathLen);

    memset(szFileName, 0x00, sizeof (szFileName));
    memcpy(szFileName, &szPath[inPreLen], inPathLen - inPreLen);

    inPRINT_Buffer_GetHeight((unsigned char*) szFileName, &gsrBMPHeight.inNoticeHeight);
    return (VS_SUCCESS);
}

/*
Function        :inFunc_Booting_Flow_Print_Image_Initial
Date&Time       :2018/6/5 下午 6:27
Describe        :開機流程列印圖片初始化，為了避免開機更新導致抓不到圖片高度
 */
int inFunc_Booting_Flow_Print_Image_Initial(TRANSACTION_OBJECT *pobTran) {
    /* 若沒下TMS會沒圖片抓高度，所以不判斷成功或失敗 */
    inPRINT_Buffer_GetHeightFlow();
    return (VS_SUCCESS);
}

int inSetTermIPAddress(char* szTermIPAddress) {
    memset(srEDCRec.szTermIPAddress, 0x00, sizeof (srEDCRec.szTermIPAddress));
    /* 傳進的指標 不得為空  長度需大於0 小於規定最大值 */
    if (szTermIPAddress == NULL || strlen(szTermIPAddress) < 0 || strlen(szTermIPAddress) > 15) {
        printf("inSetTermIPAddress failed \n");
        return (VS_ERROR);
    }
    memcpy(&srEDCRec.szTermIPAddress[0], &szTermIPAddress[0], strlen(szTermIPAddress));

    return (VS_SUCCESS);
}

int inSetTermMASKAddress(char* szTermMASKAddress) {
    memset(srEDCRec.szTermMASKAddress, 0x00, sizeof (srEDCRec.szTermMASKAddress));
    /* 傳進的指標 不得為空  長度需大於0 小於規定最大值 */
    if (szTermMASKAddress == NULL || strlen(szTermMASKAddress) < 0 || strlen(szTermMASKAddress) > 15) {
        printf("inSetTermMASKAddress failed\n");
        return (VS_ERROR);
    }
    memcpy(&srEDCRec.szTermMASKAddress[0], &szTermMASKAddress[0], strlen(szTermMASKAddress));

    return (VS_SUCCESS);
}

int inSetTermGetewayAddress(char* szTermGetewayAddress) {
    memset(srEDCRec.szTermGetewayAddress, 0x00, sizeof (srEDCRec.szTermGetewayAddress));
    /* 傳進的指標 不得為空  長度需大於0 小於規定最大值 */
    if (szTermGetewayAddress == NULL || strlen(szTermGetewayAddress) < 0 || strlen(szTermGetewayAddress) > 15) {
        printf("inSetTermGetewayAddress failed\n");
        return (VS_ERROR);
    }
    memcpy(&srEDCRec.szTermGetewayAddress[0], &szTermGetewayAddress[0], strlen(szTermGetewayAddress));

    return (VS_SUCCESS);
}

int main(int argc, char *argv[]) {
    BYTE key;
    CTOS_LCDTClearDisplay();
    int  inRetVal = 0;
    unsigned char uszBuffer[PB_CANVAS_X_SIZE * 8 * _BUFFER_MAX_LINE_];
    BufferHandle srBhandle;
    FONT_ATTRIB srFont_Attrib;
    TRANSACTION_OBJECT pobTran;
//    inFunc_ls("-R -l", _AP_ROOT_PATH_);
    
    BYTE uszPackBuf[984];
    BYTE uszRecvPacket[_NCCC_ATS_ISO_SEND_ + 1];
    int  inReceiveTimeout = 10;
    int	 inReceiveSize = _COMM_RECEIVE_MAX_LENGTH_;
    
    memset(uszPackBuf,0x00,sizeof(uszPackBuf));
    
//    int inCnt = myPackData(uszPackBuf);
//    int i;
//    for (i=0;i<inCnt;i++)
//    {
//        printf("0x%02X\t",uszPackBuf[i]);
//    }
//    inRetVal = inETHERNET_Initial();
//    if(inRetVal == VS_SUCCESS)
//    {
//        printf("inETHERNET_Initial successed\n");
//        if(inETHERNET_SetConfig() == VS_SUCCESS)
//        {
//            /*
//             * 原先傳到主機的長度對不起來，原因是inCnt已包含電文前面的長度
//             * 但inETHERNET_Send預設傳入inSendSize 是未包含Message Length
//             * 為了不改動用到此function，故先將inSendSize -2
//             */
//            inETHERNET_Send(uszPackBuf,inCnt-2,0);
//            inReceiveSize = inETHERNET_Receive(uszRecvPacket,inReceiveSize,inReceiveTimeout);
//
//            if(inReceiveSize > 0 )
//            {
//                myUnPackData( uszRecvPacket , inReceiveSize);
//            }
//        }
//        if(inETHERNET_END() == VS_SUCCESS)
//            printf("socket disconnect successed!!\n");
//        else
//            printf("socket disconnect failed!!\n");
//    }
    ECR_TABLE			gsrECROb = {.srSetting.uszComPort = d_COM2};
    if(inECR_Initial() == VS_SUCCESS)
    {
//        CTOS_LCDTPrintXY(1, 1, "inECR_Initial");
//        CTOS_KBDGet(&key);
        inRS232_ECR_8N1_Standard_Receive_Packet(&pobTran, &gsrECROb);
         
        if (inRS232_Close(gsrECROb.srSetting.uszComPort) != VS_SUCCESS)
        {
            printf("Close the RS232 port  Failed\n");
        }
        
        CTOS_KBDGet(&key);
    }


//    EthernetPing(hostIp);
    
//    inFunc_Booting_Flow_Print_Image_Initial(&pobTran);
//    printf("初始化圖片預設高度\n");
//    printf("%s:inBankLogoHeight is %d \n",     _BANK_LOGO_,      gsrBMPHeight.inBankLogoHeight);
//    printf("%s:inMerchantLogoHeight is %d \n", _MERCHANT_LOGO_,  gsrBMPHeight.inMerchantLogoHeight);
//    printf("%s:inTitleNameHeight is %d \n",    _NAME_LOGO_,      gsrBMPHeight.inTitleNameHeight);

   
    
    /*============印表機設定============*/
     CTOS_PrinterFontSelectMode(d_FONT_TTF_MODE);
    //    CTOS_FontTTFSelectFontFile(d_FONT_DEVICE_PRINTER, "ca_default.ttf", 0);
    CTOS_FontTTFSelectFontFile(d_FONT_DEVICE_PRINTER, "tsuyuan.ttf", 0);
    CTOS_FontTTFSelectStyle(d_FONT_DEVICE_PRINTER, d_FONT_STYLE_NORMAL);
    /*============印表機設定============*/

    inPRINT_Buffer_Initial(uszBuffer, _BUFFER_MAX_LINE_, &srFont_Attrib, &srBhandle);
    
    
    /*============列印一筆文字簽單============*/
//    CTOS_PrinterPutString("Print API Test");
//    CTOS_PrinterPutString("1測試1----");
//    if ((inRetVal = inPRINT_Buffer_OutPut(uszBuffer, &srBhandle)) != VS_SUCCESS)
//        printf("inPRINT_Buffer_OutPut failed, ret=%d\n",inRetVal);
    /*============列印一筆文字簽單============*/
    
//    pobTran.srBRec.inPrintOption = _PRT_CUST_;
//    strcpy(pobTran.srBRec.szCardLabel, "9中9文9字9"); //卡別
//    strcpy(pobTran.srBRec.szPAN, "252500001616"); //卡號
//    strcpy(pobTran.srBRec.szDate, "1223"); //日期
//    strcpy(pobTran.srBRec.szTime, "143059"); //時間
//    pobTran.srBRec.lnOrgInvNum = 2;
//    pobTran.srBRec.lnBatchNum = 3;
//    strcpy(pobTran.srBRec.szAuthCode, "123456");
//    strcpy(pobTran.srBRec.szRefNo, "999999");
//    if ((inRetVal = inCREDIT_PRINT_Logo_ByBuffer(&pobTran, uszBuffer, &srFont_Attrib, &srBhandle)) != VS_SUCCESS)
//        printf("inCREDIT_PRINT_Logo_ByBuffer failed, ret=%d\n",inRetVal);
//    if ((inRetVal = inCREDIT_PRINT_Tidmid_ByBuffer(&pobTran, uszBuffer, &srFont_Attrib, &srBhandle)) != VS_SUCCESS)
//        printf("inCREDIT_PRINT_Tidmid_ByBuffer failed, ret=%d\n",inRetVal);
//    if ((inRetVal = inCREDIT_PRINT_Data_ByBuffer(&pobTran, uszBuffer, &srFont_Attrib, &srBhandle)) != VS_SUCCESS)
//        printf("inCREDIT_PRINT_Data_ByBuffer failed, ret=%d\n",inRetVal);
//    if ((inRetVal = inPRINT_Buffer_OutPut(uszBuffer, &srBhandle)) != VS_SUCCESS)
//        printf("inPRINT_Buffer_OutPut failed, ret=%d\n",inRetVal);

    exit(0);
}
