
#include "Function.h" //FOR _PADDING_RIGHT_ /_PADDING_LEFT_
//extern BMPHeight gsrBMPHeight;

/*
Function        :inFunc_Amount_Comma
Date&Time       :2016/9/5 下午 4:03
Describe        :
 *szAmt:	金額
 *szCurSymbol:	金額的符號 ex: ＄、 ￥
 *szPad_char:	要Pad的字元
 *inSigned:	如果有需要印出負0的需求
 *inWide:	最後字串的寬度
 *inAlign:	右靠左靠，True的話，右靠左補空白；False的話，左靠右補空白
 *注意:		若靠右，且pad 0x00會抓不到字串長度
*/
int inFunc_Amount_Comma(char *szAmt, char *szCurSymbol, char szPad_char, int inSigned, int inWide, int inAlign)
{
	int	inOffset = 0;		
	int	inNumberLen = 0;	/* 已放進szComma的數字數量 */
        int	inLen, inPoint = 0, i;	/* inPoint:現在szComma的長度 */
        int     inFinalLen;
        char	szComma[48 + 1];
	char	szTemplate[48 + 1];
	char	szUnsignedAmt[20 + 1];
      
	
	/* 輸入數字是負數 */
        memset(szUnsignedAmt, 0x00, sizeof(szUnsignedAmt));
	if (szAmt[0] == '-')
		strcpy(szUnsignedAmt, &szAmt[1]);
	else
		strcpy(szUnsignedAmt, &szAmt[0]);
	
	inLen = strlen(szUnsignedAmt);
	inOffset = inLen % 3;
	memset(szComma, 0x00, sizeof(szComma));
	
	for (i = 0; i < inLen; i ++)
	{
		szComma[inPoint ++] = szUnsignedAmt[i];
		inNumberLen ++;			/* 數字長度 */
		
		/* 每第三個數字加comma ，若已經是最後一位也不加comma */
		if (((inNumberLen - inOffset) % 3 == 0) &&
		     (i != inLen - 1))
			szComma[inPoint ++] = 0x2C; /* 補【,】 */
	}
        
	
	
	memset(szTemplate, 0x00, sizeof(szTemplate));
	
	/* Flag有On或是原傳入金額為負 */
	if (inSigned == _SIGNED_MINUS_ || szAmt[0] == '-')
		sprintf(szTemplate, "-%s", szComma);
	else
		sprintf(szTemplate, "%s", szComma);

        inFinalLen = strlen(szCurSymbol) + strlen(szTemplate);
        
        if (inFinalLen > inWide)
        {    
                /* 已經超出inWide，直接丟append */
        }
        else
        {    
                /* 補空白 */
                if (inAlign == _PADDING_LEFT_)
                        inFunc_PAD_ASCII(szTemplate, szTemplate, szPad_char, inWide - strlen(szCurSymbol), _PADDING_LEFT_);	/* 右靠左補空白 */
                else
                        inFunc_PAD_ASCII(szTemplate, szTemplate, szPad_char, inWide - strlen(szCurSymbol), _PADDING_RIGHT_);	/* 左靠右補空白 */
	}
        
	/* 金額符號 */
	if (strlen(szCurSymbol) != 0)
		sprintf(szAmt, "%s%s", szCurSymbol, szTemplate);
	else
		sprintf(szAmt, "%s", szTemplate);

	return (VS_SUCCESS);
}
/*
Function        :inFunc_SyncPobTran_Date_Include_Year
Date&Time       :2018/2/8 上午 9:40
Describe        :將傳進的RTC的時間部份，傳進放進來的Buffer,因為只傳pointer，所以要求傳進長度避免爆掉
 *		:這個會連20一起填
*/
int inFunc_SyncPobTran_Date_Include_Year(char *szDate,int inTimeBufferLen, RTC_NEXSYS *srRTC)
{	
	if (inTimeBufferLen >= 8 + 1)
	{
		sprintf(szDate, "20%02d%02d%02d",  srRTC->uszYear, srRTC->uszMonth, srRTC->uszDay);
		return (VS_SUCCESS);
	}
	else
	{
		return (VS_ERROR);
	}
}
/*
Function        :inFunc_SyncPobTran_Time
Date&Time       :2018/2/8 上午 9:40
Describe        :將傳進的RTC的時間部份，傳進放進來的Buffer,因為只傳pointer，所以要求傳進長度避免爆掉
*/
int inFunc_SyncPobTran_Time(char *szTime,int inTimeBufferLen, RTC_NEXSYS *srRTC)
{	
	if (inTimeBufferLen >= 6 + 1)
	{
		sprintf(szTime, "%02d%02d%02d",  srRTC->uszHour, srRTC->uszMinute, srRTC->uszSecond);
		return (VS_SUCCESS);
	}
	else
	{
		return (VS_ERROR);
	}
}
/*
Function        :inFunc_Sync_BRec_Date_Time
Date&Time       :2018/2/8 上午 9:44
Describe        :同步BRec日期和時間
*/
int inFunc_Sync_BRec_Date_Time(TRANSACTION_OBJECT *pobTran, RTC_NEXSYS *srRTC)
{	
	/* 同步到pobTran */
	memset(pobTran->srBRec.szDate, 0x00, sizeof(pobTran->srBRec.szDate));
	memset(pobTran->srBRec.szOrgDate, 0x00, sizeof(pobTran->srBRec.szOrgDate));
	memset(pobTran->srBRec.szTime, 0x00, sizeof(pobTran->srBRec.szTime));
	memset(pobTran->srBRec.szOrgTime, 0x00, sizeof(pobTran->srBRec.szOrgTime));
	
	if (inFunc_SyncPobTran_Date_Include_Year(pobTran->srBRec.szDate, sizeof(pobTran->srBRec.szDate),  srRTC) != VS_SUCCESS)
	{
		return (VS_ERROR);
	}
	
	if (inFunc_SyncPobTran_Date_Include_Year(pobTran->srBRec.szOrgDate, sizeof(pobTran->srBRec.szOrgDate),  srRTC) != VS_SUCCESS)
	{
		return (VS_ERROR);
	}
	
	if (inFunc_SyncPobTran_Time(pobTran->srBRec.szTime, sizeof(pobTran->srBRec.szTime),  srRTC) != VS_SUCCESS)
	{
		return (VS_ERROR);
	}
	
	if (inFunc_SyncPobTran_Time(pobTran->srBRec.szOrgTime, sizeof(pobTran->srBRec.szOrgTime),  srRTC) != VS_SUCCESS)
	{
		return (VS_ERROR);
	}
	
	return (VS_SUCCESS);
}

/*
Function	:inFunc_ASCII_to_BCD
Date&Time	:2015/7/13 下午 2:02
Describe        :ASCII轉BCD，inLength指要轉換成的byte數(ASCII字串的1/2長度)
		注意:BCD的空間長度至少要為ASCII的1/2倍，否則會溢位。(2byte ascii可轉換成1byte bcd)
*/
int inFunc_ASCII_to_BCD(unsigned char *uszBCD, char *szASCII, int inLength)
{
        unsigned char uszTemplate = 0;
        int	i;

        /* 防呆 inLength須大於零 */
        if (inLength <= 0)
                return (VS_ERROR);

        for (i = 0; i < inLength ; i ++)
        {
                /* 字元做運算 */
                switch (*szASCII)
                {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                                uszTemplate = *szASCII - '0';
                                break;
                        case 'A':
                        case 'B':
                        case 'C':
                        case 'D':
                        case 'E':
                        case 'F':
                                uszTemplate = *szASCII - 'A' + 10;
                                break;
                        case 'a':
                        case 'b':
                        case 'c':
                        case 'd':
                        case 'e':
                        case 'f':
                                uszTemplate = *szASCII - 'a' + 10;
                                break;
                        default:
                                break;
                }

                *uszBCD = (unsigned char) (uszTemplate << 4); /* 向左移動四位元 */
                szASCII ++;

                switch (*szASCII)
                {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                                uszTemplate = *szASCII - '0';
                                break;
                        case 'A':
                        case 'B':
                        case 'C':
                        case 'D':
                        case 'E':
                        case 'F':
                                uszTemplate = *szASCII - 'A' + 10;
                                break;
                        case 'a':
                        case 'b':
                        case 'c':
                        case 'd':
                        case 'e':
                        case 'f':
                                uszTemplate = *szASCII - 'a' + 10;
                                break;
                        default:
                                break;
                }

                szASCII ++;
                *uszBCD = *uszBCD | uszTemplate;
                uszBCD ++;
        }

        return (VS_SUCCESS);
}
/*
Function	:inFunc_BCD_to_ASCII
Date&Time	:2015/7/13 下午 2:02
Describe	:BCD轉ASCII 注意:ASCII的空間長度要為BCD的兩倍，否則會溢位。
*/
int inFunc_BCD_to_ASCII(char *szASCII, unsigned char *uszBCD, int inLength)
{
	unsigned char uszTemplate, uszNibble;
        int i;

	for(i = 0; i < inLength; ++i)
	{
		uszTemplate = *uszBCD++;
                uszNibble = (uszTemplate / 16);         /* 第一個字  */

                /* 16進位A = 10進位的10，做運算 */
                if (uszNibble < 10)
                {
                    *szASCII = (char)('0' + uszNibble);
                    szASCII++;
		}
                else                                    /* A~F */
                {
                    *szASCII = (char)('A' + (uszNibble - 10));
                    szASCII++;
                }

                uszNibble = (uszTemplate % 16);         /* 第二個字  */

                if (uszNibble < 10)
                {
                        *szASCII = (char)('0' + uszNibble);
                        szASCII++;
		}
                else
                {
                        *szASCII = (char)('A' + (uszNibble - 10));
                        szASCII++;
                }
	}

	/* 不要補0比較好用 */
//	*szASCII = 0;

	return (VS_SUCCESS);
}
int inFunc_ShellCommand_System(char *szCommand) {
    int inRetVal;
    char szDebugMsg[100 + 1];
    if (szCommand == NULL) {
        return (VS_ERROR);
    }
    inRetVal = system(szCommand);
    /* 在C 程式裡其中 ret 值要除以256才會得到與shell 傳回相符的值 */
    inRetVal >>= 8;

    if (inRetVal != 0) {
        return (VS_ERROR);
    }
    return (VS_SUCCESS);
}
int inFunc_ls(char* szParameter1, char* szSource) {
    int inRetVal = VS_SUCCESS;
    char szDebugMsg[100 + 1] = {0};
    char szCommand[100 + 1] = {0}; /* Shell Command*/

    /* 組命令 */
    if (strlen(szSource) != 0) {
        memset(szCommand, 0x00, sizeof (szCommand));
        sprintf(szCommand, "ls ");

        if (strlen(szParameter1) != 0) {
            strcat(szCommand, szParameter1);
            strcat(szCommand, " ");
        }

        if (strlen(szSource) != 0) {
            strcat(szCommand, szSource);
        }
    }/* 沒有來源檔 */
    else {
        return (VS_ERROR);
    }
    //printf("szCommand is %s\n",szCommand);
    /* 執行命令 */
    inRetVal = inFunc_ShellCommand_System(szCommand);
    if (inRetVal != VS_SUCCESS) {
        return (inRetVal);
    }

    return (VS_SUCCESS);
}

/*
Function	:inFunc_PAD_ASCII
Date&Time	:2015/6/24 下午 4:14
Describe	:靠左靠右補字或空白
 *注意		:若靠右，且pad 0x00會抓不到字串長度，建議不要包含中文字串，中文字串會算三個字元
*/
int inFunc_PAD_ASCII(char *szStr_out, char *szStr_in, char szPad_char, int inPad_size, int inAlign)
{
        int inPADlen = 0;
        char szTemplate[256 + 1];

        /* 第一步:計算要補多少字元 */
        inPADlen = inPad_size - strlen(szStr_in);

        /* 第二步:靠左靠右 */
	if (inPADlen > 0)
	{
                /* 將szTemplate初始化成要補的字元 */
                memset(szTemplate, szPad_char, sizeof(szTemplate));

        	if (inAlign == _PADDING_LEFT_)
                        memcpy(&szTemplate[inPADlen], &szStr_in[0], inPad_size); /* 字靠右 */
                else if (inAlign == _PADDING_RIGHT_)
                        memcpy(&szTemplate[0], &szStr_in[0], strlen(szStr_in)); /* 字靠左 */

                /* 補結束字元 & 傳回szStr_out */
                szTemplate[inPad_size] = '\0';
                memset(szStr_out, 0x00, sizeof(szStr_out));
                strcpy(szStr_out, szTemplate);
	}
        else if (inPADlen == 0)
        {
                /* inPADlen = 0 的狀況，szStr_in將字串給szStr_out */
                memset(szTemplate, 0x00, sizeof(szTemplate));
                memcpy(&szTemplate[0], &szStr_in[0], strlen(szStr_in));
                memset(szStr_out, 0x00, sizeof(szStr_out));
                strcpy(szStr_out, szTemplate);
        }
        else
        {
                /* inPADlen < 0 清空szStr_out Return Error */
                memset(szStr_out, 0x00, sizeof(szStr_out));	
                return(VS_ERROR);
        }

	return (VS_SUCCESS);
}

/*
Function	:inPRINT_Buffer_GetHeightFlow
Date&Time	:2016/3/16 下午 2:48
Describe	:決定要加入幾張圖，不return error是因為當有缺圖，其他圖仍能運作正常
 */
int inPRINT_Buffer_GetHeightFlow(BMPHeight* gsrBMPHeight) {
    int inPreLen = 0;
    int inPathLen = 0;
    char szPath[100 + 1];
    char szFileName[50 + 1];
    /* 初始化結構 */
    memset(gsrBMPHeight, 0x00, sizeof (BMPHeight));

    inPreLen = strlen("./fs_data/");

    /* 銀行LOGO高度 */
    memset(szPath, 0x00, sizeof (szPath));
    inPathLen = strlen(_BANK_LOGO_);
    memcpy(szPath, _BANK_LOGO_, inPathLen);

    memset(szFileName, 0x00, sizeof (szFileName));
    memcpy(szFileName, &szPath[inPreLen], inPathLen - inPreLen);

    inPRINT_Buffer_GetHeight((unsigned char*) szFileName, &gsrBMPHeight->inBankLogoHeight);
    
    /* 商店LOGO高度 */
    memset(szPath, 0x00, sizeof (szPath));
    inPathLen = strlen(_MERCHANT_LOGO_);
    memcpy(szPath, _MERCHANT_LOGO_, inPathLen);

    memset(szFileName, 0x00, sizeof (szFileName));
    memcpy(szFileName, &szPath[inPreLen], inPathLen - inPreLen);

    inPRINT_Buffer_GetHeight((unsigned char*) szFileName, &gsrBMPHeight->inMerchantLogoHeight);

    /* 表頭圖檔高度 */
    memset(szPath, 0x00, sizeof (szPath));
    inPathLen = strlen(_NAME_LOGO_);
    memcpy(szPath, _NAME_LOGO_, inPathLen);

    memset(szFileName, 0x00, sizeof (szFileName));
    memcpy(szFileName, &szPath[inPreLen], inPathLen - inPreLen);

    inPRINT_Buffer_GetHeight((unsigned char*) szFileName, &gsrBMPHeight->inTitleNameHeight);

    /* 企業標語高度 */
//    memset(szPath, 0x00, sizeof (szPath));
//    inPathLen = strlen(_SLOGAN_LOGO_);
//    memcpy(szPath, _SLOGAN_LOGO_, inPathLen);
//
//    memset(szFileName, 0x00, sizeof (szFileName));
//    memcpy(szFileName, &szPath[inPreLen], inPathLen - inPreLen);
//
//    inPRINT_Buffer_GetHeight((unsigned char*) szFileName, &gsrBMPHeight->inSloganHeight);

    /* CUP警語高度 */
//    memset(szPath, 0x00, sizeof (szPath));
//    inPathLen = strlen(_CUP_LEGAL_LOGO_);
//    memcpy(szPath, _CUP_LEGAL_LOGO_, inPathLen);
//
//    memset(szFileName, 0x00, sizeof (szFileName));
//    memcpy(szFileName, &szPath[inPreLen], inPathLen - inPreLen);
//
//    inPRINT_Buffer_GetHeight((unsigned char*) szFileName, &gsrBMPHeight->inCupLegalHeight);

    /* 分期警語高度 */
//    memset(szPath, 0x00, sizeof (szPath));
//    inPathLen = strlen(_LEGAL_LOGO_);
//    memcpy(szPath, _LEGAL_LOGO_, inPathLen);
//
//    memset(szFileName, 0x00, sizeof (szFileName));
//    memcpy(szFileName, &szPath[inPreLen], inPathLen - inPreLen);
//
//    inPRINT_Buffer_GetHeight((unsigned char*) szFileName, &gsrBMPHeight->inInstHeight);

    /* 商店提示與高度*/
//    memset(szPath, 0x00, sizeof (szPath));
//    inPathLen = strlen(_NOTICE_LOGO_);
//    memcpy(szPath, _NOTICE_LOGO_, inPathLen);
//
//    memset(szFileName, 0x00, sizeof (szFileName));
//    memcpy(szFileName, &szPath[inPreLen], inPathLen - inPreLen);
//
//    inPRINT_Buffer_GetHeight((unsigned char*) szFileName, &gsrBMPHeight->inNoticeHeight);
    return (VS_SUCCESS);
}