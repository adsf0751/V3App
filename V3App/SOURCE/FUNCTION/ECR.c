#include "ECR.h"

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))
#define BYTE unsigned char
extern	int			ginDebug;
extern	int			ginISODebug;
extern	int			ginDisplayDebug;
extern	int			ginECR_ResponseFd;
extern	int			ginMachineType;
extern	int			ginFindRunTime;
extern	unsigned char		guszCTLSInitiOK;
//extern	NCCC_TMK_COMMAND	gsrTMKdata;
BYTE				gbBarCodeECRBit = VS_FALSE;	/* 是否為掃碼交易規格 */
BYTE				gbEIECRBit = VS_FALSE;          /* 是否為電子發票交易規格 */
BYTE				gbECR_UDP_TransBit = VS_FALSE;
ECR_TABLE			gsrECROb = {};
EI_TABLE			gsrEIOb = {};
FPG_FTC_REC			gsrFPG_FTC_Rec = {};

/*
Function        :inECR_Initial
Date&Time       :2016/6/20 下午 2:55
Describe        :開機initial
*/
int inECR_Initial(void)
{
	int	inRetVal = VS_ERROR;
	char	szTMSOK[2 + 1] = {0};
	char	szECREnable[2 + 1] = {0};
	char	szECRVersion[2 + 1] = {0};
	char	szComPort[4 + 1] = {0};
	char	szDebugMsg[100 + 1] = {0};
	char	szTemplate[20 + 1] = {0};
	
	/* 檢查是否做過【參數下載】 */
//	memset(szTMSOK, 0x00, sizeof(szTMSOK));
//	inGetTMSOK(szTMSOK);
//	if (szTMSOK[0] != 'Y')
//	{
//		DISPLAY_OBJECT	srDispMsgObj;
//		memset(&srDispMsgObj, 0x00, sizeof(srDispMsgObj));
//		strcpy(srDispMsgObj.szDispPic1Name, _ERR_TMS_DWL_);
//		srDispMsgObj.inDispPic1YPosition = _COORDINATE_Y_LINE_8_6_;
//		srDispMsgObj.inMsgType = _CLEAR_KEY_MSG_;
//		srDispMsgObj.inTimeout = _EDC_TIMEOUT_;
//		strcpy(srDispMsgObj.szErrMsg1, "");
//		srDispMsgObj.inErrMsgLine1 = 0;
//		inDISP_Msg_BMP(_ERR_TMS_DWL_, _COORDINATE_Y_LINE_8_6_, _CLEAR_KEY_MSG_, _EDC_TIMEOUT_, "", 0);
		
//		return (VS_SUCCESS);
//	}
	
	/* 檢查是否有支援收銀機連線 */
//	memset(szECREnable, 0x00, sizeof(szECREnable));
//	inGetECREnable(szECREnable);
//	if (szECREnable[0] != 'Y')
//	{
//		return (VS_SUCCESS);
//	}
	
//	memset(&gsrECROb, 0x00, sizeof(ECR_TABLE));
//	/* 設定 ECR 版本 */
//	memset(szECRVersion, 0x00, sizeof(szECRVersion));
//	inGetECRVersion(szECRVersion);
	gsrECROb.srSetting.inVersion = 0;

//        vdUtility_SYSFIN_LogMessage(AT, "ECR Init ECRVerson :%d", gsrECROb.srSetting.inVersion);
	
	/* UDP和RS232並存，要分開寫 */
//	memset(szTemplate, 0x00, sizeof(szTemplate));
//	inGetSupECR_UDP(szTemplate);
//	if (memcmp(szTemplate, "Y", 1) == 0)
//	{
//		inUDP_ECR_Initial();
//	}
//	
//	memset(szComPort, 0x00, sizeof(szComPort));
//	inGetECRComPort(szComPort);
//	if (memcmp(szComPort, _COMPORT_COM1_, strlen(_COMPORT_COM1_)) == 0	||
//	    memcmp(szComPort, _COMPORT_COM2_, strlen(_COMPORT_COM2_)) == 0	||
//	    memcmp(szComPort, _COMPORT_COM3_, strlen(_COMPORT_COM3_)) == 0	||
//	    memcmp(szComPort, _COMPORT_COM4_, strlen(_COMPORT_COM4_)) == 0)
//	{
//		inRetVal = inRS232_ECR_Initial();
                inRetVal = inRS232_ECR_8N1_Standard_Initial(&gsrECROb);
//	}
//	else if (memcmp(szComPort, _COMPORT_USB1_, strlen(_COMPORT_USB1_)) == 0)
//	{
//		inRetVal = inUSB_ECR_Initial();
//	}
//	else if (memcmp(szComPort, _COMPORT_USB2_, strlen(_COMPORT_USB2_)) == 0)
//	{
//		inRetVal = inBaseUSB_ECR_Initial();
//	}
//	else if (memcmp(szComPort, _COMPORT_WIFI_, strlen(_COMPORT_WIFI_)) == 0)
//	{
//		inRetVal = inWiFi_ECR_Initial();
//	}
//	else
//	{
//		inRetVal = VS_ERROR;
//	}
	
	/* 根據ECRVersion來決定，COMPORT的設定 */
	if (inRetVal != VS_SUCCESS)
	{
            printf("Set ComPort:%d Failed\n",gsrECROb.srSetting.uszComPort +1);
//		inDISP_ClearAll();
//		DISPLAY_OBJECT	srDispMsgObj;
//		memset(&srDispMsgObj, 0x00, sizeof(srDispMsgObj));
//		strcpy(srDispMsgObj.szDispPic1Name, _ERR_INIT_);
//		srDispMsgObj.inDispPic1YPosition = _COORDINATE_Y_LINE_8_6_;
//		srDispMsgObj.inMsgType = _CLEAR_KEY_MSG_;
//		srDispMsgObj.inTimeout = _EDC_TIMEOUT_;
//		strcpy(srDispMsgObj.szErrMsg1, "RS232");
//		srDispMsgObj.inErrMsg1Line = _LINE_8_5_;
//		srDispMsgObj.inBeepTimes = 1;
//		srDispMsgObj.inBeepInterval = 0;
//		inDISP_Msg_BMP(&srDispMsgObj);
	}
	else
	{
            printf("Set ComPort:%d Successed\n",gsrECROb.srSetting.uszComPort +1);
            /* 代表設定完成 */
		gsrECROb.srSetting.uszSettingOK = VS_TRUE;
	}
		
	return (VS_SUCCESS);
}
/*
Function        :inECR_Receive_Check
Date&Time       :2017/6/1 下午 6:00
Describe        :用來確認ECR是否發動交易
*/
int inECR_Receive_Check(unsigned short *usLen)
{
	int	inRetVal = VS_ERROR;
	char	szComPort[4 + 1] = {0};
	char	szSup_UDP[2 + 1] = {0};
	
//	memset(szSup_UDP, 0x00, sizeof(szSup_UDP));
//	inGetSupECR_UDP(szSup_UDP);
//	if (memcmp(szSup_UDP, "Y", 1) == 0)
//	{
//		inRetVal = inUDP_Data_Receive_Check(usLen);
//		if (inRetVal == VS_SUCCESS)
//		{
//			gbECR_UDP_TransBit = VS_TRUE;
//			return (inRetVal);
//		}
//	}
	
//	memset(szComPort, 0x00, sizeof(szComPort));
//	inGetECRComPort(szComPort);
//	if (memcmp(szComPort, _COMPORT_COM1_, strlen(_COMPORT_COM1_)) == 0	||
//	    memcmp(szComPort, _COMPORT_COM2_, strlen(_COMPORT_COM2_)) == 0	||
//	    memcmp(szComPort, _COMPORT_COM3_, strlen(_COMPORT_COM3_)) == 0	||
//	    memcmp(szComPort, _COMPORT_COM4_, strlen(_COMPORT_COM4_)) == 0)
//	{
//		/* 沒設定完成，不用檢查 */
//		if (gsrECROb.srSetting.uszSettingOK != VS_TRUE)
//		{
//			inRetVal = VS_ERROR;
//		}
//		else
//		{
                        inRetVal = inRS232_Data_Receive_Check(gsrECROb.srSetting.uszComPort, usLen);
//                        printf("inRS232_Data_Receive_Check   usLen is %x\n",usLen);
//		}
//	}
//	else if (memcmp(szComPort, _COMPORT_USB1_, strlen(_COMPORT_USB1_)) == 0)
//	{
//		/* 沒設定完成，不用檢查 */
//		if (gsrECROb.srSetting.uszSettingOK != VS_TRUE)
//		{
//			inRetVal = VS_ERROR;
//		}
//		else
//		{
//			inRetVal = inUSB_Data_Receive_Check(usLen);
//		}
//	}
//	else if (memcmp(szComPort, _COMPORT_USB2_, strlen(_COMPORT_USB2_)) == 0)
//	{
//		/* 沒設定完成，不用檢查 */
//		if (gsrECROb.srSetting.uszSettingOK != VS_TRUE)
//		{
//			inRetVal = VS_ERROR;
//		}
//		else
//		{
//			inRetVal = inBaseUSB_Data_Receive_Check(usLen);
//		}
//	}
//	else if (memcmp(szComPort, _COMPORT_WIFI_, strlen(_COMPORT_WIFI_)) == 0)
//	{
//		inRetVal = inWiFi_IsAccept();
//	}
//	else
//	{
//		inRetVal = VS_ERROR;
//	}
	
	return (inRetVal);
}
/*
Function        :inECR_Data_Receive
Date&Time       :2017/11/15 下午 3:51
Describe        :底層function分流
*/
int inECR_Data_Receive(unsigned char *uszReceBuff, unsigned short *usReceSize)
{
	int	inRetVal  = VS_ERROR;
        inRetVal = inRS232_Data_Receive(gsrECROb.srSetting.uszComPort, uszReceBuff, usReceSize);

	
	return (inRetVal);
}
/*
Function        :inECR_FlushTxBuffer
Date&Time       :2018/12/8 下午 3:10
Describe        :清Buffer用
*/
int inECR_FlushTxBuffer(void)
{
	int	inRetVal = VS_ERROR;
	inRetVal = inRS232_FlushTxBuffer(gsrECROb.srSetting.uszComPort);
	return (inRetVal);
}
/*
Function        :inECR_Send_Check
Date&Time       :2017/11/15 下午 5:35
Describe        :
*/
int inECR_Send_Check(void)
{
	int	inRetVal = VS_ERROR;
	char	szComPort[4 + 1];
//        printf("inECR_Send_Check,uszSettingOK is %d\n",gsrECROb.srSetting.uszSettingOK);
        /* 沒設定完成，不用檢查 */
        if (gsrECROb.srSetting.uszSettingOK != VS_TRUE)
        {
                inRetVal = VS_ERROR;
        }
        else
        {
                inRetVal = inRS232_Data_Send_Check(gsrECROb.srSetting.uszComPort);
        }
	
	return (inRetVal);
}

/*
Function        :inECR_Data_Send
Date&Time       :2017/11/15 下午 5:43
Describe        :分流
*/
int inECR_Data_Send(unsigned char *uszSendBuff, unsigned short usSendSize)
{
	int	inRetVal = VS_ERROR;
        inRetVal = inRS232_Data_Send(gsrECROb.srSetting.uszComPort, uszSendBuff, usSendSize);

	return (inRetVal);
}
/*
Function        :inECR_Send_ACKorNAK
Date&Time       :2017/11/15 下午 6:11
Describe        :送ACK OR NAK
*/
int inECR_Send_ACKorNAK (ECR_TABLE * srECROb, int inAckorNak)
{
	char		szDebugMsg[100 + 1] = {0};
	unsigned char	uszSendBuffer[2 + 1];
	unsigned short	usRetVal;
    	
	/* 清send Buffer */
	inECR_FlushTxBuffer();
	
	memset(uszSendBuffer, 0x00, sizeof(uszSendBuffer));
	
	inDISP_Timer_Start(_TIMER_NEXSYS_1_, 2);
	/* 檢查port是否已經準備好要送資料 */
        while (1)
	{
		/* 等TxReady*/
		if (inTimerGet(_TIMER_NEXSYS_1_) == VS_SUCCESS)
		{
                        memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                        sprintf(szDebugMsg, "ECR Send Timeout");
                        printf("%s\n",szDebugMsg);
                        printf("ECR Send Timeout\n");
			return (VS_TIMEOUT);
		}
		
		if (inECR_Send_Check() == VS_SUCCESS)
		{
			break;
		}
		else
		{
                    memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                    sprintf(szDebugMsg, "ECR Send Check Not OK");
                    printf("%s\n",szDebugMsg);                      
		}
	};
	
	if (inAckorNak == _ACK_)
	{	    
		/* 成功，回傳ACK */
		uszSendBuffer[0] = _ACK_;
		uszSendBuffer[1] = _ACK_;
		
		usRetVal = inECR_Data_Send(uszSendBuffer, 2);
		
		if (usRetVal != VS_SUCCESS)
		{
                        printf("Send ACK_ACK Not OK\n");
			return (VS_ERROR);
		}
		else
		{
			printf("ACK_ACK\n");
		}
        
	}	
	else if (inAckorNak == _NAK_)
	{
		/* 失敗，回傳NAK */
		uszSendBuffer[0] = _NAK_;
		uszSendBuffer[1] = _NAK_;
		
		usRetVal = inECR_Data_Send(uszSendBuffer, 2);
		
		if (usRetVal != VS_SUCCESS)
		{		
			printf("Send NAK NAK Not OK\n");
			return (VS_ERROR);
		}
		else
		{	
			printf("NAK_NAK\n");
		}
	
	}
	else
	{
                printf("Can't Send Neither ACK or NAK\n");
		return (VS_ERROR);
	}
        
        return (VS_SUCCESS);
}


/*
Function        :inECR_Receive
Date&Time       :2017/11/15 下午 3:14
Describe        :接收收銀機傳來的資料
*/
int inECR_Receive(TRANSACTION_OBJECT *pobTran, ECR_TABLE* srECROb, char *szDataBuffer, int inDataSize)
{
	int		i;
	int		inRetVal;
	int		inRetry = 0;							/* 目前已重試次數 */
	int		inRecMaxRetry = _ECR_RETRYTIMES_;				/* 最大重試次數 */
	char		szDebugMsg[1000 + 1];						/* DebugMsg */
	char		szCustomerIndicator[3 + 1] = {0};
	unsigned short	usReceiveBufferSize;						/* uszReceiveBuffer的長度，*/
	unsigned short	usOneSize = 1;							/* 一次只讀一個byte */
	unsigned short	usReceiveLen = 0;
	unsigned char	uszLRC;								/* LRC的值 */
	unsigned char	uszTempBuffer[1024 + 1];					/* 從 Buffer收到的片段 */
	unsigned char	uszReceiveBuffer[_ECR_BUFF_SIZE_];				/* 包含STX 和 ETX的原始電文 */
	unsigned char	uszSTX = VS_FALSE;						/* 是否收到STX */
	unsigned char	uszETX = VS_FALSE;						/* 是否收到ETX */
	unsigned char	uszCheckSpecialLenthBit = VS_FALSE;                             /* 是否已檢查為特殊長度 */
        unsigned char	uszCheckUnyBit = VS_FALSE;					/* 是否已檢查Indicator是否為Q */
        unsigned char	uszCheckEIBit = VS_FALSE;					/* 是否為電子發票 */
        char	szTemp[10 + 1];
	
	/* (需求單-109455)-Üny實體掃碼需求 by Russell 2021/9/28 下午 2:42 */
	gbBarCodeECRBit = VS_FALSE;
        gbEIECRBit = VS_FALSE;

	/* 設定Timeout */
	inRetVal = inDISP_Timer_Start(_ECR_RECEIVE_TIMER_, srECROb->srSetting.inTimeout);
	
	/* 初始化放收到資料的陣列 */
	memset(uszReceiveBuffer, 0x00, sizeof(uszReceiveBuffer));
	usReceiveBufferSize = 0;
	
	/* 客製化 */
//	memset(szCustomerIndicator, 0x00, sizeof(szCustomerIndicator));
//	inGetCustomIndicator(szCustomerIndicator);
	
	while(1)
//        while(usReceiveLen == 0)
	{
		memset(uszTempBuffer, 0x00, sizeof(uszTempBuffer));
		uszLRC = 0;
		
		/* 超過重試次數 */
		if (inRetry > inRecMaxRetry)
		{
                        printf("Retry too many times!\n");
			return (VS_ERROR);
		}
		
		/* 當Comport中有東西就開始分析 */
		while (usReceiveLen == 0)
		{
			/* 如果timeout就跳出去 */
			if (inTimerGet(_ECR_RECEIVE_TIMER_) == VS_SUCCESS)
			{
                                printf("_ECR_RECEIVE_TIMER_  TIMEOUT!\n");
				return (VS_TIMEOUT);
			}		
			inECR_Receive_Check(&usReceiveLen);
		}
		
                memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                sprintf(szDebugMsg, "Len : %d", usReceiveLen);
                printf("%s\n",szDebugMsg);


		/* 如果沒收到STX就會進這隻，直到分析出STX */
		while (uszSTX == VS_FALSE && usReceiveLen > 0)
		{
			/* 如果timeout就跳出去 */
			if (inTimerGet(_ECR_RECEIVE_TIMER_) == VS_SUCCESS)
			{
				return (VS_TIMEOUT);
			}
			
			/* 這邊一次只收一個byte */
			usOneSize = 1;
			memset(uszTempBuffer, 0x00, usOneSize);
			inRetVal = inECR_Data_Receive(uszTempBuffer, &usOneSize);
			if (inRetVal == VS_SUCCESS)
			{	           
				/* 避免長度過短的雜訊改到標準400，這邊只少要長度超過64才判斷是否轉換 */
//				if (usReceiveLen > 64)
//				{
//					/* 若為為7E1協定，且收到STX之後的第一個為"I"，則自動轉為8N1 (NCCC ECR特殊規定) */
//					if (inECR_NCCC_144_To_400(srECROb, uszTempBuffer) == VS_SUCCESS)
//					{
//						return (VS_ERROR);
//					}
//				}
				
				/* buffer讀出一個byte，長度減一 */
				usReceiveLen -= usOneSize;
//
				if (uszTempBuffer[0] == _STX_)
				{
					/* 收到STX */
					uszSTX = VS_TRUE;
                                        memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                                        sprintf(szDebugMsg, "Receive STX");
                                        printf("%s\n",szDebugMsg);
					memcpy(&uszReceiveBuffer[usReceiveBufferSize], &uszTempBuffer[0], 1);
					usReceiveBufferSize++;
					
					break;
				}
				else
				{
					/* 沒收到STX */
                                        memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                                        inFunc_BCD_to_ASCII(szDebugMsg, uszTempBuffer, usOneSize);
                                        printf("%s\n",szDebugMsg);
					/* 繼續收 */
					continue;
				}
			}

		}
                            
		/* 如果有收到STX，沒收到ETX就會進這隻，直到分析出ETX */			
		while (uszSTX == VS_TRUE && uszETX == VS_FALSE && usReceiveLen > 0)
		{
			/* 如果timeout就跳出去 */
			if (inTimerGet(_ECR_RECEIVE_TIMER_) == VS_SUCCESS)
			{
				return (VS_TIMEOUT);
			}			
			usOneSize = usReceiveLen;
			memset(uszTempBuffer, 0x00, usOneSize);
			inRetVal = inECR_Data_Receive(uszTempBuffer, &usOneSize);

			if (inRetVal == VS_SUCCESS)
			{       
				/* buffer讀出一個byte，長度減一 */
				memcpy(&uszReceiveBuffer[usReceiveBufferSize], uszTempBuffer, usOneSize);
				usReceiveLen -= usOneSize;
				usReceiveBufferSize += usOneSize;
				                                
                                if (usReceiveBufferSize >= 2)
				{ 
                                        /* (需求單-109455)-Üny實體掃碼需求 by Russell 2021/8/13 下午 4:10 */
                                        if (uszCheckUnyBit != VS_TRUE && uszCheckSpecialLenthBit != VS_TRUE)
                                        {
                                                uszCheckUnyBit = VS_TRUE;
                                                
                                                if (uszReceiveBuffer[1] == 'Q')
                                                {
                                                        uszCheckSpecialLenthBit = VS_TRUE;
                                                        gbBarCodeECRBit = VS_TRUE;
                                                        inDataSize = _ECR_8N1_1000_CUPQRCODE_Size_;
                                                }
                                        }
                                }

                                if (usReceiveBufferSize >= 11)
				{
                                        if (uszCheckEIBit != VS_TRUE && uszCheckSpecialLenthBit != VS_TRUE)
                                        {
                                                uszCheckEIBit = VS_TRUE;

                                                /* 電子發票交易別 "63" */
                                                memset(szTemp, 0x00, sizeof(szTemp));
                                                memcpy(&szTemp[0], &uszReceiveBuffer[9], 2);
                                                if (!memcmp(szTemp, _ECR_8N1_EI_TRANSACTION_, 2))
                                                {
                                                        uszCheckSpecialLenthBit = VS_TRUE;
                                                        gbEIECRBit = VS_TRUE;
                                                        inDataSize = _ECR_8N1_2044_EI_Receive_Size_;
                                                }
                                        }
				}
				/* 當已收到401byte 確認下個byte是不是ETX */
				if (usReceiveBufferSize >= (inDataSize + 3) - 1)
				{
					if (uszReceiveBuffer[(inDataSize + 3) - 2] == _ETX_)
					{	
						/* 是ETX */
						uszETX = VS_TRUE;
                                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                                                sprintf(szDebugMsg, "Receive ETX");
                                                printf("%s\n",szDebugMsg);

						
						if (usReceiveBufferSize < (inDataSize + 3))
						{
							/* 再收LRC */
							usOneSize = 1;
							memset(uszTempBuffer, 0x00, usOneSize);
							inRetVal = inECR_Data_Receive(&uszTempBuffer[0], &usOneSize);
							if (inRetVal == VS_SUCCESS)
							{
        							memcpy(&uszReceiveBuffer[usReceiveBufferSize], &uszTempBuffer[0], usOneSize);
								usReceiveLen -= usOneSize;
								usReceiveBufferSize += usOneSize;
							}
							else
							{
								/* 沒收到LRC 資料長度有誤*/
							}
						}
						
						break;
					}
					else
					{   
                                            /* 不是是ETX */
                                            printf("Not Receive ETX!\n");		
					}	
				}
			}
                    
		}

		/* 有收到STX、ETX而且收到長度也對，就開始算LRC */
		if (uszSTX == VS_TRUE && uszETX == VS_TRUE)
		{
			/* 資料長度錯誤，清Buffer，送NAK，重收 */
			if (usReceiveBufferSize < inDataSize + 3)
			{
                                memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                                sprintf(szDebugMsg, "DataLength error!:%d Retry: %d", usReceiveBufferSize, inRetry);
                                printf("%s\n",szDebugMsg);

//				if (srECROb->srSetting.inVersion == atoi(_ECR_RS232_VERSION_082_IKEA_))
//				{
//					inECR_SendError(pobTran, VS_TIMEOUT);
//				}
//				else
//				{
					/* 回傳NAK */
//					inECR_Send_ACKorNAK(srECROb, _NAK_);
//				}
				/* retry次數+ 1 */
				inRetry ++;
				
				/* 初始化資料 */
				memset(uszReceiveBuffer, 0x00, sizeof(uszReceiveBuffer));
				usReceiveBufferSize = 0;
				uszSTX = VS_FALSE;
				uszETX = VS_FALSE;
				
				continue;
			}
			else if (usReceiveBufferSize > inDataSize + 3)
			{
//				if (ginDebug == VS_TRUE)
//				{
					memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
					sprintf(szDebugMsg, "DataLength :%d", usReceiveBufferSize);
                                        printf("%s\n",szDebugMsg);
//					inLogPrintf(AT, szDebugMsg);
//				}
			}
			
			/* 計算收到DATA的LRC （Stx Not Include）*/
			for (i = 1; i < ((inDataSize + 3) - 1); i++)
			{
				uszLRC ^= uszReceiveBuffer[i];
			}
//			printf("Calculate LRC: %02x,%02x\n", uszLRC,uszReceiveBuffer[((inDataSize + 3) - 1)]);
			if (uszReceiveBuffer[((inDataSize + 3) - 1)] == uszLRC)
			{
				if (srECROb->srOptionalSetting.uszNotSendAck == VS_TRUE)
				{
					
				}
				else
				{
					/* 比對收到的LRC是否正確，若正確回傳ACK */
					inECR_Send_ACKorNAK(srECROb, _ACK_);
				}
				
				break;
			}
			else
			{
				/* 比對失敗 */
//				if (ginDebug == VS_TRUE)
//				{
//					memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
//					sprintf(szDebugMsg, "LRC error! Retry: %d", inRetry);
//					inLogPrintf(AT, szDebugMsg);
//					
//					inLogPrintf(AT, "CountLRC");
//					memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//					sprintf(szDebugMsg, "%X", uszLRC);
//					inLogPrintf(AT, szDebugMsg);
//					
//					inLogPrintf(AT, "DataSendLRC");
//					memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//					sprintf(&szDebugMsg[0], "%X", uszReceiveBuffer[((inDataSize + 3) - 1)]);
//					inLogPrintf(AT, szDebugMsg);
//				}
//				if (ginDisplayDebug == VS_TRUE)
//				{
//					memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//					sprintf(szDebugMsg, "LRC error! Retry: %d", inRetry);
//					inDISP_LOGDisplay(szDebugMsg, _FONTSIZE_16X22_, _LINE_16_3_, VS_TRUE);
//				}
				
				/* 若錯誤回傳NAK */
				inECR_Send_ACKorNAK(srECROb, _NAK_);
				/* retry次數+ 1 */
				inRetry ++;
				
				/* 初始化資料 */
				memset(uszReceiveBuffer, 0x00, sizeof(uszReceiveBuffer));
				usReceiveBufferSize = 0;
				uszSTX = VS_FALSE;
				uszETX = VS_FALSE;	
				continue;
			}		
		}/* 算LRC END */		
	}/* Receive END */
	
//	/* 去除STX、ETX、LRC，把資料放到szrRealReceBuffer */
	memcpy(szDataBuffer, &uszReceiveBuffer[1], inDataSize);
	
//	/* 列印紙本電文和顯示電文訊息 */
//	inECR_Print_Receive_ISODeBug(szDataBuffer, usReceiveBufferSize - 3, inDataSize);
	
	return (VS_SUCCESS);
}