#include "RS232.h"
MY_ECR_DATA myECRTable[43] = {
    {"ECR Indicator"                                     , 1, 1  ,NULL},
    {"ECR Version Date"                                  , 6, 2  ,NULL},
    {"Trans Type Indicator"                              , 1, 8  ,NULL},
    {"Trans Type"                                        , 2, 9  ,NULL},
    {"CUP / Smart pay / ESVC Indicator "                 , 1, 11 ,NULL},
    {"Host ID"                                           , 2, 12 ,NULL},
    {"Receipt No"                                        , 6, 14 ,NULL},
    {"Card No"                                           , 19,20 ,NULL},
    {"Card Expire Date"                                  , 4 ,39 ,NULL},
    {"Trans Amount"                                      , 12, 43,NULL},
    {"Trans Date"                                        , 6, 55 ,NULL},
    {"Trans Time"                                        , 6, 61 ,NULL},
    {"Approval No"                                       , 9, 67 ,NULL},
    {"Wave Card"                                         , 1, 76 ,NULL},
    {"ECR Response Code"                                 , 4, 77 ,NULL},
    {"Merchant ID"                                       , 15, 81,NULL},
    {"Terminal ID "                                      , 8, 96 ,NULL},
    {"Exp Amount "                                       , 12,104,NULL},
    {"Store Id "                                         , 18,116,NULL},
    {"Installment/Redeem Indicator "                     , 1, 134,NULL},
    {"RDM Paid Amt "                                     , 12,135,NULL},
    {"RDM Point"                                         , 8, 147,NULL},
    {"Points of Balance"                                 , 8, 155,NULL},
    {"Redeem Amt "                                       , 12,163,NULL},
    {"Installment Period"                                , 2, 175,NULL},
    {"Down Payment Amount/ESVC Balance before Tx. "      , 12,177,NULL},
    {"Installment Payment Amount/ESVC Balance after Tx." , 12,189,NULL},
    {"Formality Fee/ESVC Autoload Amount "               , 12,201,NULL},
    {"Card Type"                                         , 2, 213,NULL},
    {"Batch No"                                          , 6, 215,NULL},
    {"Start Trans Type"                                  , 2, 221,NULL},
    {"MP Flag"                                           , 1, 223,NULL},
    {"SP ISSUER ID"                                      , 8, 224,NULL},
    {"SP / 信用卡/ESVC Origin Date "                     , 8, 232,NULL},
    {"SP Origin RRN / 信用卡OriginRRN/ATS電票交易序號 "  ,12, 240,NULL},
    {"Pay Item"                                          , 5, 252,NULL},
    {"Card No. Hash Value"                               , 50,257,NULL},
    {"MP Response Code"                                  , 6, 307,NULL},
    {"ASM Award flag"                                    , 1, 313,NULL},
    {"MCP Indicator"                                     , 1, 314,NULL},
    {"金融機構代碼 "                                     , 3, 315,NULL},
    {"Reserved"                                          , 5, 318,NULL},
    {"HG Data"                                           , 78,323,NULL},
};

/*
Function        :inRS232_FlushRxBuffer
Date&Time       :2016/6/21 上午 10:54
Describe        :清空buffer中的資料
*/
int inRS232_FlushRxBuffer(unsigned char uszComPort)
{
	char		szDebugMsg[100 + 1];
	unsigned short	usRetVal;
	
	/* 沒設定完成，不用檢查 */
//	if (gsrECROb.srSetting.uszSettingOK != VS_TRUE)
//	{
//		return (VS_ERROR);
//	}
	
	/* 清空接收的buffer */
	usRetVal = CTOS_RS232FlushRxBuffer(uszComPort);
	if (usRetVal != d_OK)
	{
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "CTOS_RS232FlushRxBuffer Error: 0x%04x", usRetVal);
            printf("%s\n",szDebugMsg);
            return (VS_ERROR);
	}
	else
	{
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "CTOS_RS232FlushTxBuffer Successs ComPort:%d", uszComPort + 1);
            printf("%s\n",szDebugMsg);
		
            return (VS_SUCCESS);
	}
	       
}
/*
Function        :inRS232_Open
Date&Time       :2017/7/13 上午 11:54
Describe        :
*/
int inRS232_Open(unsigned char uszComport, unsigned long ulBaudRate, unsigned char uszParity, unsigned char uszDataBits, unsigned char uszStopBits)
{
	char		szDebugMsg[100 + 1];
	unsigned short	usRetVal;
	
	usRetVal = CTOS_RS232Open(uszComport, ulBaudRate, uszParity, uszDataBits, uszStopBits);
	if (usRetVal != d_OK)
	{
                memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                sprintf(szDebugMsg, "CTOS_RS232Open Error: 0x%04x", usRetVal);
                printf("%s\n",szDebugMsg);
                memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                sprintf(szDebugMsg, "COM%d BaudRate:%lu %d%c%d", uszComport + 1, ulBaudRate, uszDataBits, uszParity, uszStopBits);
                printf("%s\n",szDebugMsg);
		
		return (VS_ERROR);
	}
	else
	{
            memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
            sprintf(szDebugMsg, "COM%d BaudRate:%lu %d%c%d", uszComport, ulBaudRate, uszDataBits, uszParity, uszStopBits);
            printf("%s\n",szDebugMsg);
		
            return (VS_SUCCESS);
	}
	
}

/*
Function        :inRS232_ECR_8N1_Standard_Initial
Date&Time       :2016/6/20 下午 2:55
Describe        :initial 8N1 COM PORT
*/
int inRS232_ECR_8N1_Standard_Initial(ECR_TABLE *srECROb)
{
	char		szDebugMsg[100 + 1];
	char		szECRComPort[4 + 1];
	unsigned char	uszParity;
	unsigned char	uszDataBits;
	unsigned char	uszStopBits;
	unsigned long	ulBaudRate;
	unsigned short	usRetVal;
	
	memset(&uszParity  , 0x00, sizeof(uszParity));
	memset(&uszDataBits, 0x00, sizeof(uszDataBits));
	memset(&uszStopBits, 0x00, sizeof(uszStopBits));
	memset(&ulBaudRate , 0x00, sizeof(ulBaudRate));
	
	/* Verifone用handle紀錄，Castle用Port紀錄 */
        //d_COM1 相當於機器上的com port 而非裝置管理員取得的com port 
	srECROb->srSetting.uszComPort = d_COM2; 	
	/* BaudRate = 9600 */
	ulBaudRate = 9600;
	
	/* Parity */
	uszParity = 'N';
	
	/* Data Bits */
	uszDataBits = 8;
	
	/* Stop Bits */
	uszStopBits = 1;
	
	
        /* 開port */
	/* Portable 機型若沒接上底座再開Ethernet會失敗 */
	usRetVal = inRS232_Open(srECROb->srSetting.uszComPort, ulBaudRate, uszParity, uszDataBits, uszStopBits);

	if (usRetVal != VS_SUCCESS)
	{
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "inRS232_Open Error: 0x%04x", usRetVal);
            printf("%s\n",szDebugMsg);
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "COM%d BaudRate:%lu %d%c%d", srECROb->srSetting.uszComPort + 1, ulBaudRate, uszDataBits, uszParity, uszStopBits);
            printf("%s\n",szDebugMsg);
            return (VS_ERROR);         
	}
	else
	{
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "COM%d BaudRate:%lu %d%c%d", srECROb->srSetting.uszComPort, ulBaudRate, uszDataBits, uszParity, uszStopBits);
            printf("%s\n",szDebugMsg);
	}
        
	/* 清空接收的buffer */
        inRS232_FlushRxBuffer(srECROb->srSetting.uszComPort);
	
	srECROb->srSetting.inCustomerLen = _ECR_8N1_Standard_Data_Size_;
	
        return (VS_SUCCESS);
}

/*
Function        :inRS232_ECR_8N1_Standard_Receive_Packet
Date&Time       :2016/7/6 下午 4:04
Describe        :處理收銀機傳來的資料
*/
int inRS232_ECR_8N1_Standard_Receive_Packet(TRANSACTION_OBJECT *pobTran, ECR_TABLE * srECROb)
{
	int	inRetVal;
	char	szDataBuffer[_ECR_RS232_BUFF_SIZE_];	/* 電文不包含STX和LRC */
	memset(&szDataBuffer, 0x00, sizeof(szDataBuffer));
        
        
/* -----------------------開始接收資料------------------------------------------ */
//        if (pobTran->uszIsTradeBit)
//                srECROb->srSetting.inTimeout = _ECR_RECEIVE_EI_TIMEOUT_;
//        else
                srECROb->srSetting.inTimeout = _ECR_RECEIVE_REQUEST_TIMEOUT_;
        
	inRetVal = inECR_Receive(pobTran, srECROb, szDataBuffer, _ECR_8N1_Standard_Data_Size_);

	printf("Insert data into the table columns... \n");
        int i;
        int szECRTable = sizeof(myECRTable) / sizeof(myECRTable[0]);
        for(i= 0; i<szECRTable ;i++)
        {
            myECRTable[i].Data = malloc(myECRTable[i].Length+1);
            int idx =  myECRTable[i].Idx;
            int len =  myECRTable[i].Length;
            memcpy(myECRTable[i].Data,&szDataBuffer[idx-1],len);
            myECRTable[i].Data[len] = '\0';
        }

	if (inRetVal != VS_SUCCESS)
	{
		return (inRetVal);
	}
	
/* -----------------------開始分析資料------------------------------------------ */
//	inRetVal = inECR_8N1_Standard_Unpack(pobTran, srECROb, szDataBuffer);
//	
//	if (inRetVal != VS_SUCCESS)
//	{
//		if (pobTran->inECRErrorMsg != _ECR_RESPONSE_CODE_NOT_SET_ERROR_)
//		{
//			inECR_SendError(pobTran, pobTran->inECRErrorMsg);
//		}
//		else
//		{
//			inECR_SendError(pobTran, inRetVal);
//		}
//		return (inRetVal);
//	}
//	
	
        return (VS_SUCCESS);
}
/*
Function        :inRS232_ECR_8N1_Standard_Send_Packet
Date&Time       :2016/7/11 下午 3:29
Describe        :處理要送給收銀機的資料
*/
int inRS232_ECR_8N1_Standard_Send_Packet(TRANSACTION_OBJECT *pobTran, ECR_TABLE * srECROb)
{
	int	inRetVal = VS_ERROR;
	int	inSendSize = 0;
	char	szDataBuf[_ECR_RS232_BUFF_SIZE_] = {0};	/* 封包資料 */
	
	/* 如果已經回過ECR就不再回 */
//	if (srECROb->srTransData.uszIsResponse == VS_TRUE)
//		return (VS_SUCCESS);
	
	/* 初始化 */
	memset(szDataBuf, 0x00, sizeof(szDataBuf));
	/* (需求單-109455)-Üny實體掃碼需求 by Russell 2021/9/10 上午 10:32 */
//	if (gbBarCodeECRBit == VS_TRUE)
//	{
//		inSendSize = _ECR_8N1_1000_CUPQRCODE_Size_;
//	}
//	else
//	{
		inSendSize = _ECR_8N1_Standard_Data_Size_;
//	}
/* ---------------------包裝電文--------------------------------------------- */
//	if (memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INQUIRY_LAST_TRANSACTION_, strlen(_ECR_8N1_INQUIRY_LAST_TRANSACTION_)) == 0)
//	{
//		inRetVal = inECR_8N1_Inquiry_Last_Transaction_Pack(pobTran, srECROb, szDataBuf);
//		
//		/* (需求單-111155)-電子錢包業者收付訊息整合平台需求 by Russell 2023/8/24 下午 5:40 */
//		/* "註8.	當收銀機發動’查詢前筆交易別狀態’予EDC，EDC查詢前筆交易為電子錢包(不論成功或失敗交易)，EDC須回覆’0010’予收銀機。" */
//		if (szDataBuf[10] == 'W')
//		{
//			memcpy(&szDataBuf[76], "0010", 4);
//		}
//	}
//	else
//	{
		inRetVal = inECR_8N1_Standard_Pack(pobTran, srECROb, szDataBuf);
//	}
	
	if (inRetVal == VS_ERROR)
	{
		return (VS_ERROR);
	}
	
//	if (memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INQUIRY_LAST_TRANSACTION_, strlen(_ECR_8N1_INQUIRY_LAST_TRANSACTION_)) != 0)
//	{
//		inECR_Save_Response(szDataBuf, inSendSize);
//	}
/* ---------------------傳送電文--------------------------------------------- */
	inRetVal = inECR_Send(pobTran, srECROb, szDataBuf, inSendSize);
	/* 標示已送給ECR回覆電文 */
	srECROb->srTransData.uszIsResponse = VS_TRUE;
	
	if (inRetVal != VS_SUCCESS)
	{
		return (inRetVal);
	}
	
        return (VS_SUCCESS);
}


/*
Function        :inRS232_Data_Receive_Check
Date&Time       :2016/6/20 下午 6:02
Describe        :RS232有收到東西，就觸發事件
*/
int inRS232_Data_Receive_Check(unsigned char uszComPort, unsigned short *usReceiveLen)
{
	char		szDebugMsg[100 + 1];
	unsigned short	usRetVal;
	
	/* 檢查資料是否可以從port被取得，看是否有收到資料長度，若有資料，*usReceiveLen會被放入資料長度 */
        usRetVal = CTOS_RS232RxReady(uszComPort, usReceiveLen);
	if (usRetVal == d_OK)
	{
//            printf("RS232 is ready to Rx\n");
	}
	/* 接底座若離開會跑這一個 */
	else if (usRetVal == d_ETHERNET_HARDWARE_ERROR)
	{
            printf("d_ETHERNET_HARDWARE_ERROR\n");
	}
	else
	{
            memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
            sprintf(szDebugMsg, "CTOS_RS232RxReady : 0x%04X", usRetVal);
            printf("%s\n",szDebugMsg);

		/* Rx not ready 有可能沒開comport或其他原因 */
                return (VS_ERROR);         
        }
	
	/* 有收到資料就回傳成功 */
	if (*usReceiveLen > 0)	
		return (VS_SUCCESS);
	else
		return (VS_ERROR);
	
}

/*
Function        :inRS232_Data_Receive
Date&Time       :2016/7/6 下午 3:28
Describe        :RS232收東西API
*/
int inRS232_Data_Receive(unsigned char uszComPort, unsigned char *uszReceBuff, unsigned short *usReceSize)
{
	int	inRetVal;
	
	inRetVal = CTOS_RS232RxData(uszComPort, uszReceBuff, usReceSize);
	if (inRetVal == d_OK)
	{
                return (VS_SUCCESS);
	}
	else
	{
		return (VS_ERROR);
	}
}

/*
Function        :inRS232_Close
Date&Time       :2017/7/13 下午 1:50
Describe        :
*/
int inRS232_Close(unsigned char uszComport)
{
	char		szDebugMsg[100 + 1];
	unsigned short	usRetVal;
	
	usRetVal = CTOS_RS232Close(uszComport);
	if (usRetVal != d_OK)
	{
//		if (ginDebug == VS_TRUE)
//		{
			memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
			sprintf(szDebugMsg, "CTOS_RS232Close Error: 0x%04x", usRetVal);
//			inLogPrintf(AT, szDebugMsg);
			memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
			sprintf(szDebugMsg, "COM%d", uszComport + 1);
                        printf("%s\n",szDebugMsg);
//			inLogPrintf(AT, szDebugMsg);
//		}
		return (VS_ERROR);
	}
	else
	{
//		if (ginDebug == VS_TRUE)
//		{
//		    inLogPrintf(AT, "CTOS_RS232Close OK");
		    memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
		    sprintf(szDebugMsg, "Close ComPort:%d Successed\n", uszComport + 1);
                    printf("%s\n",szDebugMsg);
//		    inLogPrintf(AT, szDebugMsg);
//		}
		return (VS_SUCCESS);
	}
	
}
/*
Function        :inRS232_FlushTxBuffer
Date&Time       :2017/6/26 下午 5:15
Describe        :清空Txbuffer中的資料
*/
int inRS232_FlushTxBuffer(unsigned char uszComPort)
{
	char		szDebugMsg[100 + 1];
	unsigned short	usRetVal;
//	printf("inRS232_FlushTxBuffer,uszSettingOK is %d\n",uszComPort);
	/* 沒設定完成，不用檢查 */
//	if (gsrECROb.srSetting.uszSettingOK != VS_TRUE)
//	{
//		return (VS_ERROR);
//	}
	
	/* 清空接收的buffer */
	usRetVal = CTOS_RS232FlushTxBuffer(uszComPort);
	if (usRetVal != d_OK)
	{
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "CTOS_RS232FlushTxBuffer Error: 0x%04x", usRetVal);
                printf("%s\n",szDebugMsg);
		return (VS_ERROR);
	}
	else
	{
                /* 收到的資料長度歸0 */
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "CTOS_RS232FlushTxBuffer Successs ComPort:%d", uszComPort + 1);
                printf("%s\n",szDebugMsg);
                return (VS_SUCCESS);
	}
	       
}
/*
Function        :inRS232_Data_Send_Check
Date&Time       :2016/6/20 下午 6:02
Describe        :檢查是否IO忙碌，可以傳送東西
*/
int inRS232_Data_Send_Check(unsigned char uszComPort)
{
	unsigned short	usRetVal;
	
	/* 檢查是否IO忙碌，可以傳送東西 */
        usRetVal = CTOS_RS232TxReady(uszComPort);
	if (usRetVal != d_OK)
	{
		/* Rx not ready 有可能沒開comport或其他原因 */
                return (VS_ERROR);         
        }
	else
        {
//            printf("RS232 is ready to Rx\n");
            return (VS_SUCCESS);
        }
		
}

/*
Function        :inRS232_Data_Send
Date&Time       :2016/7/11 下午 2:37
Describe        :RS232送東西API
*/
int inRS232_Data_Send(unsigned char uszComPort, unsigned char *uszSendBuff, unsigned short usSendSize)
{
	int	inRetVal;
	char	szDebugMsg[100 + 1];
	
	inRS232_FlushTxBuffer(uszComPort);
	
	inRetVal = CTOS_RS232TxData(uszComPort, uszSendBuff, usSendSize);
	if (inRetVal == d_OK)
	{
		return (VS_SUCCESS);
	}
	else
	{
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "CTOS_RS232TxData Error: 0x%04x", inRetVal);
		printf("%s\n",szDebugMsg);
		return (VS_ERROR);
	}
}