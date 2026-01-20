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
Function        :inECR_FlushRxBuffer
Date&Time       :2017/6/1 下午 5:54
Describe        :清Buffer用
*/
int inECR_FlushRxBuffer(void)
{
	int	inRetVal = VS_ERROR;
        inRetVal = inRS232_FlushRxBuffer(gsrECROb.srSetting.uszComPort);
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
Function        :inECR_Receive_ACKandNAK
Date&Time       :2017/11/15 下午 5:50
Describe        :
*/
int inECR_Receive_ACKandNAK(ECR_TABLE * srECROb)
{
	int		i = 0;
	int		inRetVal = VS_SUCCESS;
	int		inACKTimeout = 0;
	char		szDebugMsg[(_ECR_BUFF_SIZE_ * 2) + 1] = {0};
	char		szCustomerIndicator[3 + 1] = {0};
	unsigned char	uszBuf[_ECR_BUFF_SIZE_] = {0};
	unsigned short	usTwoSize = 2;
	unsigned short	usReceiveLen = 0;
	
//	memset(szCustomerIndicator, 0x00, sizeof(szCustomerIndicator));
//	inGetCustomIndicator(szCustomerIndicator);
//	
//	if (!memcmp(szCustomerIndicator, _CUSTOMER_INDICATOR_082_IKEA_, _CUSTOMER_INDICATOR_SIZE_)	||
//	    !memcmp(szCustomerIndicator, _CUSTOMER_INDICATOR_123_IKEA_, _CUSTOMER_INDICATOR_SIZE_))
//	{
//		if (srECROb->srSetting.inVersion == atoi(_ECR_RS232_VERSION_082_IKEA_) ||
//		    srECROb->srSetting.inUDPVersion == atoi(_ECR_UDP_VERSION_082_IKEA_))
//		{
//			inACKTimeout = _ECR_RECEIVE_ACK_TIMEOUT_144_;
//		}
//		else
//		{
//			/* 設定Timeout */
//			if (gbBarCodeECRBit == VS_TRUE)
//			{
//				inACKTimeout = _ECR_RECEIVE_ACK_TIMEOUT_LONG_;
//			}
//			else
//			{
//				inACKTimeout = _ECR_RECEIVE_ACK_TIMEOUT_NORMAL_;
//			}
//		}
//	}
//	else
//	{
//		/* 設定Timeout */
//		if (gbBarCodeECRBit == VS_TRUE)
//		{
//			inACKTimeout = _ECR_RECEIVE_ACK_TIMEOUT_LONG_;
//		}
//		else
//		{
			inACKTimeout = _ECR_RECEIVE_ACK_TIMEOUT_NORMAL_;
//		}
//	}
//	
//	if (ginDebug == VS_TRUE)
//	{
//		inLogPrintf(AT, "ACK Receive Timeout:%d(10ms)", inACKTimeout);
//	}
	inRetVal = inDISP_Timer_Start_MicroSecond(_TIMER_NEXSYS_2_, inACKTimeout);
	
	/* 客製化005可以只收一個ACK */
//	if (1)
//	{
//		while (1)
//		{
//			memset(uszBuf, 0x00, sizeof(uszBuf));
//			/* 當COMPort中有東西就開始分析 */
//			while (usReceiveLen == 0)
//			{
//				inECR_Receive_Check(&usReceiveLen);
//
//				/* 如果timeout就跳出去 */
//				if (inTimerGet(_TIMER_NEXSYS_2_) == VS_SUCCESS)
//				{
//					return (VS_TIMEOUT);
//				}
//			}
//
//			if (ginDebug == VS_TRUE)
//			{
//				memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
//				sprintf(szDebugMsg, "Len : %d", usReceiveLen);
//				inLogPrintf(AT, szDebugMsg);
//			}
//
//			while (usReceiveLen > 0)
//			{
//				/* 如果timeout就跳出去 */
//				if (inTimerGet(_TIMER_NEXSYS_2_) == VS_SUCCESS)
//				{
//					return (VS_TIMEOUT);
//				}
//
//				usTwoSize = usReceiveLen;
//				inRetVal = inECR_Data_Receive(&uszBuf[0], &usTwoSize);
//
//				if (inRetVal == VS_SUCCESS)
//				{
//					if (ginDebug == VS_TRUE)
//					{
//						memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
//						inFunc_BCD_to_ASCII(szDebugMsg, uszBuf, usTwoSize);
//						inLogPrintf(AT, szDebugMsg);
//					}
//					
//					i = 0;
//					do
//					{
//						if (usReceiveLen == 0)
//						{
//							break;
//						}
//						
//						/* buffer讀出1個byte，長度減1 */
//						usReceiveLen -= 1;
//
//						/* 判斷收到資料是否為ACK */	
//						if (uszBuf[i] == _ACK_)
//						{ 
//							if (ginDebug == VS_TRUE)
//							{
//								inLogPrintf(AT, "Receive ACK!");
//							}
//
//							return (_ACK_);				
//						}
//						/* 判斷收到資料是否為NAK */
//						else if (uszBuf[i] == _NAK_)
//						{	
//							if (ginDebug == VS_TRUE)
//							{
//								inLogPrintf(AT, "Receive NAK!");
//							}
//
//							return (_NAK_);
//						}
//						else 
//						{
//							i++;
//							if (ginDebug == VS_TRUE)
//							{
//								inLogPrintf(AT, "Receive Not Ack Neither NAK!");
//							}
//							continue;
//						}
//					}
//					while(1);
//
//				}/*  */
//
//			}/* while (usReceiveLen > 0) (有收到資料) */
//
//		}/* while(1)...... */
//	}
//	else
//	{
		while (1)
		{
			memset(uszBuf, 0x00, sizeof(uszBuf));
			/* 當COMPort中有東西就開始分析 */
			while (usReceiveLen == 0)
			{
				inECR_Receive_Check(&usReceiveLen);

				/* 如果timeout就跳出去 */
				if (inTimerGet(_TIMER_NEXSYS_2_) == VS_SUCCESS)
				{
					return (VS_TIMEOUT);
				}
			}

//			if (ginDebug == VS_TRUE)
//			{
				memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
				sprintf(szDebugMsg, "Len : %d", usReceiveLen);
                                printf("%s\n",szDebugMsg);
//				inLogPrintf(AT, szDebugMsg);
//			}

			while (usReceiveLen > 0)
			{
				/* 如果timeout就跳出去 */
				if (inTimerGet(_TIMER_NEXSYS_2_) == VS_SUCCESS)
				{
					return (VS_TIMEOUT);
				}

				/* 這邊一次只收1個byte */
				usTwoSize = 1;
				uszBuf[0] = uszBuf[1];
				inRetVal = inECR_Data_Receive(&uszBuf[1], &usTwoSize);

				if (inRetVal == VS_SUCCESS)
				{
					/* buffer讀出1個byte，長度減1 */
					usReceiveLen -= 1;

					/* 判斷收到資料是否為ACK */	
					if (uszBuf[0] == _ACK_ && uszBuf[1] == _ACK_)
					{ 
//						if (ginDebug == VS_TRUE)
//						{
//							inLogPrintf(AT, "Receive ACK!");
//						}
                                            printf("Receive ACK!\n");
						return (_ACK_);				
					}
					/* 判斷收到資料是否為NAK */
					else if (uszBuf[0] == _NAK_ && uszBuf[1] == _NAK_)
					{	
//						if (ginDebug == VS_TRUE)
//						{
//							inLogPrintf(AT, "Receive NAK!");
//						}
                                                printf("Receive NAK!\n");
						return (_NAK_);
					}
					else 
					{
//						if (ginDebug == VS_TRUE)
//						{
//							inLogPrintf(AT, "Receive Not Ack Neither NAK!");
//							memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
//							inFunc_BCD_to_ASCII(szDebugMsg, uszBuf, 2);
//							inLogPrintf(AT, szDebugMsg);
//						}
                                            printf("Receive Not Ack Neither NAK!\n");
						continue;
					}

				}/*  */

			}/* while (usReceiveLen > 0) (有收到資料) */

		}/* while(1)...... */
//	}
		
}
/*
Function        :inECR_Send
Date&Time       :2017/11/15 下午 5:21
Describe        :傳送要給收銀機的資料
*/
int inECR_Send(TRANSACTION_OBJECT *pobTran, ECR_TABLE * srECROb, char *szDataBuffer, int inDataSize)
{
	int		i = 0;
	int		inRetVal = VS_SUCCESS;
	int		inFinalRetVal = VS_ERROR;
	int		inTryTimes = 0;
	int		inTryTimesMax = _ECR_RETRYTIMES_;					/* 最大重試次數 */
	int		inSendLen = 0;
	int		inDataIndex = 0;
	int		inCheckTimeout = 0;
	char		szDebugMsg[100 +1] = {0};
	unsigned char	uszSendBuf[_ECR_BUFF_SIZE_];					/* 包含STX、ETX、LRC的電文 */
	unsigned char	uszLRC = 0;
	printf("inECR_Send() START !\n");
	
//	if (srECROb->srSetting.uszNonDefaultRetry == VS_TRUE)
//	{
//		inTryTimesMax = srECROb->srSetting.inNonDefaultRetryTimes + 1;
//	}
//	else
//	{
		inTryTimesMax = _ECR_RETRYTIMES_ + 1;
//	}
	printf("inTryTimesMax:%d", inTryTimesMax);
	
	/* 清send Buffer */
	inECR_FlushTxBuffer();

	/* Send之前清Buffer，避免收到錯的回應 */
	inECR_FlushRxBuffer();
	
//	if (gbECR_UDP_TransBit == VS_TRUE)
//	{
//		inDataIndex = 18;
//		uszSendBuf[inSendLen] = _SOH_;
//		inSendLen ++;
//		memcpy(&uszSendBuf[inSendLen], gsrECROb.srTransData.szPOSTxUniqueNo, 16);
//		inSendLen += 16;
//	}
//	else
//	{
		inDataIndex = 1;
//	}
	
	/* 在要傳送Buffer裡放STX */
	uszSendBuf[inSendLen] = _STX_;
	inSendLen ++;
	
	/* 把Data Buffer(純資料)裡所有0x00值改成0x20 */
	for (i = 0; i < inDataSize; i++)
	{
		if (szDataBuffer[i] == 0x00)
		{
			szDataBuffer[i] = 0x20;
		}
	}
	
	/* 在要傳送Buffer裡放要傳送的資料 */
	memcpy(&uszSendBuf[inSendLen], szDataBuffer, inDataSize);
	inSendLen += inDataSize;
	
	/* 在要傳送Buffer裡放ETX */
	uszSendBuf[inSendLen] = _ETX_;
	inSendLen ++;
	
	/* 運算LRC(STX Not include) */
	for (i = inDataIndex; i < (inSendLen); i++)
	{
		uszLRC ^= uszSendBuf[i];
	}

	/* 在要傳送Buffer裡放LRC */
	uszSendBuf[inSendLen] = uszLRC;
	inSendLen ++;
	
        memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
        sprintf(szDebugMsg, "LRC : 0x%02X", uszLRC);
        printf("%s\n",szDebugMsg);
	
//	if (gbECR_UDP_TransBit == VS_TRUE)
//	{
//		uszSendBuf[inSendLen] = _EOT_;
//		inSendLen ++;
//	}
	
	inFinalRetVal = VS_ERROR;
	for (inTryTimes = 0; inTryTimes < inTryTimesMax; )
	{
		inCheckTimeout = 2;
		inDISP_Timer_Start(_TIMER_NEXSYS_1_, inCheckTimeout);
		/* 檢查port是否已經準備好要送資料 */
		do
		{
			if (inTimerGet(_TIMER_NEXSYS_1_) == VS_SUCCESS)
			{

                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                                sprintf(szDebugMsg, "ECR Check Timeout");
                                printf("%s\n",szDebugMsg);	
				/* 即使Send Check不成功也嘗試送 */
				break;
			}
			
			/* 檢查port是否已經準備好要送資料 */	
			if (inECR_Send_Check() == VS_SUCCESS)
			{
                                printf("ECR_Send_Check OK\n");
				break;
			}
			else
			{
                            printf("ECR_Send_Check NOT OK\n");
			}
		}while(1);
		
		/* 經由port傳送資料 */
		inRetVal = inECR_Data_Send(uszSendBuf, (unsigned short)inSendLen);
		/* 不管是否成功，嘗試次數都 +1 */
		inTryTimes++;
		if (inRetVal != VS_SUCCESS)
		{
                        printf("ECR Data send fail\n");
			inFinalRetVal = VS_ERROR;
		}
		else
		{
			printf("ECR Data send success\n");
                        
//		/*---------------------------------- 印Debug 開始---------------------------------------------------------------*/
//			/* 列印紙本電文和顯示電文訊息 */
//			inECR_Print_Send_ISODeBug(szDataBuffer, inSendLen, inDataSize);
//		/*---------------------------------- 印Debug 結束---------------------------------------------------------------*/
//			if (srECROb->srOptionalSetting.uszNotReceiveAck == VS_TRUE)
//			{
//				/* 成功 */
//				if (ginDebug == VS_TRUE)
//				{
//					inLogPrintf(AT, "ECR NO NEED ACK OK!");
//				}
//				if (ginDisplayDebug == VS_TRUE)
//				{
//					memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//					sprintf(szDebugMsg, "ECR NO NEED ACK OK!");
//					inDISP_LOGDisplay(szDebugMsg, _FONTSIZE_16X22_, _LINE_16_2_, VS_FALSE);
//				}
//				inFinalRetVal = VS_SUCCESS;
//				break;
//			}
//			else
//			{
//				/* 接收ACK OR NAK */
				inRetVal = inECR_Receive_ACKandNAK(srECROb);

				/* 超過一秒沒收到回應 */
				if (inRetVal == VS_TIMEOUT)
				{
//					if (ginDebug == VS_TRUE)
//					{
//						inLogPrintf(AT, "Not Receive Response,Need Retry");
//					}
//					if (ginDisplayDebug == VS_TRUE)
//					{
//						memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//						sprintf(szDebugMsg, "Not Receive Response,Need Retry");
//						inDISP_LOGDisplay(szDebugMsg, _FONTSIZE_16X22_, _LINE_16_2_, VS_FALSE);
//					}
                                    printf("VS_TIMEOUT\n");
					inFinalRetVal = VS_ERROR;
				}
//				/* 收到NAK */
				else if (inRetVal == _NAK_)
				{
//					if (ginDebug == VS_TRUE)
//					{
//						inLogPrintf(AT, "Receive NAK,Need Retry");
//					}
//					if (ginDisplayDebug == VS_TRUE)
//					{
//						memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//						sprintf(szDebugMsg, "Receive NAK,Need Retry");
//						inDISP_LOGDisplay(szDebugMsg, _FONTSIZE_16X22_, _LINE_16_2_, VS_FALSE);
//					}
                                    printf("Receive NAK,Need Retry\n");
					inFinalRetVal = VS_ERROR;
				}
//				/* 收到ACK */	
				else
				{	
//					/* 成功 */
//					if (ginDebug == VS_TRUE)
//					{
//						inLogPrintf(AT, "ECR ACK OK!");
//					}
//					if (ginDisplayDebug == VS_TRUE)
//					{
//						memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//						sprintf(szDebugMsg, "ECR ACK OK!");
//						inDISP_LOGDisplay(szDebugMsg, _FONTSIZE_16X22_, _LINE_16_2_, VS_FALSE);
//					}
                                    printf("ECR ACK OK!\n");
					inFinalRetVal = VS_SUCCESS;
					break;
				}
//			}/* 需要接收ACK的流程 */
		}/* 送ECR成功的判斷流程*/
		
		/* 超過最大重試次數，仍要完成交易，收銀機提示補登畫面 */
		/* MPAS要沒收到ACK要印簽單 */
		/* 傳送Retry */
		if (inTryTimes >= inTryTimesMax)
		{
                        printf("Exceed max retry times!\n");
			inFinalRetVal = VS_ERROR;
			break;
		}
	}
	if (inFinalRetVal != VS_SUCCESS)
	{
//		if (ginDebug == VS_TRUE)
//		{
//			inLogPrintf(AT, "inECR_Send()Error END !");
//			inLogPrintf(AT, "----------------------------------------");
//		}
            printf("inECR_Send()Error END !\n");
		return (VS_ERROR);
	}
	else
	{
//		if (ginDebug == VS_TRUE)
//		{
//			inLogPrintf(AT, "inECR_Send()Success END !");
//			inLogPrintf(AT, "----------------------------------------");
//		}
		 printf("inECR_Send()Success END !\n");
		return (VS_SUCCESS);
	}
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

/*
Function        :inRS232_ECR_8N1_Standard_Pack
Date&Time       :2016/7/6 下午 2:40
Describe        :先把要送的資料組好
*/
int inECR_8N1_Standard_Pack(TRANSACTION_OBJECT *pobTran, ECR_TABLE * srECROb, char *szDataBuffer)
{
	int	inRetVal = VS_ERROR;
	int	i = 0, inCardLen = 0;
	int	inPacketSizes = 0;
	int	inLen = 0;
	int	inBarCodeLen = 0;
	char	szTemplate[100 + 1];
	char	szHash[44 + 1];
	char	szFESMode[2 + 1];
	char	szTemp[8 + 1] = {0};
	char	szTemp2[8 + 1] = {0};
        char	szCustomerIndicator[3 + 1] = {0};

//        memset(szCustomerIndicator, 0x00, sizeof(szCustomerIndicator));
//        inGetCustomIndicator(szCustomerIndicator);
        memcpy( srECROb->srTransData.szECRIndicator,"I",1);
	/* ECR Indicator (1 Byte) */
	memcpy(&szDataBuffer[inPacketSizes], srECROb->srTransData.szECRIndicator, 1);
	inPacketSizes ++;
	/* ECR Version Date (6 Byte) */
	memcpy(&szDataBuffer[inPacketSizes], "260116", 6);
	inPacketSizes += 6;
	/* Trans Type Indicator (1 Byte) */
	inPacketSizes ++;
	/* Trans Type (2 Byte) */
//	memcpy(&szDataBuffer[inPacketSizes], srECROb->srTransData.szTransType, 2);
        memcpy(srECROb->srTransData.szTransType,_ECR_8N1_SALE_,strlen(_ECR_8N1_SALE_));
	memcpy(&szDataBuffer[inPacketSizes], srECROb->srTransData.szTransType, 2);
	inPacketSizes += 2;
	/* CUP Indicator (1 Byte) */
//	if (!memcmp(srECROb->srTransData.szField_05, _ECR_TRUST_INDICATOR_, 1) || pobTran->srTrustRec.uszTrustTransBit == VS_TRUE)
//	{
//		memcpy(&szDataBuffer[inPacketSizes], _ECR_TRUST_INDICATOR_, 1);
//	}
//	else if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2)	||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2)			||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2)			||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_, 2)			||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)		||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_, 2)		||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)	||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2)	||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2)		||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_SALE_, 2)		||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)	||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)		||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//	{
//		if (!memcmp(srECROb->srTransData.szField_05, "C", 1) || pobTran->srBRec.uszCUPTransBit == VS_TRUE)
//			memcpy(&szDataBuffer[inPacketSizes], "C", 1);
//		else if (!memcmp(srECROb->srTransData.szField_05, "S", 1) || pobTran->srBRec.uszFiscTransBit == VS_TRUE)
//			memcpy(&szDataBuffer[inPacketSizes], "S", 1);
//		else if (!memcmp(srECROb->srTransData.szField_05, "E", 1) || pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//			memcpy(&szDataBuffer[inPacketSizes], "E", 1);
//		else if (!memcmp(srECROb->srTransData.szField_05, _ECR_EW_INDICATOR_, 1) || pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//			memcpy(&szDataBuffer[inPacketSizes], _ECR_EW_INDICATOR_, 1);
//		else
		        memcpy(&szDataBuffer[inPacketSizes], "N", 1);
//	}

	inPacketSizes ++;

	/* HOST ID (2 Byte) */
	if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_OFFLINE_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2)				||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_INQUIRY_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_SALE_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_AWARD_REDEEM_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
	{
//		memset(szTemplate, 0x00, sizeof(szTemplate));
//		inGetTRTFileName(szTemplate);
		
//		if (!memcmp(szTemplate, _TRT_FILE_NAME_CREDIT_, strlen(_TRT_FILE_NAME_CREDIT_)))
			memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_HOSTID_NCCC_, 2);
//		/* 2018/5/8 下午 2:10 DFS需求不再使用大來主機 */
//		else if (!memcmp(szTemplate, _TRT_FILE_NAME_HG_, strlen(_TRT_FILE_NAME_HG_)))
//			memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_HOSTID_HG_, 2);
//		else if (!memcmp(szTemplate, _TRT_FILE_NAME_DCC_, strlen(_TRT_FILE_NAME_DCC_)))
//			memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_HOSTID_DCC_, 2);
//		else if (!memcmp(szTemplate, _TRT_FILE_NAME_ESVC_, strlen(_TRT_FILE_NAME_ESVC_)))
//			memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_HOSTID_ESVC_, 2);
//		else if (!memcmp(szTemplate, _TRT_FILE_NAME_EW_, strlen(_TRT_FILE_NAME_EW_)))
//			memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_HOSTID_EW_, 2);
//		else if (!memcmp(szTemplate, _TRT_FILE_NAME_TRUST_, strlen(_TRT_FILE_NAME_TRUST_)))
//			memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_HOSTID_TRUST_, 2);
	}

	inPacketSizes += 2;

//	if (pobTran->srTrustRec.uszTrustTransBit == VS_TRUE)
//	{
//		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2))
//		{
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			sprintf(szTemplate, "%s", pobTran->srTrustRec.szTrustRRN);
//			memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//		}
//                inPacketSizes += 12;
//		
//		/* Reserved */
//		inPacketSizes += 17;
//	}
//	else
//	{
//		/* Receipt No (6 Byte) */
		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_OFFLINE_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2)				||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)	||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_INQUIRY_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_SALE_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2)	||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2)	||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)	||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_AWARD_REDEEM_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
		{
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//			{
//				sprintf(szTemplate, "%06ld", pobTran->srTRec.lnInvNum);
//			}
//			else
//			{
//				sprintf(szTemplate, "%06ld", pobTran->srBRec.lnOrgInvNum);
//			}
//			memcpy(&szDataBuffer[inPacketSizes], szTemplate, 6);
                        memcpy(&szDataBuffer[inPacketSizes], "000777", 6);
		}

		inPacketSizes += 6;

		/* Card No (19 Byte) */
//		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SETTLEMENT_, 2)			||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_DETAIL_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_TOTAL_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_DETAIL_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_TOTAL_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EDC_REBOOT_, 2)			||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_INQUIRY_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2)		||
//		    (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2) && !(!memcmp(pobTran->szL3_AwardWay, "4", 1) || !memcmp(pobTran->szL3_AwardWay, "5", 1)))	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_AWARD_REDEEM_, 2)		||
//		    pobTran->srBRec.uszHappyGoSingle == VS_TRUE)
//		{
//			/* 優惠兌換先設定不回傳卡號 */
//			inPacketSizes += 19;
//		}
//		else
//		{
//			if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//			{
//				memset(szTemplate, 0x00, sizeof(szTemplate));
//
//				if (pobTran->uszCardInquiryFirstBit == VS_TRUE)
//				{
//					if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ECC_)
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						inLen = strlen(pobTran->srTRec.srECCRec.szCardID);
//						memcpy(szTemplate, pobTran->srTRec.srECCRec.szCardID, inLen);
//					}
//					else
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						inLen = strlen(pobTran->srTRec.szUID);
//						memcpy(szTemplate, pobTran->srTRec.szUID, inLen);
//					}
//				}
//				else
//				{
//					/* 一卡通：遮掩後2 碼，遮掩字元為”*” */
//					if (pobTran->srTRec.inTicketType == _TICKET_TYPE_IPASS_)
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						inLen = strlen(pobTran->srTRec.szUID);
//						memcpy(szTemplate, pobTran->srTRec.szUID, inLen);
//						szTemplate[inLen - 1] = 0x2A;
//						szTemplate[inLen - 2] = 0x2A;
//					}
//					/*	1.簽單卡號列印邏輯
//						<T4800>票卡版號(Purse Version Number)=0，<T0200>列印晶片卡號(Card Id)
//						<T0200>20 bytes，14th,15th,16th隱碼，17th,18th,19th,20th顯示
//						<T4800>票卡版號(Purse Version Number)<>0，<T0211>列印外觀卡號(Purse Id)
//						<T0211>16 bytes，6th,7th,8th,9th,10th,11th隱碼，12th,13th,14th,15th,16th顯示
//						，遮掩字元為”*”
//						以<T0200>悠遊卡卡號遮掩從後往前算第五碼，第六碼，第七碼遮掩
//						範例:以8碼卡號為例，卡號11651733，1,6,5要遮掩，實際列印為1***1733
//						1th 2th 3th 4th 5th 6th 7th 8th 9th 10th 11th 12th 13th 14th 15th 16th 17th
//						0 0 0 0 0 0 0 0 0 1 1 6 5 1 7 3 3
//						0 0 0 0 0 0 0 0 0 1 * * * 1 7 3 3
//						範例:以10碼卡號為例，卡號6611651733，1,6,5要遮掩，實際列印為661***1733
//						1th 2th 3th 4th 5th 6th 7th 8th 9th 10th 11th 12th 13th 14th 15th 16th 17th
//						0 0 0 0 0 0 0 6 6 1 1 6 5 1 7 3 3
//						0 0 0 0 0 0 0 6 6 1 * * * 1 7 3 3
//
//						2. 電子發票載具卡號為T0200 (不遮掩)
//					*/
//					else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ECC_)
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						inLen = strlen(pobTran->srTRec.szUID);
//						memcpy(szTemplate, pobTran->srTRec.szUID, inLen);
//
//						if (atoi(pobTran->srTRec.srECCRec.szPurseVersionNumber) == 0)
//						{
//							szTemplate[inLen - 5] = 0x2A;
//							szTemplate[inLen - 6] = 0x2A;
//							szTemplate[inLen - 7] = 0x2A;
//						}
//						else
//						{
//							/* 全部16 第6-11隱碼 */
//							szTemplate[inLen - 6] = 0x2A;
//							szTemplate[inLen - 7] = 0x2A;
//							szTemplate[inLen - 8] = 0x2A;
//							szTemplate[inLen - 9] = 0x2A;
//							szTemplate[inLen - 10] = 0x2A;
//							szTemplate[inLen - 11] = 0x2A;
//						}
//					}
//					else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ICASH_)
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						inLen = strlen(pobTran->srTRec.szUID);
//						memcpy(szTemplate, pobTran->srTRec.szUID, inLen);
//						/* 愛金卡：遮掩9-12 碼，遮掩字元為"*" */
//						szTemplate[8] = 0x2A;
//						szTemplate[9] = 0x2A;
//						szTemplate[10] = 0x2A;
//						szTemplate[11] = 0x2A;
//					}
//				}
//
//				memcpy(&szDataBuffer[inPacketSizes], szTemplate, strlen(szTemplate));
//			}
//			/* (需求單-109455)-Üny實體掃碼需求 by Russell 2021/9/3 下午 6:18 */
//			/* 不參考TMS遮掩開關 */
//			else if (pobTran->srBRec.uszUnyTransBit == VS_TRUE)
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srBRec.szPAN, strlen(pobTran->srBRec.szPAN));
//			}
//			else if (pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//			{
//				if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2))
//				{
//					/* 悠遊付：當消費者支付工具為”1：信用卡”時，遮掩的信用卡卡號 */
//					/* 全支付：遮掩的信用卡卡號或帳戶 */
//					memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_MaskCreditCardNo, strlen(pobTran->srEWRec.szEW_MaskCreditCardNo));
//				}
//				else if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//				{
//					/* 悠遊付：當消費者支付工具為”1：信用卡”時，遮掩的信用卡卡號 */
//					/* icash Pay：卡號前六、後四碼，中間*號遮蔽 */
//					/* 全支付：遮掩的信用卡卡號或帳戶 */
//					memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_MaskCreditCardNo, strlen(pobTran->srEWRec.szEW_MaskCreditCardNo));
//				}
//			}
//			else
//			{
//				/* 卡號是否遮掩 */
//				inRetVal = inECR_CardNoTruncateDecision(pobTran);
//				/* 要遮卡號 */
//				if (inRetVal == VS_SUCCESS)
//				{
//					memset(szTemplate, 0x00, sizeof(szTemplate));
//					strcpy(szTemplate, pobTran->srBRec.szPAN);
//
//					/* HAPPG_GO 卡不掩飾 */
//					if (!memcmp(&pobTran->srBRec.szPAN[0], "9552", 4))
//					{
//
//					}
//					else
//					{
//						/* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//						/* 卡號長度 */
//						inCardLen = strlen(szTemplate);
//
//						if (!memcmp(szCustomerIndicator, _CUSTOMER_INDICATOR_088_TINTIN_, _CUSTOMER_INDICATOR_SIZE_))
//						{
//							/* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//							for (i = 8; i < (inCardLen - 4); i ++)
//								szTemplate[i] = '*';
//						}
//						else if (!memcmp(szCustomerIndicator, _CUSTOMER_INDICATOR_124_EVER_RICH_, _CUSTOMER_INDICATOR_SIZE_))
//						{
//							/* 第一段查詢交易，回覆信用卡前8後2卡號 */
//							if (pobTran->uszCardInquiryFirstBit == VS_TRUE ||
//							    pobTran->uszCardInquirysSecondBit == VS_TRUE)
//							{
//								for (i = 8; i < (inCardLen - 2); i ++)
//									szTemplate[i] = '*';
//							}
//							else
//							{
//								for (i = 6; i < (inCardLen - 4); i ++)
//									szTemplate[i] = '*';
//							}
//						}
//						else
//						{    
//							/* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//							for (i = 6; i < (inCardLen - 4); i ++)
//								szTemplate[i] = '*';
//						}
//					}
//					memcpy(&szDataBuffer[inPacketSizes], szTemplate, strlen(szTemplate));
//				}
//				else
//				{
//					memcpy(&szDataBuffer[inPacketSizes], pobTran->srBRec.szPAN, strlen(pobTran->srBRec.szPAN));
//				}
//			}
			inPacketSizes += 19;
//		}
//
//		/* Card Expire Date (4 Byte) */
//		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SETTLEMENT_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_DETAIL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_TOTAL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_DETAIL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_TOTAL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EDC_REBOOT_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_INQUIRY_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2))
//		{
//			inPacketSizes += 4;
//		}
//		else if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//		{
//			inPacketSizes += 4;
//		}
//		/* (需求單-109455)-Üny實體掃碼需求 by Russell 2021/9/3 下午 6:18 */
//		/* Uny交易不回傳有效期 */
//		else if (pobTran->srBRec.uszUnyTransBit == VS_TRUE)
//		{
//			inPacketSizes += 4;
//		}
//		/* 電子錢包交易不回傳有效期 */
//		/* (需求單-111155)-電子錢包業者收付訊息整合平台需求 by Russell 2023/7/24 下午 6:17 */
//		else if (pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//		{
//			inPacketSizes += 4;
//		}
//		else
//		{
//			/* 有效期是否回傳 */
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			inGetECRExpDateReturnEnable(szTemplate);
//
//			/* 要回傳有效期 */
//			if (!memcmp(&szTemplate, "Y", 1))
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srBRec.szExpDate, strlen(pobTran->srBRec.szExpDate));
//
			inPacketSizes += 4;
//		}
//	}

	/* Trans Amount (12 Byte) */
//	if (pobTran->srTrustRec.uszTrustTransBit == VS_TRUE)
//	{
//		/* 因為信託交易必定要回交易金額，所以必須擺在最上面 */
//                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2))
//		{
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", pobTran->srTrustRec.lnTxnAmount);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//		}
//
//		inPacketSizes += 12;
//	}
//	else if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2)			||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2)			||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)	||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2)	||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SETTLEMENT_, 2)			||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_DETAIL_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_TOTAL_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_DETAIL_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_TOTAL_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EDC_REBOOT_, 2)			||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_INQUIRY_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2))
//	{
//	        inPacketSizes += 12;
//	}
//        else if (pobTran->srBRec.uszUPlanTransBit == VS_TRUE)
//        {
//                if (pobTran->srBRec.uszVOIDBit == VS_TRUE)
//                {
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnTxnAmount);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                }
//                else
//                {
//                    if (pobTran->srBRec.lnCUPUPlanDiscountedAmount > 0)
//                    {
//                            memset(szTemplate, 0x00, sizeof(szTemplate));
//                            sprintf(szTemplate, "%010lu00", (pobTran->srBRec.lnTxnAmount + pobTran->srBRec.lnCUPUPlanPreferentialAmount));
//                            memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                    }
//                    else
//                    {
//                            memset(szTemplate, 0x00, sizeof(szTemplate));
//                            sprintf(szTemplate, "%010lu00", (pobTran->srBRec.lnTxnAmount + pobTran->srBRec.lnCUPUPlanPreferentialAmount));
//                            memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                    }
//                }
//                inPacketSizes += 12;
//        }
//        else
//        {
//                if (pobTran->srTRec.lnTxnAmount != 0)
//                {
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", pobTran->srTRec.lnTxnAmount);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//
//                        inPacketSizes += 12;
//                }
//                else if (pobTran->srBRec.lnTxnAmount != 0)
//                {
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnTxnAmount);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//
//                        inPacketSizes += 12;
//                }
//                else if (pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//                {
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu", pobTran->srEWRec.lnTxnAmount);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 10);
//                        inPacketSizes += 10;
//
//                        memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_Decimal, 2);
//                        inPacketSizes += 2;
//                }
//                else
//                {
//                        inPacketSizes += 12;
//                }
//        }
            memcpy(&szDataBuffer[inPacketSizes], "123215432100", 12);
            inPacketSizes += 12;
	/* Trans Date (6 Byte) & Trans Time (6 Byte) */
	if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_OFFLINE_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2)				||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SETTLEMENT_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_INQUIRY_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_SALE_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_AWARD_REDEEM_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ECHO_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
	{
//		if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//		{
//			/* Trans Date */
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			strcat(szTemplate, pobTran->srTRec.szDate);
//			memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 6);
//			inPacketSizes += 6;
//
//			/* Trans Time */
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			strcat(szTemplate, pobTran->srTRec.szTime);
//			memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 6);
//			inPacketSizes += 6;
//		}
//		else
//		{
//			/* Trans Date */
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			strcat(szTemplate, pobTran->srBRec.szDate);
//			memcpy(&szDataBuffer[inPacketSizes], &szTemplate[2], 6);
			memcpy(&szDataBuffer[inPacketSizes], "260116", 6);
			inPacketSizes += 6;
//
//			/* Trans Time */
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			strcat(szTemplate, pobTran->srBRec.szTime);
//			memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 6);
			memcpy(&szDataBuffer[inPacketSizes], "103259", 6);
			inPacketSizes += 6;
//		}
	}
	else
	{
		inPacketSizes += 6;
		inPacketSizes += 6;
	}

	/* Approval No (9 Byte) */
	if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_OFFLINE_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2)				||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_SALE_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2))
	{
//		if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//		{
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			strcpy(szTemplate, pobTran->srTRec.szAuthCode);
//			inFunc_PAD_ASCII(szTemplate, szTemplate, ' ', 9, _PADDING_RIGHT_);
//			memcpy(&szDataBuffer[inPacketSizes], szTemplate, 9);
//		}
//		/* 電子錢包交易不回傳授權碼 */
//		/* (需求單-111155)-電子錢包業者收付訊息整合平台需求 by Russell 2023/7/24 下午 6:17 */
//		else if (pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//		{
//			
//		}
//		else
//		{
			memset(szTemplate, 0x00, sizeof(szTemplate));
//			strcpy(szTemplate, pobTran->srBRec.szAuthCode);
			strcpy(szTemplate, "00");
			inFunc_PAD_ASCII(szTemplate, szTemplate, ' ', 9, _PADDING_RIGHT_);
			memcpy(&szDataBuffer[inPacketSizes], szTemplate, 9);
//		}
	}

	inPacketSizes += 9;

	/* Wave Card Indicator (1 Byte) */
//	if (!memcmp(&srECROb->srTransData.szECRIndicator[0], "Q", 1))
//	{
//		/* 電子錢包交易不回傳Wave Card Indicator */
//		/* (需求單-111155)-電子錢包業者收付訊息整合平台需求 by Russell 2023/7/24 下午 6:17 */
//		/* Indicator為Q目前不送此欄位 */
//	}
//	else
//	{
//		if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//		{
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2))
//			{
//				if (pobTran->srTRec.inTicketType == _TICKET_TYPE_IPASS_)
//				{
//					memcpy(&szDataBuffer[inPacketSizes], "P", 1);
//				}
//				else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ECC_)
//				{
//					memcpy(&szDataBuffer[inPacketSizes], "Z", 1);
//				}
//				else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ICASH_)
//				{
//					memcpy(&szDataBuffer[inPacketSizes], "G", 1);
//				}
//				else
//				{
//					memcpy(&szDataBuffer[inPacketSizes], "O", 1);
//				}
//			}
//		}
//		else
//		{
			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2)	||
			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2)	||
			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2))
			{
//				if (pobTran->srBRec.uszContactlessBit == VS_TRUE)
//				{
//					/* 規格未寫M 跟單機同步 */
//					memset(szTemplate, 0x00, sizeof(szTemplate));
//					strcpy(szTemplate, pobTran->srBRec.szCardLabel);
//
//					if (!memcmp(szTemplate, _CARD_TYPE_VISA_, strlen(_CARD_TYPE_VISA_)))
						memcpy(&szDataBuffer[inPacketSizes], "V", 1);
//					else if (!memcmp(szTemplate, _CARD_TYPE_MASTERCARD_, strlen(_CARD_TYPE_MASTERCARD_)))
//						memcpy(&szDataBuffer[inPacketSizes], "M", 1);
//					else if (!memcmp(szTemplate, _CARD_TYPE_JCB_, strlen(_CARD_TYPE_JCB_)))
//						memcpy(&szDataBuffer[inPacketSizes], "J", 1);
//					else if (!memcmp(szTemplate, _CARD_TYPE_CUP_, strlen(_CARD_TYPE_CUP_)))
//						memcpy(&szDataBuffer[inPacketSizes], "C", 1);
//					else if (!memcmp(szTemplate, _CARD_TYPE_AMEX_, strlen(_CARD_TYPE_AMEX_)))
//						memcpy(&szDataBuffer[inPacketSizes], "A", 1);
//					/* (需求單 - 106349) 2018/10/31 下午 3:41 by Russell */
//					else if (!memcmp(szTemplate, _CARD_TYPE_DINERS_, strlen(_CARD_TYPE_DINERS_)) ||
//						 pobTran->srBRec.uszUCARDTransBit == VS_TRUE)
//					{
//						if (pobTran->srBRec.uszUCARDTransBit == VS_TRUE)
//						{
//							memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szMCP_BANKID[4], 1);
//						}
//						else
//						{
//							memcpy(&szDataBuffer[inPacketSizes], "D", 1);
//						}
//					}
//					else
//						memcpy(&szDataBuffer[inPacketSizes], "O", 1);
//				}
			}
//		}
//	}
	inPacketSizes ++;

	/* ECR Response Code (4 Byte) */
	memcpy(&szDataBuffer[inPacketSizes], "0000", 4);
	
	inPacketSizes += 4;

	/* Merchant ID (15 Byte) & Terminal ID (8 Byte) */
//	if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_DETAIL_, 2) ||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_TOTAL_, 2) ||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SETTLEMENT_, 2))
//	{
//		inPacketSizes += 15;
//		inPacketSizes += 8;
//	}
//	else
//	{
		/* Merchant ID (15 Byte) */
		memset(szTemplate, 0x00, sizeof(szTemplate));
                strcpy(szTemplate,"1213141");
//		inGetMerchantID(szTemplate);
		inFunc_PAD_ASCII(szTemplate, szTemplate, ' ', 15, _PADDING_RIGHT_);
		memcpy(&szDataBuffer[inPacketSizes], szTemplate, 15);
		inPacketSizes += 15;
		/* Terminal ID (8 Byte) */
		memset(szTemplate, 0x00, sizeof(szTemplate));
                strcpy(szTemplate,"3456");
//		inGetTerminalID(szTemplate);
		inFunc_PAD_ASCII(szTemplate, szTemplate, ' ', 8, _PADDING_RIGHT_);
		memcpy(&szDataBuffer[inPacketSizes], szTemplate, 8);
		inPacketSizes += 8;
//	}
//
	/* Exp Amount (12 Byte) */
//	memset(szTemplate, 0x00, sizeof(szTemplate));
//	inGetTRTFileName(szTemplate);
//        if (!memcmp(szTemplate, _TRT_FILE_NAME_DCC_, _TRT_FILE_NAME_DCC_LEN_) && !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2))
//        {
//                if (!memcmp(&pobTran->srBRec.szDCC_FCMU[0], "0", 1))
//                {
//        	        memset(szTemplate, 0x00, sizeof(szTemplate));
//        		sprintf(szTemplate, "%010ld00", atol(pobTran->srBRec.szDCC_TIPFCA));
//        		memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//        	}
//                else if (!memcmp(&pobTran->srBRec.szDCC_FCMU[0], "1", 1))
//                {
//        	        memset(szTemplate, 0x00, sizeof(szTemplate));
//        		sprintf(szTemplate, "%011ld0", atol(pobTran->srBRec.szDCC_TIPFCA));
//        		memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//        	}
//        	else if (!memcmp(&pobTran->srBRec.szDCC_FCMU[0], "2", 1))
//                {
//        	        memset(szTemplate, 0x00, sizeof(szTemplate));
//        		sprintf(szTemplate, "%012ld", atol(pobTran->srBRec.szDCC_TIPFCA));
//        		memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//        	}
//        }
//        else if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2))
//        {
//		memset(szTemplate, 0x00, sizeof(szTemplate));
//		sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnTipTxnAmount);
//		memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//	}

	inPacketSizes += 12;
	
	/* Store Id (18 Byte) */
	inPacketSizes += 18;

//        if (pobTran->srTrustRec.uszTrustTransBit == VS_TRUE)
//        {
//                /* 信託 */
//                /* 銷帳編號 */
//                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2))
//                {
//                        if (strlen(pobTran->srTrustRec.szReconciliationNo) > 0)
//                        {
//                                memcpy(&szDataBuffer[inPacketSizes], &pobTran->srTrustRec.szReconciliationNo[0], 20);
//                        }
//                }
//                inPacketSizes += 20;
//                /* 信託機構代碼 */
//                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2))
//                {
//                        memcpy(&szDataBuffer[inPacketSizes], &pobTran->srTrustRec.szTrustInstitutionCode[0], 7);
//                }
//                inPacketSizes += 7;
//                /* 交換平台序號 */
//                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2))
//                {
//                        memcpy(&szDataBuffer[inPacketSizes], &pobTran->srTrustRec.szExchangeTxSerialNumber[0], 32);
//                }
//                inPacketSizes += 32;
//                /* 主機回復中文說明 */
//                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2))
//                {
//                        memcpy(&szDataBuffer[inPacketSizes], &pobTran->szHostResponseMessageBIG5[0], 20);
//                }
//                inPacketSizes += 20;
//                /* 信託原交易日期 */
//                inPacketSizes += 4;
//                /* 保留 */
//                inPacketSizes += 4;
//        }
//        else
//        {
//                /* 處理紅利、分期 */
//                /* Installment / Redeem Indictor (1 Byte) */
//                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)		||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)		||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2) ||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2))
//                {
//                        if (pobTran->srBRec.uszInstallmentBit == VS_TRUE)
//                                memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szInstallmentIndicator[0], 1);
//                        else if (pobTran->srBRec.uszRedeemBit == VS_TRUE)
//                                memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szRedeemIndicator[0], 1);
//                }
//
//                /* 優惠兌換方式 */
//                /* 1.條碼兌換 2.卡號兌換 */
//                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2))
//                {
//                        if (!memcmp(pobTran->szL3_AwardWay, "1", 1)	||
//                            !memcmp(pobTran->szL3_AwardWay, "2", 1)	||
//                            !memcmp(pobTran->szL3_AwardWay, "3", 1))
//                        {
//                                memcpy(&szDataBuffer[inPacketSizes], "1", 1);
//                        }
//                        else if (!memcmp(pobTran->szL3_AwardWay, "4", 1)	||
//                                 !memcmp(pobTran->szL3_AwardWay, "5", 1))
//                        {
//                                memcpy(&szDataBuffer[inPacketSizes], "2", 1);
//                        }
//                }
//                else if(!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_AWARD_REDEEM_, 2))
//                {
//                        /* (1= 條碼兌換， 2= 卡號兌換 )*/
//                        /* 取消優惠兌換只接受 條碼兌換 。 */
//                        memcpy(&szDataBuffer[inPacketSizes], "1", 1);
//                }
                inPacketSizes ++;
//
//                /* 處理紅利扣抵 */
//                if ((!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)		||
//                     !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)	||
//                     !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)	||
//                     !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2)) && pobTran->srBRec.uszRedeemBit == VS_TRUE)
//                {
//                        /* 支付金額 RDM Paid Amt (12 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", (pobTran->srBRec.lnRedemptionPaidCreditAmount + pobTran->srBRec.lnTipTxnAmount));
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                        inPacketSizes += 12;
//                        /* 紅利扣抵點數 RDM Point (8 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%08lu", pobTran->srBRec.lnRedemptionPoints);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 8);
//                        inPacketSizes += 8;
//                        /* 紅利剩餘點數 Points Of Balance (8 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%08lu", pobTran->srBRec.lnRedemptionPointsBalance);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 8);
//                        inPacketSizes += 8;
//                        /* 紅利扣抵金額 Redeem Amt (12 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", (pobTran->srBRec.lnTxnAmount - pobTran->srBRec.lnRedemptionPaidCreditAmount));
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                        inPacketSizes += 12;
//                }
//                else if (pobTran->srBRec.uszUPlanTransBit == VS_TRUE)
//                {
//                        if (pobTran->srBRec.uszVOIDBit == VS_TRUE)
//                        {
//                                inPacketSizes += 40;
//                        }
//                        else
//                        {
//                                /* 銀聯優計劃：優惠後金額(含小數2 位) */
//                                memset(szTemplate, 0x00, sizeof(szTemplate));
//                                sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnCUPUPlanDiscountedAmount);
//                                memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                                inPacketSizes += 12;
//
//                                /* reserved */
//                                inPacketSizes += 8;
//
//                                /* reserved */
//                                inPacketSizes += 8;
//
//                                /* 銀聯優計劃：優惠金額(含小數2 位) */
//                                memset(szTemplate, 0x00, sizeof(szTemplate));
//                                sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnCUPUPlanPreferentialAmount);
//                                memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                                inPacketSizes += 12;
//                        }
//                }
//                else
//                {
                        inPacketSizes += 40;
//                }
//
//                /* 處理分期交易 */
//                if ((!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)	||
//                     !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)	||
//                     !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)	||
//                     !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2)) && pobTran->srBRec.uszInstallmentBit == VS_TRUE)
//                {
//                        /* 分期期數 Installment Period (2 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%02ld", pobTran->srBRec.lnInstallmentPeriod);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 2);
//                        inPacketSizes += 2;
//                        /* 首期金額 Down Payment (12 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnInstallmentDownPayment);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                        inPacketSizes += 12;
//                        /* 每期金額 Installment Payment Amt (12 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnInstallmentPayment);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                        inPacketSizes += 12;
//                        /* 分期手續費 Formlity Fee (12 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnInstallmentFormalityFee);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                        inPacketSizes += 12;
//                }
//                else if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//                {
//                        inPacketSizes += 2;
//
//                        /* 交易前餘額 */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2)	||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2)	||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)		||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2)		||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_TOTAL_, 2)		||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_DETAIL_, 2))
//                        {
//                                /* 查餘額不帶此欄位 */
//                                /* 啟動卡號查詢不帶此欄位 */
//                                sprintf(szTemplate, "            ");
//                        }
//                        else
//                        {
//                                if (pobTran->srTRec.inTicketType == _TICKET_TYPE_IPASS_)
//                                {
//                                        if (pobTran->srTRec.lnFinalBeforeAmt > 100000)
//                                        {
//                                                sprintf(szTemplate, "-%09lu00", (pobTran->srTRec.lnFinalBeforeAmt - 100000));
//                                        }
//                                        else
//                                        {
//                                                /* 交易前餘額要帶未加值的金額 */
//                                                if ((pobTran->srTRec.lnCardRemainAmount - pobTran->srTRec.lnTotalTopUpAmount) < 0)
//                                                        sprintf(szTemplate, "-%09lu00", (unsigned long)(0 - (pobTran->srTRec.lnCardRemainAmount - pobTran->srTRec.lnTotalTopUpAmount)));
//                                                else
//                                                        sprintf(szTemplate, "+%09lu00", (unsigned long)pobTran->srTRec.lnCardRemainAmount - pobTran->srTRec.lnTotalTopUpAmount);
//                                        }
//                                }
//                                else
//                                {
//                                        if (pobTran->srTRec.lnFinalBeforeAmt < 0)
//                                        {
//                                                sprintf(szTemplate, "-%09lu00", (0 - pobTran->srTRec.lnFinalBeforeAmt));
//                                        }
//                                        else
//                                        {
//                                                sprintf(szTemplate, "+%09lu00", pobTran->srTRec.lnFinalBeforeAmt);
//                                        }
//                                }
//
//                        }
//
//                        memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//                        inPacketSizes += 12;
//
//                        /* 交易後餘額 */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2)	||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)		||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2)		||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_TOTAL_, 2)		||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_DETAIL_, 2))
//                        {
//                                /* 啟動卡號查詢不帶此欄位 */
//                                sprintf(szTemplate, "            ");
//                        }
//                        else if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2))
//                        {
//                                if (pobTran->srTRec.lnCardRemainAmount < 0)
//                                {
//                                        sprintf(szTemplate, "-%09lu00", (unsigned long)(0 - pobTran->srTRec.lnCardRemainAmount));
//                                }
//                                else
//                                {
//                                        sprintf(szTemplate, "+%09lu00", (unsigned long)pobTran->srTRec.lnCardRemainAmount);
//                                }
//                        }
//                        else
//                        {
//                                if (pobTran->srTRec.inTicketType == _TICKET_TYPE_IPASS_)
//                                {
//                                        if (pobTran->srTRec.lnFinalAfterAmt > 100000)
//                                                sprintf(szTemplate, "-%09lu00", (unsigned long)(pobTran->srTRec.lnFinalAfterAmt - 100000));
//                                        else
//                                                sprintf(szTemplate, "+%09lu00", (unsigned long)(pobTran->srTRec.lnFinalAfterAmt));
//                                }
//                                else
//                                {
//                                        if (pobTran->srTRec.lnFinalAfterAmt < 0)
//                                                sprintf(szTemplate, "-%09lu00", (unsigned long)(0 - pobTran->srTRec.lnFinalAfterAmt));
//                                        else
//                                                sprintf(szTemplate, "+%09lu00", (unsigned long)(pobTran->srTRec.lnFinalAfterAmt));
//                                }
//                        }
//
//                        memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//                        inPacketSizes += 12;
//
//                        /* 自動加值金額 */
//                        if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2))
//                        {
//                                memset(szTemplate, 0x00, sizeof(szTemplate));
//                                if (pobTran->srTRec.lnTotalTopUpAmount > 0L)
//                                        sprintf(szTemplate, "+%09lu00", (unsigned long)(pobTran->srTRec.lnTotalTopUpAmount));
//                                else
//                                        sprintf(szTemplate, "+%09lu00", (unsigned long)(0));
//
//                                memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//                        }
//                        inPacketSizes += 12;
//                }
//                else
                        inPacketSizes += 38;

                /* Card Type (2 Byte) */
                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2)					||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)							||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2)							||
                    (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2) && pobTran->srBRec.uszHappyGoSingle != VS_TRUE)	||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_OFFLINE_, 2)							||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2)								||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_, 2)							||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)						||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_, 2)						||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)					||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)							||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)						||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)						||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)							||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)						||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)						||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)							||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2)					||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2)						||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2)					||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2)					||
                    ((!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_SALE_, 2)						||
                      !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)						||
                      !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)) && pobTran->srBRec.uszHappyGoMulti == VS_TRUE)	||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
                {
//                        if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//                        {
//                                if (pobTran->srTRec.inTicketType == _TICKET_TYPE_IPASS_)
//                                {
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_IPASS_, 2);
//                                }
//                                else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ECC_)
//                                {
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_ECC_, 2);
//                                }
//                                else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ICASH_)
//                                {
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_ICASH_, 2);
//                                }
//                                else
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_UNKNOWN_, _ECR_8N1_NCCC_CARDTYPE_UNKNOWN_LEN_);
//                        }
//                        else if (pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//                        {
//                                memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_EWIssuerID, _EW_ISSUERID_LEN_);
//                        }
//                        else
//                        {
//                                memset(szTemplate, 0x00, sizeof(szTemplate));
//                                strcpy(szTemplate, pobTran->srBRec.szCardLabel);
//
//                                /* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//                                if (!memcmp(szTemplate, _CARD_TYPE_VISA_, strlen(_CARD_TYPE_VISA_)))
                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_VISA_, 2);
//                                else if (!memcmp(szTemplate, _CARD_TYPE_MASTERCARD_, strlen(_CARD_TYPE_MASTERCARD_)))
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_MASTERCARD_, 2);
//                                else if (!memcmp(szTemplate, _CARD_TYPE_JCB_, strlen(_CARD_TYPE_JCB_)))
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_JCB_, 2);
//                                else if (!memcmp(szTemplate, _CARD_TYPE_AMEX_, strlen(_CARD_TYPE_AMEX_)))
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_AMEX_, 2);
//                                else if (!memcmp(szTemplate, _CARD_TYPE_CUP_, strlen(_CARD_TYPE_CUP_)))
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_CUP_, 2);
//                                else if (!memcmp(szTemplate, _CARD_TYPE_DINERS_, strlen(_CARD_TYPE_DINERS_)) ||
//                                         pobTran->srBRec.uszUCARDTransBit == VS_TRUE)
//                                {
//                                        if (pobTran->srBRec.uszUCARDTransBit == VS_TRUE)
//                                        {
//                                                memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_UCARD_, 2);
//                                        }
//                                        else
//                                        {
//                                                memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_DINERS_, 2);
//                                        }
//                                }
//                                else if (!memcmp(szTemplate, _CARD_TYPE_SMARTPAY_, strlen(_CARD_TYPE_SMARTPAY_)))
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_SMARTPAY_, 2);
//                                else
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_UNKNOWN_, _ECR_8N1_NCCC_CARDTYPE_UNKNOWN_LEN_);
//                        }
                }

                inPacketSizes += 2;

                /* Batch No (6 Byte) */
                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_OFFLINE_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2)				||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)	||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_SALE_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2)	||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2)	||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)	||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
                {
                        memset(szTemplate, 0x00, sizeof(szTemplate));
                        sprintf(szTemplate, "%06ld", 999L);
//                        sprintf(szTemplate, "%06ld", pobTran->srBRec.lnBatchNum);
                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 6);
                }

                inPacketSizes += 6;
//        }/* 和信託做區別 */

	/* (需求單-111155)-電子錢包業者收付訊息整合平台需求 by Russell 2023/7/24 下午 6:17 */
//	if (pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//	{
//		/* 特店訂單編號 */
//		memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_OrderId, 30);
//		inPacketSizes += 30;
//		
//		/* 保留 */
//		inPacketSizes += 6;
//		
//		/* 電子發票載具號碼 */
//		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//		{
//			if (strlen(pobTran->srEWRec.szEW_EinvoiceHASH) > 0)
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_EinvoiceHASH, 50);
//			}
//		}
//		inPacketSizes += 50;
//		
//		/* 退貨訂單編號*/
//		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2))
//		{
//			memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_RefundTradeNo, 30);
//		}
//		inPacketSizes += 30;
//		
//		/* 電子錢包業者交易序號 */
//		memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_TransactionId, 30);
//		inPacketSizes += 64;
//	}
//	else
//	{
		/* Start Trans Type (2 Byte) */
//		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2))
//			memcpy(&szDataBuffer[inPacketSizes], srECROb->srTransData.szStartTransType, 2);

		inPacketSizes += 2;

		/* MP Flag(是否小額交易，MPAS小額交易用) (1 Byte) */
		/* 合併MPAS ECR到標準400 */
//		memset(szFESMode, 0x00, sizeof(szFESMode));
//		inGetNCCCFESMode(szFESMode);
//		if (memcmp(szFESMode, _NCCC_04_MPAS_MODE_, strlen(_NCCC_04_MPAS_MODE_)) == 0)
//		{
//			if (pobTran->srBRec.uszMPASTransBit == VS_TRUE)
				memcpy(&szDataBuffer[inPacketSizes], "M", 1);
//			else
//				memcpy(&szDataBuffer[inPacketSizes], " ", 1);
//		}
		inPacketSizes += 1;

//		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SETTLEMENT_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_DETAIL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_TOTAL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_DETAIL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_TOTAL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EDC_REBOOT_, 2))
//		{
			/* 共99Bytes */
			inPacketSizes += 8;
			inPacketSizes += 8;
			inPacketSizes += 12;
//			inPacketSizes += 5;
//			inPacketSizes += 50;
//			inPacketSizes += 6;
//			inPacketSizes += 1;
//			inPacketSizes += 1;
//			inPacketSizes += 3;
//			inPacketSizes += 5;
//		}
//		else if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//		{
//			inPacketSizes += 8;
//
//			/* ESVC Origin Date */
//			/* 電票退貨要帶回原交易日期 */
//			if (memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2) == 0)
//			{
//				memset(szTemp, 0x00, sizeof(szTemp));
//				memset(szTemp2, 0x00, sizeof(szTemp2));
//				memcpy(szTemp2, pobTran->srTRec.szTicketRefundDate, 2);
//
//				inNCCC_TICKET_Decide_Year(szTemp, szTemp2);
//				memcpy(&szDataBuffer[inPacketSizes], szTemp, 4); 
//				inPacketSizes += 4;
//
//				memcpy(&szDataBuffer[inPacketSizes], &pobTran->srTRec.szTicketRefundDate[0], 4);
//				inPacketSizes += 4;
//			}
//			else
//			{
//				inPacketSizes += 8;
//			}
//
//			/* 兩段式，第一段不用帶RF Number */
//			if (pobTran->uszCardInquiryFirstBit == VS_TRUE)
//			{
//				inPacketSizes += 12;
//			}
//			else
//			{
//				/* 餘額查詢不用帶RF序號 */
//				if (memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2) == 0)
//				{
//					inPacketSizes += 12;
//				}
//				else
//				{
//					/* RF序號 */
//					memcpy(&szDataBuffer[inPacketSizes], &pobTran->srTRec.szTicketRefundCode[0], 6);
//					inPacketSizes += 12;
//				}
//			}

			/* Pay item */
			inPacketSizes += 5;

//			if (!memcmp(&srECROb->srTransData.szECRIndicator[0], "E", 1))
//			{
//				/* Card No. Hash Value */
//				/* 因為卡號查詢會轉交易別，所以用Flag判斷 */
//				if (pobTran->uszCardInquiryFirstBit == VS_TRUE)
//				{
//					memset(szTemplate, 0x00, sizeof(szTemplate));
//				}
//				else
//				{
//					if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2) ||
//					    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2) ||
//					    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2))
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//					}
//					else
//					{
//						if (pobTran->srTRec.inTicketType == _TICKET_TYPE_IPASS_)
//						{
//							memset(szTemplate, 0x00, sizeof(szTemplate));
//							strcpy(szTemplate, pobTran->srTRec.szUID);
//						}
//						else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ECC_)
//						{
//							/* 【需求單-108240】EDC修改電票需求 悠遊卡指定要T0200 by Russell 2020/6/4 下午 2:02 */
//							memset(szTemplate, 0x00, sizeof(szTemplate));
//							strcpy(szTemplate, pobTran->srTRec.srECCRec.szCardID);
//						}
//						else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ICASH_)
//						{
//							memset(szTemplate, 0x00, sizeof(szTemplate));
//							strcpy(szTemplate, pobTran->srTRec.szUID);	
//						}
//					}
//				}
//			}
//			else
//			{
//				/* Card No. Hash Value */
//				if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2) ||
//				    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2) ||
//				    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2))
//				{
//					memset(szTemplate, 0x00, sizeof(szTemplate));
//				}
//				else
//				{
//					if (pobTran->srTRec.inTicketType == _TICKET_TYPE_IPASS_)
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						strcpy(szTemplate, pobTran->srTRec.szUID);
//					}
//					else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ECC_)
//					{
//						/* 【需求單-108240】EDC修改電票需求 悠遊卡指定要T0200 by Russell 2020/6/4 下午 2:02 */
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						strcpy(szTemplate, pobTran->srTRec.srECCRec.szCardID);
//					}
//					else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ICASH_)
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						strcpy(szTemplate, pobTran->srTRec.szUID);	
//					}
//				}
//			}
//			memcpy(&szDataBuffer[inPacketSizes], szTemplate, strlen(szTemplate));
			inPacketSizes += 50;

			/* MP Response Code */
			inPacketSizes += 6;

			/* ASM award flag */
			/* 【需求單-108215】電票交易支援優惠平台需求 by Russell 2020/5/11 下午 3:53 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2))
//			{
//				if ((pobTran->srTRec.uszRewardL1Bit == VS_TRUE	||
//				     pobTran->srTRec.uszRewardL2Bit == VS_TRUE	||
//				     pobTran->srTRec.uszRewardL5Bit == VS_TRUE)	&&
//				     pobTran->srTRec.uszRewardSuspendBit != VS_TRUE)
//				{
//					memcpy(&szDataBuffer[inPacketSizes], "A", strlen("A"));
//				}
//				else
//				{
					memcpy(&szDataBuffer[inPacketSizes], " ", strlen(" "));
//				}
//			}
//			else
//			{
//				memcpy(&szDataBuffer[inPacketSizes], " ", strlen(" "));
//			}
			inPacketSizes += 1;

			/* MCP Indicator & etc.*/
			inPacketSizes += 9;
//		}
//		else
//		{
//			/* SmartPay要回傳這三項資訊 (99 Bytes) */
//			if ((!memcmp(&srECROb->srTransData.szField_05[0], "S", 1) || pobTran->srBRec.uszFiscTransBit == VS_TRUE) &&
//			     pobTran->uszCardInquiryFirstBit != VS_TRUE)
//			{
//				/* SP ISSUER ID (8 Byte) */
//				memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szFiscIssuerID[0], 8);
//				inPacketSizes += 8;
//				/* SP Origin Date (8 Byte) */
//				if (strlen(pobTran->srBRec.szFiscRefundDate) > 0)
//				{
//					memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szFiscRefundDate[0], 8);
//				}  
//				else
//				{
//					memset(szTemplate, 0x00, sizeof(szTemplate));
//					strcat(szTemplate, pobTran->srBRec.szDate);
//					memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 8);
//				}
//				inPacketSizes += 8;
//				/* SP RRN (12 Byte) */
//				memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szFiscRRN[0], 12);
//				inPacketSizes += 12;
//			}
//			else
//			{
//				inPacketSizes += 8;
//				inPacketSizes += 8;
//				inPacketSizes += 12;
//			}
//
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			inGetPayItemEnable(szTemplate);
//			if (memcmp(szTemplate, "Y", strlen("Y")) == 0	&&
//			   (strlen(pobTran->srBRec.szPayItemCode) > 0))
//			{
//				/* Pay Item (5 Bytes) */
//				/* ECR一段式收銀機連線 送空白payitem 不必回傳 */
//				if (srECROb->srTransData.uszECRResponsePayitem == VS_FALSE)
//				{
//					inPacketSizes += 5;
//				}
//				else
//				{
//					memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szPayItemCode[0], 5);
//					inPacketSizes += 5;
//				}
//			}
//			else
//			{
//				inPacketSizes += 5;
//			}
//			/* 【需求單 - 105039】信用卡為電子發票載具，端末機將卡號加密後回傳收銀機 add by LingHsiung 2016-04-20 上午 09:56:24 */
//			/* 【需求單 - 108046】電子發票BIN大於6碼需求 by Russell 2019/7/8 上午 11:44 */
//			/* 收銀機欄位ECR Indicator = “E” : 
//			 * (1)銀行代碼為 ”B00999” : Card No. Hash Value = 空白50碼
//			 * (2)其他 :Card No. Hash Value = 銀行代碼6碼 + Hash Value 44碼。
//			*/
//			/* Card No. Hash Value (50 Bytes) */
//			/* Smartpay 和 DCC 因為不回傳"NI"，所以不回傳hash value */
//			/* 只要沒回傳Table "NI" 一律回50個空白 */
//			/* (需求單-109455)-Üny實體掃碼需求 by Russell 2021/9/3 下午 6:18 */
//			/* Üny 交易：
//			   電子發票加密卡號 (B00xxx ’’+44 碼 HASH 值 xxx為金融機構代碼) */
//			/* 【需求單 - 112071】客製化122 京站時尚百貨，調整收銀機規格
//			 * 符合下列全部條件時，ECR加回欄位37 Card No. Hash Value，內容為 : “B00000” + 44碼卡號Hash值。
//			 * ECR Indicator=‘E’、 有卡號、 電文無NI Table或NI Table為”B00999”(非未加入電子發票載具之銀行或國外卡)。
//			 * 2023/6/12 下午 4:03 by Russell */
//			if (!memcmp(szCustomerIndicator, _CUSTOMER_INDICATOR_122_QSQUARE_, _CUSTOMER_INDICATOR_SIZE_) &&
//			    !memcmp(&srECROb->srTransData.szECRIndicator[0], "E", 1) &&
//			     strlen(pobTran->srBRec.szPAN) > 0 &&
//			    (strlen(pobTran->srBRec.szEIVI_BANKID) == 0 || memcmp(pobTran->srBRec.szEIVI_BANKID, "B00999", 6) == 0))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], "B00000", 6);
//				inPacketSizes += 6;
//
//				memset(szTemplate, 0x00, sizeof(szTemplate));
//				if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2))
//				{
//					
//				}
//				else
//				{
//					memcpy(&szTemplate[0], &pobTran->srBRec.szPAN[0], strlen(pobTran->srBRec.szPAN));
//				}
//				
//				if (strlen(szTemplate) > 0)
//				{
//					memset(szHash, 0x00, sizeof(szHash));
//					inNCCC_Func_CardNumber_Hash(szTemplate, szHash);
//					memcpy(&szDataBuffer[inPacketSizes], &szHash[0], 44);
//				}
//				inPacketSizes += 44;
//			}
//			else
//			{
//				if (!memcmp(&srECROb->srTransData.szECRIndicator[0], "Q", 1))
//				{
//					if (pobTran->uszCardInquiryFirstBit == VS_TRUE	||
//					    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)	||
//					    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2) ||
//                                            pobTran->srBRec.uszUPlanECRBit == VS_TRUE)
//					{
//						inPacketSizes += 6;
//						inPacketSizes += 44;
//					}
//					else
//					{
//						memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szEIVI_BANKID[0], 6);
//						inPacketSizes += 6;
//						memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szEInvoiceHASH[0], 44);
//						inPacketSizes += 44;
//					}
//				}
//				else if (!memcmp(&srECROb->srTransData.szECRIndicator[0], "E", 1))
//				{
//					/* 啟動卡號查詢只有在Indicator = "I" 才送Hash Value("E"一律送50個空白) */
//					/* 因為卡號查詢會轉交易別，所以用Flag判斷 */
//					if (pobTran->uszCardInquiryFirstBit == VS_TRUE)
//					{
//						inPacketSizes += 50;
//					}
//					else
//					{
//						if (strlen(pobTran->srBRec.szEIVI_BANKID) > 0)
//						{
//							if (!memcmp(&pobTran->srBRec.szEIVI_BANKID[0], "B00999", 6))
//							{
//								inPacketSizes += 50;
//							}
//							else
//							{
//								memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szEIVI_BANKID[0], 6);
//								inPacketSizes += 6;
//
//								memset(szTemplate, 0x00, sizeof(szTemplate));
//								if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2))
//								{
//									
//								}
//								else
//								{
//									memcpy(&szTemplate[0], &pobTran->srBRec.szPAN[0], strlen(pobTran->srBRec.szPAN));
//								}
//								
//								if (strlen(szTemplate) > 0)
//								{
//									memset(szHash, 0x00, sizeof(szHash));
//									inNCCC_Func_CardNumber_Hash(szTemplate, szHash);
//									memcpy(&szDataBuffer[inPacketSizes], &szHash[0], 44);
//								}
//								inPacketSizes += 44;
//							}
//						}
//						else
//						{
//							inPacketSizes += 50;
//						}
//					}
//				}
//				/* 收銀機欄位ECR Indicator = “I” : Card No. Hash Value = Card number前6碼 + Hash Value 44碼(原規格) */
//				else
//				{
//					memset(szTemplate, 0x00, sizeof(szTemplate));
//					if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2))
//					{
//						
//					}
//					else
//					{
//						/* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//						memcpy(&szTemplate[0], &pobTran->srBRec.szPAN[0], strlen(pobTran->srBRec.szPAN));
//					}
//
//					if (strlen(szTemplate) > 0)
//					{
//						memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 6);
//						inPacketSizes += 6;
//
//						memset(szHash, 0x00, sizeof(szHash));
//						inNCCC_Func_CardNumber_Hash(szTemplate, szHash);
//						memcpy(&szDataBuffer[inPacketSizes], &szHash[0], 44);
//						inPacketSizes += 44;
//					}
//					else
//					{
//						inPacketSizes += 50;
//					}
//				}
//			}
//
//			/* MP Response Code (6 Bytes) */
//			memset(szFESMode, 0x00, sizeof(szFESMode));
//			inGetNCCCFESMode(szFESMode);
//			if (memcmp(szFESMode, _NCCC_04_MPAS_MODE_, strlen(_NCCC_04_MPAS_MODE_)) == 0)
//			{
//				if (memcmp(pobTran->srBRec.szRespCode, "00", 2) != 0		&& 
//				    memcmp(pobTran->srBRec.szRespCode, "11", 2) != 0		&&
//				    pobTran->srBRec.uszMPASTransBit == VS_TRUE			&&
//				    (memcmp(&pobTran->srBRec.szMPASAuthCode[0], "CAF", 3) == 0	||
//				     memcmp(&pobTran->srBRec.szMPASAuthCode[0], "NEG", 3) == 0	||
//				     memcmp(&pobTran->srBRec.szMPASAuthCode[0], "WEB", 3) == 0	|| 
//				     memcmp(&pobTran->srBRec.szMPASAuthCode[0], "REJ", 3) == 0))
//				{
//					memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szMPASAuthCode[0], 6);
//				}
//			}
//			inPacketSizes += 6;
//
//			/* ASM award flag */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_OFFLINE_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2))
//			{	
//				if ((pobTran->srBRec.uszRewardL1Bit == VS_TRUE	||
//				     pobTran->srBRec.uszRewardL2Bit == VS_TRUE	||
//				     pobTran->srBRec.uszRewardL5Bit == VS_TRUE)	&&
//				     pobTran->srBRec.uszRewardSuspendBit != VS_TRUE)
//				{
//					memcpy(&szDataBuffer[inPacketSizes], "A", strlen("A"));
//				}
//				else
//				{
//					memcpy(&szDataBuffer[inPacketSizes], " ", strlen(" "));
//				}
//			}
//			else
//			{
//				memcpy(&szDataBuffer[inPacketSizes], " ", strlen(" "));
//			}
//			inPacketSizes += 1;
//
//			/* 【需求單 - 106128】行動支付標記及金融機構代碼 add by LingHsiung 2017-08-23 上午 11:48:57 */
//			if (strlen(pobTran->srBRec.szMCP_BANKID) > 0)
//			{
//				/* MCP Indicator (1 Bytes) */
//				memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szMCP_BANKID[0], 1);
//				inPacketSizes += 1;
//
//				/* 金融機構代碼 (3 Bytes) */
//				memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szMCP_BANKID[1], 3);
//				inPacketSizes += 3;
//			}
//			else
//			{
//				inPacketSizes += 4;
//			}
//
//			/* Reserve (5 Bytes) */
//			inPacketSizes += 5;
//		}
//
//		/* HG Data (78 Byte) */
//		if (pobTran->srBRec.lnHGTransactionType != 0	&&
//		    (pobTran->srBRec.uszHappyGoMulti == VS_TRUE || pobTran->srBRec.uszHappyGoSingle == VS_TRUE))
//		{
//			/* Payment Tools (2 Byte) */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2))
//			{
//				if (pobTran->srBRec.lnHGPaymentType == _HG_PAY_CREDIT_)
//					memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_HG_PAYMENT_TOOL_TYPE_CREDIT_CARD_, 2);
//				else if (pobTran->srBRec.lnHGPaymentType == _HG_PAY_CASH_)
//					memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_HG_PAYMENT_TOOL_TYPE_CASH_, 2);
//				else if (pobTran->srBRec.lnHGPaymentType == _HG_PAY_GIFT_PAPER_)
//					memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_HG_PAYMENT_TOOL_TYPE_GIFT_PAPER_, 2);
//				else if (pobTran->srBRec.lnHGPaymentType == _HG_PAY_CREDIT_INSIDE_)
//					memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_HG_PAYMENT_TOOL_TYPE_HGI_, 2);
//				else if (pobTran->srBRec.lnHGPaymentType == _HG_PAY_CUP_)
//					memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_HG_PAYMENT_TOOL_TYPE_CUP_CARD_, 2);
//			}
//
//			inPacketSizes += 2;
//
//			/* HG Card Number (18 Byte) */
//			/* 重印HG混信用卡的交易簽單不回傳HG卡號 */
//			if (memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2) != 0)
//			{
//				inRetVal = inECR_CardNoTruncateDecision_HG(pobTran);
//				/* 要遮卡號 */
//				if (inRetVal == VS_SUCCESS)
//				{
//					memset(szTemplate, 0x00, sizeof(szTemplate));
//					strcpy(szTemplate, pobTran->srBRec.szHGPAN);
//
//					/* HAPPG_GO 卡不掩飾 */
//					if (!memcmp(&pobTran->srBRec.szHGPAN[0], "9552", 4))
//					{
//
//					}
//					else
//					{
//						/* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//						/* 卡號長度 */
//						inCardLen = strlen(szTemplate);
//
//						if (!memcmp(szCustomerIndicator, _CUSTOMER_INDICATOR_088_TINTIN_, _CUSTOMER_INDICATOR_SIZE_))
//						{
//							/* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//							for (i = 8; i < (inCardLen - 4); i ++)
//								szTemplate[i] = '*';
//						}
//						else
//						{    
//							/* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//							for (i = 6; i < (inCardLen - 4); i ++)
//								szTemplate[i] = '*';
//						}
//					}
//
//					memcpy(&szDataBuffer[inPacketSizes], szTemplate, strlen(szTemplate));
//				}
//				else
//				{
//					memcpy(&szDataBuffer[inPacketSizes], pobTran->srBRec.szHGPAN, strlen(pobTran->srBRec.szHGPAN));
//				}
//
//			}
//
//			inPacketSizes += 18;
//
//			/* HG Pay Amount (12 Byte) 實際支付金額 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2))
//			{
//				memset(szTemplate, 0x00, sizeof(szTemplate));
//				sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnHGAmount);
//				memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//			}
//
//			inPacketSizes += 12;
//			/* HG Redeem Amount (12 Byte) 扣抵金額 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2))
//			{
//				memset(szTemplate, 0x00, sizeof(szTemplate));
//				sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnHGRedeemAmount);
//				memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//			}
//
//			inPacketSizes += 12;
//			/* HG Redeem Point (8 Byte) 扣抵點數 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)		||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)		||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2))
//			{
//				memset(szTemplate, 0x00, sizeof(szTemplate));
//				sprintf(szTemplate, "%08lu", pobTran->srBRec.lnHGTransactionPoint);
//				memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 8);
//			}
//
//			inPacketSizes += 8;
//			/* HG Lack Point (8 Byte) 不足點數*/
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2))
//			{
//				memset(szTemplate, 0x00, sizeof(szTemplate));
//				sprintf(szTemplate, "%08lu", pobTran->srBRec.lnHGRefundLackPoint);
//				memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 8);
//			}
//
//			inPacketSizes += 8;
//			/* HG Balance Point (8 Byte) 剩餘點數 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_INQUIRY_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2))
//			{
//				memset(szTemplate, 0x00, sizeof(szTemplate));
//				sprintf(szTemplate, "%08lu", pobTran->srBRec.lnHGBalancePoint);
//				memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 8);
//			}
//
//			inPacketSizes += 8;
//			/* HG Reserve (10 Byte) */
//			inPacketSizes += 10;
//		}
//		else
			inPacketSizes += 78;
//	}
        
//	if (gbBarCodeECRBit == VS_TRUE)
//	{
//		/* Üny 交易碼 */
//		if (pobTran->srBRec.uszUnyTransBit == VS_TRUE)
//		{
//			/* 一維或二維條碼資料長度 */
//			inBarCodeLen = strlen(pobTran->srBRec.szBarCodeData);
//			inPacketSizes += 3;
//			/* 一維或二維條碼資料內容 */
//			inPacketSizes += inBarCodeLen;
//
//			/* 補滿至980 */
//			inPacketSizes = 980;
//
//			/* Üny 交易碼 */
//			/* 除結束卡號查詢不用，其他Uny交易都要 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2))
//			{
//
//			}
//			else
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srBRec.szUnyTransCode, 20);
//			}
//			inPacketSizes += 20;
//		}
//		else if (pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//		{
//			/* 電子錢包資料長度 */
//			inBarCodeLen = strlen(pobTran->srBRec.szBarCodeData);
//			sprintf(&szDataBuffer[inPacketSizes], "%03d", 184);
//			inPacketSizes += 3;
//			/* 電子錢包交易條碼 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srBRec.szBarCodeData, inBarCodeLen);
//			}
//			inPacketSizes += 25;
//			
//			/* 電子錢包業者特店代號 */
//			memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_ChannelId, 50);
//			inPacketSizes += 50;
//			/* 電子錢包交易日期時間 */
//			memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_PaymentDate, 24);
//			inPacketSizes += 24;
//			/* 消費者支付工具 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_PayMethod, 1);
//			}
//			inPacketSizes += 1;
//			/* 訂單狀態 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_TransactionType, 2);
//			}
//			inPacketSizes += 2;
//			/* 付款交易編號 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_PaymentNo, 30);
//			}
//			inPacketSizes += 30;
//			/* 是否為重複付款請求 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_IsDuplicatedPaymentRequest, 1);
//			}
//			inPacketSizes += 1;
//			/* 聯名卡代碼 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_CobrandedCode, 11);
//			}
//			inPacketSizes += 11;
//			/* 載具類型 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_EinvoiceCarrierType, 6);
//			}
//			inPacketSizes += 6;
//			/* 折抵金額/退還折抵金額 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_DiscountAmt, 10);
//			}
//			else if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_DiscountAmt, 10);
//			}
//			inPacketSizes += 10;
//			/* 門市代碼 */
//			memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_Store_no, 24);
//			inPacketSizes += 24;
//			/* 保留 */
//			inPacketSizes += 413;
//		}
//		else if (pobTran->srBRec.uszUPlanECRBit == VS_TRUE)
//		{
//			/* 一維或二維條碼資料長度 */
//			inBarCodeLen = strlen(pobTran->srBRec.szBarCodeData);
//			inPacketSizes += 3;
//			/* 一維或二維條碼資料內容 */
//			inPacketSizes += inBarCodeLen;
//			
//			/* 補滿至980 */
//			inPacketSizes = 980;
//			
//			inPacketSizes += 20;
//		}
//	}
        printf("inPacketSizes : %d\n",inPacketSizes);
	return (VS_SUCCESS);
}


int inCusECR_8N1_Standard_Pack(TRANSACTION_OBJECT *pobTran, ECR_TABLE * srECROb, char *szDataBuffer)
{
	int	inRetVal = VS_ERROR;
	int	i = 0, inCardLen = 0;
	int	inPacketSizes = 0;
	int	inLen = 0;
	int	inBarCodeLen = 0;
	char	szTemplate[100 + 1];
	char	szHash[44 + 1];
	char	szFESMode[2 + 1];
	char	szTemp[8 + 1] = {0};
	char	szTemp2[8 + 1] = {0};
        char	szCustomerIndicator[3 + 1] = {0};

//        memset(szCustomerIndicator, 0x00, sizeof(szCustomerIndicator));
//        inGetCustomIndicator(szCustomerIndicator);
        memcpy( srECROb->srTransData.szECRIndicator,"I",1);
	/* ECR Indicator (1 Byte) */
	memcpy(&szDataBuffer[inPacketSizes], srECROb->srTransData.szECRIndicator, 1);
	inPacketSizes ++;
	/* ECR Version Date (6 Byte) */
	memcpy(&szDataBuffer[inPacketSizes], "260119", 6);
	inPacketSizes += 6;
	/* Trans Type Indicator (1 Byte) */
	inPacketSizes ++;
	/* Trans Type (2 Byte) */
//	memcpy(&szDataBuffer[inPacketSizes], srECROb->srTransData.szTransType, 2);
        memcpy(srECROb->srTransData.szTransType,_ECR_8N1_SALE_,strlen(_ECR_8N1_SALE_));
	memcpy(&szDataBuffer[inPacketSizes], srECROb->srTransData.szTransType, 2);
	inPacketSizes += 2;
	/* CUP Indicator (1 Byte) */
//	if (!memcmp(srECROb->srTransData.szField_05, _ECR_TRUST_INDICATOR_, 1) || pobTran->srTrustRec.uszTrustTransBit == VS_TRUE)
//	{
//		memcpy(&szDataBuffer[inPacketSizes], _ECR_TRUST_INDICATOR_, 1);
//	}
//	else if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2)	||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2)			||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2)			||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_, 2)			||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)		||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_, 2)		||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)	||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2)	||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2)		||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_SALE_, 2)		||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)	||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)		||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//	{
//		if (!memcmp(srECROb->srTransData.szField_05, "C", 1) || pobTran->srBRec.uszCUPTransBit == VS_TRUE)
//			memcpy(&szDataBuffer[inPacketSizes], "C", 1);
//		else if (!memcmp(srECROb->srTransData.szField_05, "S", 1) || pobTran->srBRec.uszFiscTransBit == VS_TRUE)
//			memcpy(&szDataBuffer[inPacketSizes], "S", 1);
//		else if (!memcmp(srECROb->srTransData.szField_05, "E", 1) || pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//			memcpy(&szDataBuffer[inPacketSizes], "E", 1);
//		else if (!memcmp(srECROb->srTransData.szField_05, _ECR_EW_INDICATOR_, 1) || pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//			memcpy(&szDataBuffer[inPacketSizes], _ECR_EW_INDICATOR_, 1);
//		else
		        memcpy(&szDataBuffer[inPacketSizes], "N", 1);
//	}

	inPacketSizes ++;

	/* HOST ID (2 Byte) */
	if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_OFFLINE_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2)				||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_INQUIRY_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_SALE_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_AWARD_REDEEM_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
	{
//		memset(szTemplate, 0x00, sizeof(szTemplate));
//		inGetTRTFileName(szTemplate);
		
//		if (!memcmp(szTemplate, _TRT_FILE_NAME_CREDIT_, strlen(_TRT_FILE_NAME_CREDIT_)))
			memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_HOSTID_NCCC_, 2);
//		/* 2018/5/8 下午 2:10 DFS需求不再使用大來主機 */
//		else if (!memcmp(szTemplate, _TRT_FILE_NAME_HG_, strlen(_TRT_FILE_NAME_HG_)))
//			memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_HOSTID_HG_, 2);
//		else if (!memcmp(szTemplate, _TRT_FILE_NAME_DCC_, strlen(_TRT_FILE_NAME_DCC_)))
//			memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_HOSTID_DCC_, 2);
//		else if (!memcmp(szTemplate, _TRT_FILE_NAME_ESVC_, strlen(_TRT_FILE_NAME_ESVC_)))
//			memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_HOSTID_ESVC_, 2);
//		else if (!memcmp(szTemplate, _TRT_FILE_NAME_EW_, strlen(_TRT_FILE_NAME_EW_)))
//			memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_HOSTID_EW_, 2);
//		else if (!memcmp(szTemplate, _TRT_FILE_NAME_TRUST_, strlen(_TRT_FILE_NAME_TRUST_)))
//			memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_HOSTID_TRUST_, 2);
	}

	inPacketSizes += 2;

//	if (pobTran->srTrustRec.uszTrustTransBit == VS_TRUE)
//	{
//		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2))
//		{
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			sprintf(szTemplate, "%s", pobTran->srTrustRec.szTrustRRN);
//			memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//		}
//                inPacketSizes += 12;
//		
//		/* Reserved */
//		inPacketSizes += 17;
//	}
//	else
//	{
//		/* Receipt No (6 Byte) */
		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_OFFLINE_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2)				||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)	||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_INQUIRY_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_SALE_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2)	||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2)	||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)	||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_AWARD_REDEEM_, 2)		||
		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
		{
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//			{
//				sprintf(szTemplate, "%06ld", pobTran->srTRec.lnInvNum);
//			}
//			else
//			{
//				sprintf(szTemplate, "%06ld", pobTran->srBRec.lnOrgInvNum);
//			}
//			memcpy(&szDataBuffer[inPacketSizes], szTemplate, 6);
                        memcpy(&szDataBuffer[inPacketSizes], "000777", 6);
		}

		inPacketSizes += 6;

		/* Card No (19 Byte) */
//		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SETTLEMENT_, 2)			||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_DETAIL_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_TOTAL_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_DETAIL_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_TOTAL_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EDC_REBOOT_, 2)			||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_INQUIRY_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2)		||
//		    (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2) && !(!memcmp(pobTran->szL3_AwardWay, "4", 1) || !memcmp(pobTran->szL3_AwardWay, "5", 1)))	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_AWARD_REDEEM_, 2)		||
//		    pobTran->srBRec.uszHappyGoSingle == VS_TRUE)
//		{
//			/* 優惠兌換先設定不回傳卡號 */
//			inPacketSizes += 19;
//		}
//		else
//		{
//			if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//			{
//				memset(szTemplate, 0x00, sizeof(szTemplate));
//
//				if (pobTran->uszCardInquiryFirstBit == VS_TRUE)
//				{
//					if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ECC_)
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						inLen = strlen(pobTran->srTRec.srECCRec.szCardID);
//						memcpy(szTemplate, pobTran->srTRec.srECCRec.szCardID, inLen);
//					}
//					else
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						inLen = strlen(pobTran->srTRec.szUID);
//						memcpy(szTemplate, pobTran->srTRec.szUID, inLen);
//					}
//				}
//				else
//				{
//					/* 一卡通：遮掩後2 碼，遮掩字元為”*” */
//					if (pobTran->srTRec.inTicketType == _TICKET_TYPE_IPASS_)
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						inLen = strlen(pobTran->srTRec.szUID);
//						memcpy(szTemplate, pobTran->srTRec.szUID, inLen);
//						szTemplate[inLen - 1] = 0x2A;
//						szTemplate[inLen - 2] = 0x2A;
//					}
//					/*	1.簽單卡號列印邏輯
//						<T4800>票卡版號(Purse Version Number)=0，<T0200>列印晶片卡號(Card Id)
//						<T0200>20 bytes，14th,15th,16th隱碼，17th,18th,19th,20th顯示
//						<T4800>票卡版號(Purse Version Number)<>0，<T0211>列印外觀卡號(Purse Id)
//						<T0211>16 bytes，6th,7th,8th,9th,10th,11th隱碼，12th,13th,14th,15th,16th顯示
//						，遮掩字元為”*”
//						以<T0200>悠遊卡卡號遮掩從後往前算第五碼，第六碼，第七碼遮掩
//						範例:以8碼卡號為例，卡號11651733，1,6,5要遮掩，實際列印為1***1733
//						1th 2th 3th 4th 5th 6th 7th 8th 9th 10th 11th 12th 13th 14th 15th 16th 17th
//						0 0 0 0 0 0 0 0 0 1 1 6 5 1 7 3 3
//						0 0 0 0 0 0 0 0 0 1 * * * 1 7 3 3
//						範例:以10碼卡號為例，卡號6611651733，1,6,5要遮掩，實際列印為661***1733
//						1th 2th 3th 4th 5th 6th 7th 8th 9th 10th 11th 12th 13th 14th 15th 16th 17th
//						0 0 0 0 0 0 0 6 6 1 1 6 5 1 7 3 3
//						0 0 0 0 0 0 0 6 6 1 * * * 1 7 3 3
//
//						2. 電子發票載具卡號為T0200 (不遮掩)
//					*/
//					else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ECC_)
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						inLen = strlen(pobTran->srTRec.szUID);
//						memcpy(szTemplate, pobTran->srTRec.szUID, inLen);
//
//						if (atoi(pobTran->srTRec.srECCRec.szPurseVersionNumber) == 0)
//						{
//							szTemplate[inLen - 5] = 0x2A;
//							szTemplate[inLen - 6] = 0x2A;
//							szTemplate[inLen - 7] = 0x2A;
//						}
//						else
//						{
//							/* 全部16 第6-11隱碼 */
//							szTemplate[inLen - 6] = 0x2A;
//							szTemplate[inLen - 7] = 0x2A;
//							szTemplate[inLen - 8] = 0x2A;
//							szTemplate[inLen - 9] = 0x2A;
//							szTemplate[inLen - 10] = 0x2A;
//							szTemplate[inLen - 11] = 0x2A;
//						}
//					}
//					else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ICASH_)
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						inLen = strlen(pobTran->srTRec.szUID);
//						memcpy(szTemplate, pobTran->srTRec.szUID, inLen);
//						/* 愛金卡：遮掩9-12 碼，遮掩字元為"*" */
//						szTemplate[8] = 0x2A;
//						szTemplate[9] = 0x2A;
//						szTemplate[10] = 0x2A;
//						szTemplate[11] = 0x2A;
//					}
//				}
//
//				memcpy(&szDataBuffer[inPacketSizes], szTemplate, strlen(szTemplate));
//			}
//			/* (需求單-109455)-Üny實體掃碼需求 by Russell 2021/9/3 下午 6:18 */
//			/* 不參考TMS遮掩開關 */
//			else if (pobTran->srBRec.uszUnyTransBit == VS_TRUE)
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srBRec.szPAN, strlen(pobTran->srBRec.szPAN));
//			}
//			else if (pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//			{
//				if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2))
//				{
//					/* 悠遊付：當消費者支付工具為”1：信用卡”時，遮掩的信用卡卡號 */
//					/* 全支付：遮掩的信用卡卡號或帳戶 */
//					memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_MaskCreditCardNo, strlen(pobTran->srEWRec.szEW_MaskCreditCardNo));
//				}
//				else if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//				{
//					/* 悠遊付：當消費者支付工具為”1：信用卡”時，遮掩的信用卡卡號 */
//					/* icash Pay：卡號前六、後四碼，中間*號遮蔽 */
//					/* 全支付：遮掩的信用卡卡號或帳戶 */
//					memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_MaskCreditCardNo, strlen(pobTran->srEWRec.szEW_MaskCreditCardNo));
//				}
//			}
//			else
//			{
//				/* 卡號是否遮掩 */
//				inRetVal = inECR_CardNoTruncateDecision(pobTran);
//				/* 要遮卡號 */
//				if (inRetVal == VS_SUCCESS)
//				{
//					memset(szTemplate, 0x00, sizeof(szTemplate));
//					strcpy(szTemplate, pobTran->srBRec.szPAN);
//
//					/* HAPPG_GO 卡不掩飾 */
//					if (!memcmp(&pobTran->srBRec.szPAN[0], "9552", 4))
//					{
//
//					}
//					else
//					{
//						/* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//						/* 卡號長度 */
//						inCardLen = strlen(szTemplate);
//
//						if (!memcmp(szCustomerIndicator, _CUSTOMER_INDICATOR_088_TINTIN_, _CUSTOMER_INDICATOR_SIZE_))
//						{
//							/* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//							for (i = 8; i < (inCardLen - 4); i ++)
//								szTemplate[i] = '*';
//						}
//						else if (!memcmp(szCustomerIndicator, _CUSTOMER_INDICATOR_124_EVER_RICH_, _CUSTOMER_INDICATOR_SIZE_))
//						{
//							/* 第一段查詢交易，回覆信用卡前8後2卡號 */
//							if (pobTran->uszCardInquiryFirstBit == VS_TRUE ||
//							    pobTran->uszCardInquirysSecondBit == VS_TRUE)
//							{
//								for (i = 8; i < (inCardLen - 2); i ++)
//									szTemplate[i] = '*';
//							}
//							else
//							{
//								for (i = 6; i < (inCardLen - 4); i ++)
//									szTemplate[i] = '*';
//							}
//						}
//						else
//						{    
//							/* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//							for (i = 6; i < (inCardLen - 4); i ++)
//								szTemplate[i] = '*';
//						}
//					}
//					memcpy(&szDataBuffer[inPacketSizes], szTemplate, strlen(szTemplate));
//				}
//				else
//				{
//					memcpy(&szDataBuffer[inPacketSizes], pobTran->srBRec.szPAN, strlen(pobTran->srBRec.szPAN));
//				}
//			}
			inPacketSizes += 19;
//		}
//
//		/* Card Expire Date (4 Byte) */
//		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SETTLEMENT_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_DETAIL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_TOTAL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_DETAIL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_TOTAL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EDC_REBOOT_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_INQUIRY_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2))
//		{
//			inPacketSizes += 4;
//		}
//		else if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//		{
//			inPacketSizes += 4;
//		}
//		/* (需求單-109455)-Üny實體掃碼需求 by Russell 2021/9/3 下午 6:18 */
//		/* Uny交易不回傳有效期 */
//		else if (pobTran->srBRec.uszUnyTransBit == VS_TRUE)
//		{
//			inPacketSizes += 4;
//		}
//		/* 電子錢包交易不回傳有效期 */
//		/* (需求單-111155)-電子錢包業者收付訊息整合平台需求 by Russell 2023/7/24 下午 6:17 */
//		else if (pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//		{
//			inPacketSizes += 4;
//		}
//		else
//		{
//			/* 有效期是否回傳 */
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			inGetECRExpDateReturnEnable(szTemplate);
//
//			/* 要回傳有效期 */
//			if (!memcmp(&szTemplate, "Y", 1))
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srBRec.szExpDate, strlen(pobTran->srBRec.szExpDate));
//
			inPacketSizes += 4;
//		}
//	}

	/* Trans Amount (12 Byte) */
//	if (pobTran->srTrustRec.uszTrustTransBit == VS_TRUE)
//	{
//		/* 因為信託交易必定要回交易金額，所以必須擺在最上面 */
//                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2))
//		{
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", pobTran->srTrustRec.lnTxnAmount);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//		}
//
//		inPacketSizes += 12;
//	}
//	else if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2)			||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2)			||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)	||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2)	||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SETTLEMENT_, 2)			||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_DETAIL_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_TOTAL_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_DETAIL_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_TOTAL_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EDC_REBOOT_, 2)			||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_INQUIRY_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2)		||
//		 !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2))
//	{
//	        inPacketSizes += 12;
//	}
//        else if (pobTran->srBRec.uszUPlanTransBit == VS_TRUE)
//        {
//                if (pobTran->srBRec.uszVOIDBit == VS_TRUE)
//                {
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnTxnAmount);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                }
//                else
//                {
//                    if (pobTran->srBRec.lnCUPUPlanDiscountedAmount > 0)
//                    {
//                            memset(szTemplate, 0x00, sizeof(szTemplate));
//                            sprintf(szTemplate, "%010lu00", (pobTran->srBRec.lnTxnAmount + pobTran->srBRec.lnCUPUPlanPreferentialAmount));
//                            memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                    }
//                    else
//                    {
//                            memset(szTemplate, 0x00, sizeof(szTemplate));
//                            sprintf(szTemplate, "%010lu00", (pobTran->srBRec.lnTxnAmount + pobTran->srBRec.lnCUPUPlanPreferentialAmount));
//                            memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                    }
//                }
//                inPacketSizes += 12;
//        }
//        else
//        {
//                if (pobTran->srTRec.lnTxnAmount != 0)
//                {
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", pobTran->srTRec.lnTxnAmount);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//
//                        inPacketSizes += 12;
//                }
//                else if (pobTran->srBRec.lnTxnAmount != 0)
//                {
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnTxnAmount);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//
//                        inPacketSizes += 12;
//                }
//                else if (pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//                {
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu", pobTran->srEWRec.lnTxnAmount);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 10);
//                        inPacketSizes += 10;
//
//                        memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_Decimal, 2);
//                        inPacketSizes += 2;
//                }
//                else
//                {
//                        inPacketSizes += 12;
//                }
//        }
            memcpy(&szDataBuffer[inPacketSizes], "123215432100", 12);
            inPacketSizes += 12;
	/* Trans Date (6 Byte) & Trans Time (6 Byte) */
	if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_OFFLINE_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2)				||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SETTLEMENT_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_INQUIRY_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_SALE_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_AWARD_REDEEM_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ECHO_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
	{
//		if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//		{
//			/* Trans Date */
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			strcat(szTemplate, pobTran->srTRec.szDate);
//			memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 6);
//			inPacketSizes += 6;
//
//			/* Trans Time */
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			strcat(szTemplate, pobTran->srTRec.szTime);
//			memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 6);
//			inPacketSizes += 6;
//		}
//		else
//		{
//			/* Trans Date */
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			strcat(szTemplate, pobTran->srBRec.szDate);
//			memcpy(&szDataBuffer[inPacketSizes], &szTemplate[2], 6);
			memcpy(&szDataBuffer[inPacketSizes], "260119", 6);
			inPacketSizes += 6;
//
//			/* Trans Time */
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			strcat(szTemplate, pobTran->srBRec.szTime);
//			memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 6);
			memcpy(&szDataBuffer[inPacketSizes], "103259", 6);
			inPacketSizes += 6;
//		}
	}
	else
	{
		inPacketSizes += 6;
		inPacketSizes += 6;
	}

	/* Approval No (9 Byte) */
	if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_OFFLINE_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2)				||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_SALE_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)	||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)		||
	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2))
	{
//		if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//		{
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			strcpy(szTemplate, pobTran->srTRec.szAuthCode);
//			inFunc_PAD_ASCII(szTemplate, szTemplate, ' ', 9, _PADDING_RIGHT_);
//			memcpy(&szDataBuffer[inPacketSizes], szTemplate, 9);
//		}
//		/* 電子錢包交易不回傳授權碼 */
//		/* (需求單-111155)-電子錢包業者收付訊息整合平台需求 by Russell 2023/7/24 下午 6:17 */
//		else if (pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//		{
//			
//		}
//		else
//		{
			memset(szTemplate, 0x00, sizeof(szTemplate));
//			strcpy(szTemplate, pobTran->srBRec.szAuthCode);
			strcpy(szTemplate, "00");
			inFunc_PAD_ASCII(szTemplate, szTemplate, ' ', 9, _PADDING_RIGHT_);
			memcpy(&szDataBuffer[inPacketSizes], szTemplate, 9);
//		}
	}

	inPacketSizes += 9;

	/* Wave Card Indicator (1 Byte) */
//	if (!memcmp(&srECROb->srTransData.szECRIndicator[0], "Q", 1))
//	{
//		/* 電子錢包交易不回傳Wave Card Indicator */
//		/* (需求單-111155)-電子錢包業者收付訊息整合平台需求 by Russell 2023/7/24 下午 6:17 */
//		/* Indicator為Q目前不送此欄位 */
//	}
//	else
//	{
//		if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//		{
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2))
//			{
//				if (pobTran->srTRec.inTicketType == _TICKET_TYPE_IPASS_)
//				{
//					memcpy(&szDataBuffer[inPacketSizes], "P", 1);
//				}
//				else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ECC_)
//				{
//					memcpy(&szDataBuffer[inPacketSizes], "Z", 1);
//				}
//				else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ICASH_)
//				{
//					memcpy(&szDataBuffer[inPacketSizes], "G", 1);
//				}
//				else
//				{
//					memcpy(&szDataBuffer[inPacketSizes], "O", 1);
//				}
//			}
//		}
//		else
//		{
			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2)	||
			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2)	||
			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2))
			{
//				if (pobTran->srBRec.uszContactlessBit == VS_TRUE)
//				{
//					/* 規格未寫M 跟單機同步 */
//					memset(szTemplate, 0x00, sizeof(szTemplate));
//					strcpy(szTemplate, pobTran->srBRec.szCardLabel);
//
//					if (!memcmp(szTemplate, _CARD_TYPE_VISA_, strlen(_CARD_TYPE_VISA_)))
						memcpy(&szDataBuffer[inPacketSizes], "V", 1);
//					else if (!memcmp(szTemplate, _CARD_TYPE_MASTERCARD_, strlen(_CARD_TYPE_MASTERCARD_)))
//						memcpy(&szDataBuffer[inPacketSizes], "M", 1);
//					else if (!memcmp(szTemplate, _CARD_TYPE_JCB_, strlen(_CARD_TYPE_JCB_)))
//						memcpy(&szDataBuffer[inPacketSizes], "J", 1);
//					else if (!memcmp(szTemplate, _CARD_TYPE_CUP_, strlen(_CARD_TYPE_CUP_)))
//						memcpy(&szDataBuffer[inPacketSizes], "C", 1);
//					else if (!memcmp(szTemplate, _CARD_TYPE_AMEX_, strlen(_CARD_TYPE_AMEX_)))
//						memcpy(&szDataBuffer[inPacketSizes], "A", 1);
//					/* (需求單 - 106349) 2018/10/31 下午 3:41 by Russell */
//					else if (!memcmp(szTemplate, _CARD_TYPE_DINERS_, strlen(_CARD_TYPE_DINERS_)) ||
//						 pobTran->srBRec.uszUCARDTransBit == VS_TRUE)
//					{
//						if (pobTran->srBRec.uszUCARDTransBit == VS_TRUE)
//						{
//							memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szMCP_BANKID[4], 1);
//						}
//						else
//						{
//							memcpy(&szDataBuffer[inPacketSizes], "D", 1);
//						}
//					}
//					else
//						memcpy(&szDataBuffer[inPacketSizes], "O", 1);
//				}
			}
//		}
//	}
	inPacketSizes ++;

	/* ECR Response Code (4 Byte) */
	memcpy(&szDataBuffer[inPacketSizes], "0000", 4);
	
	inPacketSizes += 4;

	/* Merchant ID (15 Byte) & Terminal ID (8 Byte) */
//	if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_DETAIL_, 2) ||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_TOTAL_, 2) ||
//	    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SETTLEMENT_, 2))
//	{
//		inPacketSizes += 15;
//		inPacketSizes += 8;
//	}
//	else
//	{
		/* Merchant ID (15 Byte) */
		memset(szTemplate, 0x00, sizeof(szTemplate));
                strcpy(szTemplate,"1213141");
//		inGetMerchantID(szTemplate);
		inFunc_PAD_ASCII(szTemplate, szTemplate, ' ', 15, _PADDING_RIGHT_);
		memcpy(&szDataBuffer[inPacketSizes], szTemplate, 15);
		inPacketSizes += 15;
		/* Terminal ID (8 Byte) */
		memset(szTemplate, 0x00, sizeof(szTemplate));
                strcpy(szTemplate,"3456");
//		inGetTerminalID(szTemplate);
		inFunc_PAD_ASCII(szTemplate, szTemplate, ' ', 8, _PADDING_RIGHT_);
		memcpy(&szDataBuffer[inPacketSizes], szTemplate, 8);
		inPacketSizes += 8;
//	}
//
	/* Exp Amount (12 Byte) */
//	memset(szTemplate, 0x00, sizeof(szTemplate));
//	inGetTRTFileName(szTemplate);
//        if (!memcmp(szTemplate, _TRT_FILE_NAME_DCC_, _TRT_FILE_NAME_DCC_LEN_) && !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2))
//        {
//                if (!memcmp(&pobTran->srBRec.szDCC_FCMU[0], "0", 1))
//                {
//        	        memset(szTemplate, 0x00, sizeof(szTemplate));
//        		sprintf(szTemplate, "%010ld00", atol(pobTran->srBRec.szDCC_TIPFCA));
//        		memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//        	}
//                else if (!memcmp(&pobTran->srBRec.szDCC_FCMU[0], "1", 1))
//                {
//        	        memset(szTemplate, 0x00, sizeof(szTemplate));
//        		sprintf(szTemplate, "%011ld0", atol(pobTran->srBRec.szDCC_TIPFCA));
//        		memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//        	}
//        	else if (!memcmp(&pobTran->srBRec.szDCC_FCMU[0], "2", 1))
//                {
//        	        memset(szTemplate, 0x00, sizeof(szTemplate));
//        		sprintf(szTemplate, "%012ld", atol(pobTran->srBRec.szDCC_TIPFCA));
//        		memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//        	}
//        }
//        else if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2))
//        {
//		memset(szTemplate, 0x00, sizeof(szTemplate));
//		sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnTipTxnAmount);
//		memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//	}

	inPacketSizes += 12;
	
	/* Store Id (18 Byte) */
	inPacketSizes += 18;

//        if (pobTran->srTrustRec.uszTrustTransBit == VS_TRUE)
//        {
//                /* 信託 */
//                /* 銷帳編號 */
//                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2))
//                {
//                        if (strlen(pobTran->srTrustRec.szReconciliationNo) > 0)
//                        {
//                                memcpy(&szDataBuffer[inPacketSizes], &pobTran->srTrustRec.szReconciliationNo[0], 20);
//                        }
//                }
//                inPacketSizes += 20;
//                /* 信託機構代碼 */
//                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2))
//                {
//                        memcpy(&szDataBuffer[inPacketSizes], &pobTran->srTrustRec.szTrustInstitutionCode[0], 7);
//                }
//                inPacketSizes += 7;
//                /* 交換平台序號 */
//                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2))
//                {
//                        memcpy(&szDataBuffer[inPacketSizes], &pobTran->srTrustRec.szExchangeTxSerialNumber[0], 32);
//                }
//                inPacketSizes += 32;
//                /* 主機回復中文說明 */
//                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2))
//                {
//                        memcpy(&szDataBuffer[inPacketSizes], &pobTran->szHostResponseMessageBIG5[0], 20);
//                }
//                inPacketSizes += 20;
//                /* 信託原交易日期 */
//                inPacketSizes += 4;
//                /* 保留 */
//                inPacketSizes += 4;
//        }
//        else
//        {
//                /* 處理紅利、分期 */
//                /* Installment / Redeem Indictor (1 Byte) */
//                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)		||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)		||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)	||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2) ||
//                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2))
//                {
//                        if (pobTran->srBRec.uszInstallmentBit == VS_TRUE)
//                                memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szInstallmentIndicator[0], 1);
//                        else if (pobTran->srBRec.uszRedeemBit == VS_TRUE)
//                                memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szRedeemIndicator[0], 1);
//                }
//
//                /* 優惠兌換方式 */
//                /* 1.條碼兌換 2.卡號兌換 */
//                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2))
//                {
//                        if (!memcmp(pobTran->szL3_AwardWay, "1", 1)	||
//                            !memcmp(pobTran->szL3_AwardWay, "2", 1)	||
//                            !memcmp(pobTran->szL3_AwardWay, "3", 1))
//                        {
//                                memcpy(&szDataBuffer[inPacketSizes], "1", 1);
//                        }
//                        else if (!memcmp(pobTran->szL3_AwardWay, "4", 1)	||
//                                 !memcmp(pobTran->szL3_AwardWay, "5", 1))
//                        {
//                                memcpy(&szDataBuffer[inPacketSizes], "2", 1);
//                        }
//                }
//                else if(!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_AWARD_REDEEM_, 2))
//                {
//                        /* (1= 條碼兌換， 2= 卡號兌換 )*/
//                        /* 取消優惠兌換只接受 條碼兌換 。 */
//                        memcpy(&szDataBuffer[inPacketSizes], "1", 1);
//                }
                inPacketSizes ++;
//
//                /* 處理紅利扣抵 */
//                if ((!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)		||
//                     !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)	||
//                     !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)	||
//                     !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2)) && pobTran->srBRec.uszRedeemBit == VS_TRUE)
//                {
//                        /* 支付金額 RDM Paid Amt (12 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", (pobTran->srBRec.lnRedemptionPaidCreditAmount + pobTran->srBRec.lnTipTxnAmount));
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                        inPacketSizes += 12;
//                        /* 紅利扣抵點數 RDM Point (8 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%08lu", pobTran->srBRec.lnRedemptionPoints);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 8);
//                        inPacketSizes += 8;
//                        /* 紅利剩餘點數 Points Of Balance (8 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%08lu", pobTran->srBRec.lnRedemptionPointsBalance);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 8);
//                        inPacketSizes += 8;
//                        /* 紅利扣抵金額 Redeem Amt (12 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", (pobTran->srBRec.lnTxnAmount - pobTran->srBRec.lnRedemptionPaidCreditAmount));
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                        inPacketSizes += 12;
//                }
//                else if (pobTran->srBRec.uszUPlanTransBit == VS_TRUE)
//                {
//                        if (pobTran->srBRec.uszVOIDBit == VS_TRUE)
//                        {
//                                inPacketSizes += 40;
//                        }
//                        else
//                        {
//                                /* 銀聯優計劃：優惠後金額(含小數2 位) */
//                                memset(szTemplate, 0x00, sizeof(szTemplate));
//                                sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnCUPUPlanDiscountedAmount);
//                                memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                                inPacketSizes += 12;
//
//                                /* reserved */
//                                inPacketSizes += 8;
//
//                                /* reserved */
//                                inPacketSizes += 8;
//
//                                /* 銀聯優計劃：優惠金額(含小數2 位) */
//                                memset(szTemplate, 0x00, sizeof(szTemplate));
//                                sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnCUPUPlanPreferentialAmount);
//                                memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                                inPacketSizes += 12;
//                        }
//                }
//                else
//                {
                        inPacketSizes += 40;
//                }
//
//                /* 處理分期交易 */
//                if ((!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)	||
//                     !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)	||
//                     !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)	||
//                     !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2)) && pobTran->srBRec.uszInstallmentBit == VS_TRUE)
//                {
//                        /* 分期期數 Installment Period (2 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%02ld", pobTran->srBRec.lnInstallmentPeriod);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 2);
//                        inPacketSizes += 2;
//                        /* 首期金額 Down Payment (12 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnInstallmentDownPayment);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                        inPacketSizes += 12;
//                        /* 每期金額 Installment Payment Amt (12 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnInstallmentPayment);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                        inPacketSizes += 12;
//                        /* 分期手續費 Formlity Fee (12 Byte) */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnInstallmentFormalityFee);
//                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 12);
//                        inPacketSizes += 12;
//                }
//                else if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//                {
//                        inPacketSizes += 2;
//
//                        /* 交易前餘額 */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2)	||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2)	||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)		||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2)		||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_TOTAL_, 2)		||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_DETAIL_, 2))
//                        {
//                                /* 查餘額不帶此欄位 */
//                                /* 啟動卡號查詢不帶此欄位 */
//                                sprintf(szTemplate, "            ");
//                        }
//                        else
//                        {
//                                if (pobTran->srTRec.inTicketType == _TICKET_TYPE_IPASS_)
//                                {
//                                        if (pobTran->srTRec.lnFinalBeforeAmt > 100000)
//                                        {
//                                                sprintf(szTemplate, "-%09lu00", (pobTran->srTRec.lnFinalBeforeAmt - 100000));
//                                        }
//                                        else
//                                        {
//                                                /* 交易前餘額要帶未加值的金額 */
//                                                if ((pobTran->srTRec.lnCardRemainAmount - pobTran->srTRec.lnTotalTopUpAmount) < 0)
//                                                        sprintf(szTemplate, "-%09lu00", (unsigned long)(0 - (pobTran->srTRec.lnCardRemainAmount - pobTran->srTRec.lnTotalTopUpAmount)));
//                                                else
//                                                        sprintf(szTemplate, "+%09lu00", (unsigned long)pobTran->srTRec.lnCardRemainAmount - pobTran->srTRec.lnTotalTopUpAmount);
//                                        }
//                                }
//                                else
//                                {
//                                        if (pobTran->srTRec.lnFinalBeforeAmt < 0)
//                                        {
//                                                sprintf(szTemplate, "-%09lu00", (0 - pobTran->srTRec.lnFinalBeforeAmt));
//                                        }
//                                        else
//                                        {
//                                                sprintf(szTemplate, "+%09lu00", pobTran->srTRec.lnFinalBeforeAmt);
//                                        }
//                                }
//
//                        }
//
//                        memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//                        inPacketSizes += 12;
//
//                        /* 交易後餘額 */
//                        memset(szTemplate, 0x00, sizeof(szTemplate));
//                        if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2)	||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)		||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2)		||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_TOTAL_, 2)		||
//                            !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_DETAIL_, 2))
//                        {
//                                /* 啟動卡號查詢不帶此欄位 */
//                                sprintf(szTemplate, "            ");
//                        }
//                        else if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2))
//                        {
//                                if (pobTran->srTRec.lnCardRemainAmount < 0)
//                                {
//                                        sprintf(szTemplate, "-%09lu00", (unsigned long)(0 - pobTran->srTRec.lnCardRemainAmount));
//                                }
//                                else
//                                {
//                                        sprintf(szTemplate, "+%09lu00", (unsigned long)pobTran->srTRec.lnCardRemainAmount);
//                                }
//                        }
//                        else
//                        {
//                                if (pobTran->srTRec.inTicketType == _TICKET_TYPE_IPASS_)
//                                {
//                                        if (pobTran->srTRec.lnFinalAfterAmt > 100000)
//                                                sprintf(szTemplate, "-%09lu00", (unsigned long)(pobTran->srTRec.lnFinalAfterAmt - 100000));
//                                        else
//                                                sprintf(szTemplate, "+%09lu00", (unsigned long)(pobTran->srTRec.lnFinalAfterAmt));
//                                }
//                                else
//                                {
//                                        if (pobTran->srTRec.lnFinalAfterAmt < 0)
//                                                sprintf(szTemplate, "-%09lu00", (unsigned long)(0 - pobTran->srTRec.lnFinalAfterAmt));
//                                        else
//                                                sprintf(szTemplate, "+%09lu00", (unsigned long)(pobTran->srTRec.lnFinalAfterAmt));
//                                }
//                        }
//
//                        memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//                        inPacketSizes += 12;
//
//                        /* 自動加值金額 */
//                        if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2))
//                        {
//                                memset(szTemplate, 0x00, sizeof(szTemplate));
//                                if (pobTran->srTRec.lnTotalTopUpAmount > 0L)
//                                        sprintf(szTemplate, "+%09lu00", (unsigned long)(pobTran->srTRec.lnTotalTopUpAmount));
//                                else
//                                        sprintf(szTemplate, "+%09lu00", (unsigned long)(0));
//
//                                memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//                        }
//                        inPacketSizes += 12;
//                }
//                else
                        inPacketSizes += 38;

                /* Card Type (2 Byte) */
                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2)					||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)							||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2)							||
                    (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2) && pobTran->srBRec.uszHappyGoSingle != VS_TRUE)	||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_OFFLINE_, 2)							||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2)								||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_, 2)							||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)						||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_, 2)						||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)					||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)							||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)						||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)						||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)							||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)						||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)						||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)							||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2)					||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2)						||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2)					||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2)					||
                    ((!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_SALE_, 2)						||
                      !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)						||
                      !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)) && pobTran->srBRec.uszHappyGoMulti == VS_TRUE)	||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
                {
//                        if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//                        {
//                                if (pobTran->srTRec.inTicketType == _TICKET_TYPE_IPASS_)
//                                {
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_IPASS_, 2);
//                                }
//                                else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ECC_)
//                                {
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_ECC_, 2);
//                                }
//                                else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ICASH_)
//                                {
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_ICASH_, 2);
//                                }
//                                else
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_UNKNOWN_, _ECR_8N1_NCCC_CARDTYPE_UNKNOWN_LEN_);
//                        }
//                        else if (pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//                        {
//                                memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_EWIssuerID, _EW_ISSUERID_LEN_);
//                        }
//                        else
//                        {
//                                memset(szTemplate, 0x00, sizeof(szTemplate));
//                                strcpy(szTemplate, pobTran->srBRec.szCardLabel);
//
//                                /* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//                                if (!memcmp(szTemplate, _CARD_TYPE_VISA_, strlen(_CARD_TYPE_VISA_)))
                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_VISA_, 2);
//                                else if (!memcmp(szTemplate, _CARD_TYPE_MASTERCARD_, strlen(_CARD_TYPE_MASTERCARD_)))
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_MASTERCARD_, 2);
//                                else if (!memcmp(szTemplate, _CARD_TYPE_JCB_, strlen(_CARD_TYPE_JCB_)))
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_JCB_, 2);
//                                else if (!memcmp(szTemplate, _CARD_TYPE_AMEX_, strlen(_CARD_TYPE_AMEX_)))
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_AMEX_, 2);
//                                else if (!memcmp(szTemplate, _CARD_TYPE_CUP_, strlen(_CARD_TYPE_CUP_)))
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_CUP_, 2);
//                                else if (!memcmp(szTemplate, _CARD_TYPE_DINERS_, strlen(_CARD_TYPE_DINERS_)) ||
//                                         pobTran->srBRec.uszUCARDTransBit == VS_TRUE)
//                                {
//                                        if (pobTran->srBRec.uszUCARDTransBit == VS_TRUE)
//                                        {
//                                                memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_UCARD_, 2);
//                                        }
//                                        else
//                                        {
//                                                memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_DINERS_, 2);
//                                        }
//                                }
//                                else if (!memcmp(szTemplate, _CARD_TYPE_SMARTPAY_, strlen(_CARD_TYPE_SMARTPAY_)))
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_SMARTPAY_, 2);
//                                else
//                                        memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_NCCC_CARDTYPE_UNKNOWN_, _ECR_8N1_NCCC_CARDTYPE_UNKNOWN_LEN_);
//                        }
                }

                inPacketSizes += 2;

                /* Batch No (6 Byte) */
                if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_OFFLINE_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_TIP_, 2)				||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_CANCEL_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_PREAUTH_COMPLETE_CANCEL_, 2)	||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_REFUND_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_ADJUST_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_REFUND_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_ADJUST_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2)			||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_SALE_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_INSTALLMENT_, 2)	||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REDEMPTION_, 2)	||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)	||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2)		||
                    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
                {
                        memset(szTemplate, 0x00, sizeof(szTemplate));
                        sprintf(szTemplate, "%06ld", 999L);
//                        sprintf(szTemplate, "%06ld", pobTran->srBRec.lnBatchNum);
                        memcpy(&szDataBuffer[inPacketSizes], szTemplate, 6);
                }

                inPacketSizes += 6;
//        }/* 和信託做區別 */

	/* (需求單-111155)-電子錢包業者收付訊息整合平台需求 by Russell 2023/7/24 下午 6:17 */
//	if (pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//	{
//		/* 特店訂單編號 */
//		memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_OrderId, 30);
//		inPacketSizes += 30;
//		
//		/* 保留 */
//		inPacketSizes += 6;
//		
//		/* 電子發票載具號碼 */
//		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)			||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//		{
//			if (strlen(pobTran->srEWRec.szEW_EinvoiceHASH) > 0)
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_EinvoiceHASH, 50);
//			}
//		}
//		inPacketSizes += 50;
//		
//		/* 退貨訂單編號*/
//		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2))
//		{
//			memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_RefundTradeNo, 30);
//		}
//		inPacketSizes += 30;
//		
//		/* 電子錢包業者交易序號 */
//		memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_TransactionId, 30);
//		inPacketSizes += 64;
//	}
//	else
//	{
		/* Start Trans Type (2 Byte) */
//		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_START_CARD_NO_INQUIRY_, 2))
//			memcpy(&szDataBuffer[inPacketSizes], srECROb->srTransData.szStartTransType, 2);

		inPacketSizes += 2;

		/* MP Flag(是否小額交易，MPAS小額交易用) (1 Byte) */
		/* 合併MPAS ECR到標準400 */
//		memset(szFESMode, 0x00, sizeof(szFESMode));
//		inGetNCCCFESMode(szFESMode);
//		if (memcmp(szFESMode, _NCCC_04_MPAS_MODE_, strlen(_NCCC_04_MPAS_MODE_)) == 0)
//		{
//			if (pobTran->srBRec.uszMPASTransBit == VS_TRUE)
				memcpy(&szDataBuffer[inPacketSizes], "M", 1);
//			else
//				memcpy(&szDataBuffer[inPacketSizes], " ", 1);
//		}
		inPacketSizes += 1;

//		if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SETTLEMENT_, 2)		||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_DETAIL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REVIEW_TOTAL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_DETAIL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_MENU_REPORT_TOTAL_, 2)	||
//		    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EDC_REBOOT_, 2))
//		{
			/* 共99Bytes */
			inPacketSizes += 8;
			inPacketSizes += 8;
			inPacketSizes += 12;
//			inPacketSizes += 5;
//			inPacketSizes += 50;
//			inPacketSizes += 6;
//			inPacketSizes += 1;
//			inPacketSizes += 1;
//			inPacketSizes += 3;
//			inPacketSizes += 5;
//		}
//		else if (pobTran->srTRec.uszESVCTransBit == VS_TRUE)
//		{
//			inPacketSizes += 8;
//
//			/* ESVC Origin Date */
//			/* 電票退貨要帶回原交易日期 */
//			if (memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2) == 0)
//			{
//				memset(szTemp, 0x00, sizeof(szTemp));
//				memset(szTemp2, 0x00, sizeof(szTemp2));
//				memcpy(szTemp2, pobTran->srTRec.szTicketRefundDate, 2);
//
//				inNCCC_TICKET_Decide_Year(szTemp, szTemp2);
//				memcpy(&szDataBuffer[inPacketSizes], szTemp, 4); 
//				inPacketSizes += 4;
//
//				memcpy(&szDataBuffer[inPacketSizes], &pobTran->srTRec.szTicketRefundDate[0], 4);
//				inPacketSizes += 4;
//			}
//			else
//			{
//				inPacketSizes += 8;
//			}
//
//			/* 兩段式，第一段不用帶RF Number */
//			if (pobTran->uszCardInquiryFirstBit == VS_TRUE)
//			{
//				inPacketSizes += 12;
//			}
//			else
//			{
//				/* 餘額查詢不用帶RF序號 */
//				if (memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2) == 0)
//				{
//					inPacketSizes += 12;
//				}
//				else
//				{
//					/* RF序號 */
//					memcpy(&szDataBuffer[inPacketSizes], &pobTran->srTRec.szTicketRefundCode[0], 6);
//					inPacketSizes += 12;
//				}
//			}

			/* Pay item */
			inPacketSizes += 5;

//			if (!memcmp(&srECROb->srTransData.szECRIndicator[0], "E", 1))
//			{
//				/* Card No. Hash Value */
//				/* 因為卡號查詢會轉交易別，所以用Flag判斷 */
//				if (pobTran->uszCardInquiryFirstBit == VS_TRUE)
//				{
//					memset(szTemplate, 0x00, sizeof(szTemplate));
//				}
//				else
//				{
//					if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2) ||
//					    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2) ||
//					    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2))
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//					}
//					else
//					{
//						if (pobTran->srTRec.inTicketType == _TICKET_TYPE_IPASS_)
//						{
//							memset(szTemplate, 0x00, sizeof(szTemplate));
//							strcpy(szTemplate, pobTran->srTRec.szUID);
//						}
//						else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ECC_)
//						{
//							/* 【需求單-108240】EDC修改電票需求 悠遊卡指定要T0200 by Russell 2020/6/4 下午 2:02 */
//							memset(szTemplate, 0x00, sizeof(szTemplate));
//							strcpy(szTemplate, pobTran->srTRec.srECCRec.szCardID);
//						}
//						else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ICASH_)
//						{
//							memset(szTemplate, 0x00, sizeof(szTemplate));
//							strcpy(szTemplate, pobTran->srTRec.szUID);	
//						}
//					}
//				}
//			}
//			else
//			{
//				/* Card No. Hash Value */
//				if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_TOP_UP_, 2) ||
//				    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_VOID_TOP_UP_, 2) ||
//				    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_ESVC_BALANCE_INQUIRY_, 2))
//				{
//					memset(szTemplate, 0x00, sizeof(szTemplate));
//				}
//				else
//				{
//					if (pobTran->srTRec.inTicketType == _TICKET_TYPE_IPASS_)
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						strcpy(szTemplate, pobTran->srTRec.szUID);
//					}
//					else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ECC_)
//					{
//						/* 【需求單-108240】EDC修改電票需求 悠遊卡指定要T0200 by Russell 2020/6/4 下午 2:02 */
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						strcpy(szTemplate, pobTran->srTRec.srECCRec.szCardID);
//					}
//					else if (pobTran->srTRec.inTicketType == _TICKET_TYPE_ICASH_)
//					{
//						memset(szTemplate, 0x00, sizeof(szTemplate));
//						strcpy(szTemplate, pobTran->srTRec.szUID);	
//					}
//				}
//			}
//			memcpy(&szDataBuffer[inPacketSizes], szTemplate, strlen(szTemplate));
			inPacketSizes += 50;

			/* MP Response Code */
			inPacketSizes += 6;

			/* ASM award flag */
			/* 【需求單-108215】電票交易支援優惠平台需求 by Russell 2020/5/11 下午 3:53 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2))
//			{
//				if ((pobTran->srTRec.uszRewardL1Bit == VS_TRUE	||
//				     pobTran->srTRec.uszRewardL2Bit == VS_TRUE	||
//				     pobTran->srTRec.uszRewardL5Bit == VS_TRUE)	&&
//				     pobTran->srTRec.uszRewardSuspendBit != VS_TRUE)
//				{
//					memcpy(&szDataBuffer[inPacketSizes], "A", strlen("A"));
//				}
//				else
//				{
					memcpy(&szDataBuffer[inPacketSizes], " ", strlen(" "));
//				}
//			}
//			else
//			{
//				memcpy(&szDataBuffer[inPacketSizes], " ", strlen(" "));
//			}
			inPacketSizes += 1;

			/* MCP Indicator & etc.*/
			inPacketSizes += 9;
//		}
//		else
//		{
//			/* SmartPay要回傳這三項資訊 (99 Bytes) */
//			if ((!memcmp(&srECROb->srTransData.szField_05[0], "S", 1) || pobTran->srBRec.uszFiscTransBit == VS_TRUE) &&
//			     pobTran->uszCardInquiryFirstBit != VS_TRUE)
//			{
//				/* SP ISSUER ID (8 Byte) */
//				memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szFiscIssuerID[0], 8);
//				inPacketSizes += 8;
//				/* SP Origin Date (8 Byte) */
//				if (strlen(pobTran->srBRec.szFiscRefundDate) > 0)
//				{
//					memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szFiscRefundDate[0], 8);
//				}  
//				else
//				{
//					memset(szTemplate, 0x00, sizeof(szTemplate));
//					strcat(szTemplate, pobTran->srBRec.szDate);
//					memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 8);
//				}
//				inPacketSizes += 8;
//				/* SP RRN (12 Byte) */
//				memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szFiscRRN[0], 12);
//				inPacketSizes += 12;
//			}
//			else
//			{
//				inPacketSizes += 8;
//				inPacketSizes += 8;
//				inPacketSizes += 12;
//			}
//
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			inGetPayItemEnable(szTemplate);
//			if (memcmp(szTemplate, "Y", strlen("Y")) == 0	&&
//			   (strlen(pobTran->srBRec.szPayItemCode) > 0))
//			{
//				/* Pay Item (5 Bytes) */
//				/* ECR一段式收銀機連線 送空白payitem 不必回傳 */
//				if (srECROb->srTransData.uszECRResponsePayitem == VS_FALSE)
//				{
//					inPacketSizes += 5;
//				}
//				else
//				{
//					memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szPayItemCode[0], 5);
//					inPacketSizes += 5;
//				}
//			}
//			else
//			{
//				inPacketSizes += 5;
//			}
//			/* 【需求單 - 105039】信用卡為電子發票載具，端末機將卡號加密後回傳收銀機 add by LingHsiung 2016-04-20 上午 09:56:24 */
//			/* 【需求單 - 108046】電子發票BIN大於6碼需求 by Russell 2019/7/8 上午 11:44 */
//			/* 收銀機欄位ECR Indicator = “E” : 
//			 * (1)銀行代碼為 ”B00999” : Card No. Hash Value = 空白50碼
//			 * (2)其他 :Card No. Hash Value = 銀行代碼6碼 + Hash Value 44碼。
//			*/
//			/* Card No. Hash Value (50 Bytes) */
//			/* Smartpay 和 DCC 因為不回傳"NI"，所以不回傳hash value */
//			/* 只要沒回傳Table "NI" 一律回50個空白 */
//			/* (需求單-109455)-Üny實體掃碼需求 by Russell 2021/9/3 下午 6:18 */
//			/* Üny 交易：
//			   電子發票加密卡號 (B00xxx ’’+44 碼 HASH 值 xxx為金融機構代碼) */
//			/* 【需求單 - 112071】客製化122 京站時尚百貨，調整收銀機規格
//			 * 符合下列全部條件時，ECR加回欄位37 Card No. Hash Value，內容為 : “B00000” + 44碼卡號Hash值。
//			 * ECR Indicator=‘E’、 有卡號、 電文無NI Table或NI Table為”B00999”(非未加入電子發票載具之銀行或國外卡)。
//			 * 2023/6/12 下午 4:03 by Russell */
//			if (!memcmp(szCustomerIndicator, _CUSTOMER_INDICATOR_122_QSQUARE_, _CUSTOMER_INDICATOR_SIZE_) &&
//			    !memcmp(&srECROb->srTransData.szECRIndicator[0], "E", 1) &&
//			     strlen(pobTran->srBRec.szPAN) > 0 &&
//			    (strlen(pobTran->srBRec.szEIVI_BANKID) == 0 || memcmp(pobTran->srBRec.szEIVI_BANKID, "B00999", 6) == 0))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], "B00000", 6);
//				inPacketSizes += 6;
//
//				memset(szTemplate, 0x00, sizeof(szTemplate));
//				if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2))
//				{
//					
//				}
//				else
//				{
//					memcpy(&szTemplate[0], &pobTran->srBRec.szPAN[0], strlen(pobTran->srBRec.szPAN));
//				}
//				
//				if (strlen(szTemplate) > 0)
//				{
//					memset(szHash, 0x00, sizeof(szHash));
//					inNCCC_Func_CardNumber_Hash(szTemplate, szHash);
//					memcpy(&szDataBuffer[inPacketSizes], &szHash[0], 44);
//				}
//				inPacketSizes += 44;
//			}
//			else
//			{
//				if (!memcmp(&srECROb->srTransData.szECRIndicator[0], "Q", 1))
//				{
//					if (pobTran->uszCardInquiryFirstBit == VS_TRUE	||
//					    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)	||
//					    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2) ||
//                                            pobTran->srBRec.uszUPlanECRBit == VS_TRUE)
//					{
//						inPacketSizes += 6;
//						inPacketSizes += 44;
//					}
//					else
//					{
//						memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szEIVI_BANKID[0], 6);
//						inPacketSizes += 6;
//						memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szEInvoiceHASH[0], 44);
//						inPacketSizes += 44;
//					}
//				}
//				else if (!memcmp(&srECROb->srTransData.szECRIndicator[0], "E", 1))
//				{
//					/* 啟動卡號查詢只有在Indicator = "I" 才送Hash Value("E"一律送50個空白) */
//					/* 因為卡號查詢會轉交易別，所以用Flag判斷 */
//					if (pobTran->uszCardInquiryFirstBit == VS_TRUE)
//					{
//						inPacketSizes += 50;
//					}
//					else
//					{
//						if (strlen(pobTran->srBRec.szEIVI_BANKID) > 0)
//						{
//							if (!memcmp(&pobTran->srBRec.szEIVI_BANKID[0], "B00999", 6))
//							{
//								inPacketSizes += 50;
//							}
//							else
//							{
//								memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szEIVI_BANKID[0], 6);
//								inPacketSizes += 6;
//
//								memset(szTemplate, 0x00, sizeof(szTemplate));
//								if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2))
//								{
//									
//								}
//								else
//								{
//									memcpy(&szTemplate[0], &pobTran->srBRec.szPAN[0], strlen(pobTran->srBRec.szPAN));
//								}
//								
//								if (strlen(szTemplate) > 0)
//								{
//									memset(szHash, 0x00, sizeof(szHash));
//									inNCCC_Func_CardNumber_Hash(szTemplate, szHash);
//									memcpy(&szDataBuffer[inPacketSizes], &szHash[0], 44);
//								}
//								inPacketSizes += 44;
//							}
//						}
//						else
//						{
//							inPacketSizes += 50;
//						}
//					}
//				}
//				/* 收銀機欄位ECR Indicator = “I” : Card No. Hash Value = Card number前6碼 + Hash Value 44碼(原規格) */
//				else
//				{
//					memset(szTemplate, 0x00, sizeof(szTemplate));
//					if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_AWARD_REDEEM_, 2))
//					{
//						
//					}
//					else
//					{
//						/* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//						memcpy(&szTemplate[0], &pobTran->srBRec.szPAN[0], strlen(pobTran->srBRec.szPAN));
//					}
//
//					if (strlen(szTemplate) > 0)
//					{
//						memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 6);
//						inPacketSizes += 6;
//
//						memset(szHash, 0x00, sizeof(szHash));
//						inNCCC_Func_CardNumber_Hash(szTemplate, szHash);
//						memcpy(&szDataBuffer[inPacketSizes], &szHash[0], 44);
//						inPacketSizes += 44;
//					}
//					else
//					{
//						inPacketSizes += 50;
//					}
//				}
//			}
//
//			/* MP Response Code (6 Bytes) */
//			memset(szFESMode, 0x00, sizeof(szFESMode));
//			inGetNCCCFESMode(szFESMode);
//			if (memcmp(szFESMode, _NCCC_04_MPAS_MODE_, strlen(_NCCC_04_MPAS_MODE_)) == 0)
//			{
//				if (memcmp(pobTran->srBRec.szRespCode, "00", 2) != 0		&& 
//				    memcmp(pobTran->srBRec.szRespCode, "11", 2) != 0		&&
//				    pobTran->srBRec.uszMPASTransBit == VS_TRUE			&&
//				    (memcmp(&pobTran->srBRec.szMPASAuthCode[0], "CAF", 3) == 0	||
//				     memcmp(&pobTran->srBRec.szMPASAuthCode[0], "NEG", 3) == 0	||
//				     memcmp(&pobTran->srBRec.szMPASAuthCode[0], "WEB", 3) == 0	|| 
//				     memcmp(&pobTran->srBRec.szMPASAuthCode[0], "REJ", 3) == 0))
//				{
//					memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szMPASAuthCode[0], 6);
//				}
//			}
//			inPacketSizes += 6;
//
//			/* ASM award flag */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_OFFLINE_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_INSTALLMENT_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REDEEM_, 2))
//			{	
//				if ((pobTran->srBRec.uszRewardL1Bit == VS_TRUE	||
//				     pobTran->srBRec.uszRewardL2Bit == VS_TRUE	||
//				     pobTran->srBRec.uszRewardL5Bit == VS_TRUE)	&&
//				     pobTran->srBRec.uszRewardSuspendBit != VS_TRUE)
//				{
//					memcpy(&szDataBuffer[inPacketSizes], "A", strlen("A"));
//				}
//				else
//				{
//					memcpy(&szDataBuffer[inPacketSizes], " ", strlen(" "));
//				}
//			}
//			else
//			{
//				memcpy(&szDataBuffer[inPacketSizes], " ", strlen(" "));
//			}
//			inPacketSizes += 1;
//
//			/* 【需求單 - 106128】行動支付標記及金融機構代碼 add by LingHsiung 2017-08-23 上午 11:48:57 */
//			if (strlen(pobTran->srBRec.szMCP_BANKID) > 0)
//			{
//				/* MCP Indicator (1 Bytes) */
//				memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szMCP_BANKID[0], 1);
//				inPacketSizes += 1;
//
//				/* 金融機構代碼 (3 Bytes) */
//				memcpy(&szDataBuffer[inPacketSizes], &pobTran->srBRec.szMCP_BANKID[1], 3);
//				inPacketSizes += 3;
//			}
//			else
//			{
//				inPacketSizes += 4;
//			}
//
//			/* Reserve (5 Bytes) */
//			inPacketSizes += 5;
//		}
//
//		/* HG Data (78 Byte) */
//		if (pobTran->srBRec.lnHGTransactionType != 0	&&
//		    (pobTran->srBRec.uszHappyGoMulti == VS_TRUE || pobTran->srBRec.uszHappyGoSingle == VS_TRUE))
//		{
//			/* Payment Tools (2 Byte) */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2))
//			{
//				if (pobTran->srBRec.lnHGPaymentType == _HG_PAY_CREDIT_)
//					memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_HG_PAYMENT_TOOL_TYPE_CREDIT_CARD_, 2);
//				else if (pobTran->srBRec.lnHGPaymentType == _HG_PAY_CASH_)
//					memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_HG_PAYMENT_TOOL_TYPE_CASH_, 2);
//				else if (pobTran->srBRec.lnHGPaymentType == _HG_PAY_GIFT_PAPER_)
//					memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_HG_PAYMENT_TOOL_TYPE_GIFT_PAPER_, 2);
//				else if (pobTran->srBRec.lnHGPaymentType == _HG_PAY_CREDIT_INSIDE_)
//					memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_HG_PAYMENT_TOOL_TYPE_HGI_, 2);
//				else if (pobTran->srBRec.lnHGPaymentType == _HG_PAY_CUP_)
//					memcpy(&szDataBuffer[inPacketSizes], _ECR_8N1_HG_PAYMENT_TOOL_TYPE_CUP_CARD_, 2);
//			}
//
//			inPacketSizes += 2;
//
//			/* HG Card Number (18 Byte) */
//			/* 重印HG混信用卡的交易簽單不回傳HG卡號 */
//			if (memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2) != 0)
//			{
//				inRetVal = inECR_CardNoTruncateDecision_HG(pobTran);
//				/* 要遮卡號 */
//				if (inRetVal == VS_SUCCESS)
//				{
//					memset(szTemplate, 0x00, sizeof(szTemplate));
//					strcpy(szTemplate, pobTran->srBRec.szHGPAN);
//
//					/* HAPPG_GO 卡不掩飾 */
//					if (!memcmp(&pobTran->srBRec.szHGPAN[0], "9552", 4))
//					{
//
//					}
//					else
//					{
//						/* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//						/* 卡號長度 */
//						inCardLen = strlen(szTemplate);
//
//						if (!memcmp(szCustomerIndicator, _CUSTOMER_INDICATOR_088_TINTIN_, _CUSTOMER_INDICATOR_SIZE_))
//						{
//							/* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//							for (i = 8; i < (inCardLen - 4); i ++)
//								szTemplate[i] = '*';
//						}
//						else
//						{    
//							/* (需求單 - 106349)移除原U CARD邏輯判斷 2018/10/31 下午 3:41 by Russell */
//							for (i = 6; i < (inCardLen - 4); i ++)
//								szTemplate[i] = '*';
//						}
//					}
//
//					memcpy(&szDataBuffer[inPacketSizes], szTemplate, strlen(szTemplate));
//				}
//				else
//				{
//					memcpy(&szDataBuffer[inPacketSizes], pobTran->srBRec.szHGPAN, strlen(pobTran->srBRec.szHGPAN));
//				}
//
//			}
//
//			inPacketSizes += 18;
//
//			/* HG Pay Amount (12 Byte) 實際支付金額 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2))
//			{
//				memset(szTemplate, 0x00, sizeof(szTemplate));
//				sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnHGAmount);
//				memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//			}
//
//			inPacketSizes += 12;
//			/* HG Redeem Amount (12 Byte) 扣抵金額 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2))
//			{
//				memset(szTemplate, 0x00, sizeof(szTemplate));
//				sprintf(szTemplate, "%010lu00", pobTran->srBRec.lnHGRedeemAmount);
//				memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 12);
//			}
//
//			inPacketSizes += 12;
//			/* HG Redeem Point (8 Byte) 扣抵點數 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2)		||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2)		||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2)		||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2))
//			{
//				memset(szTemplate, 0x00, sizeof(szTemplate));
//				sprintf(szTemplate, "%08lu", pobTran->srBRec.lnHGTransactionPoint);
//				memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 8);
//			}
//
//			inPacketSizes += 8;
//			/* HG Lack Point (8 Byte) 不足點數*/
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2))
//			{
//				memset(szTemplate, 0x00, sizeof(szTemplate));
//				sprintf(szTemplate, "%08lu", pobTran->srBRec.lnHGRefundLackPoint);
//				memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 8);
//			}
//
//			inPacketSizes += 8;
//			/* HG Balance Point (8 Byte) 剩餘點數 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_ONLINE_REDEEMPTION_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_CERTAIN_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_FULL_REDEEMPTION_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REDEEM_REFUND_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_REWARD_REFUND_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_HG_POINT_INQUIRY_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_VOID_, 2))
//			{
//				memset(szTemplate, 0x00, sizeof(szTemplate));
//				sprintf(szTemplate, "%08lu", pobTran->srBRec.lnHGBalancePoint);
//				memcpy(&szDataBuffer[inPacketSizes], &szTemplate[0], 8);
//			}
//
//			inPacketSizes += 8;
//			/* HG Reserve (10 Byte) */
//			inPacketSizes += 10;
//		}
//		else
			inPacketSizes += 78;
//	}
        
//	if (gbBarCodeECRBit == VS_TRUE)
//	{
//		/* Üny 交易碼 */
//		if (pobTran->srBRec.uszUnyTransBit == VS_TRUE)
//		{
//			/* 一維或二維條碼資料長度 */
//			inBarCodeLen = strlen(pobTran->srBRec.szBarCodeData);
//			inPacketSizes += 3;
//			/* 一維或二維條碼資料內容 */
//			inPacketSizes += inBarCodeLen;
//
//			/* 補滿至980 */
//			inPacketSizes = 980;
//
//			/* Üny 交易碼 */
//			/* 除結束卡號查詢不用，其他Uny交易都要 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_END_CARD_NO_INQUIRY_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REPRINT_RECEIPT_, 2))
//			{
//
//			}
//			else
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srBRec.szUnyTransCode, 20);
//			}
//			inPacketSizes += 20;
//		}
//		else if (pobTran->srEWRec.uszEWTransBit == VS_TRUE)
//		{
//			/* 電子錢包資料長度 */
//			inBarCodeLen = strlen(pobTran->srBRec.szBarCodeData);
//			sprintf(&szDataBuffer[inPacketSizes], "%03d", 184);
//			inPacketSizes += 3;
//			/* 電子錢包交易條碼 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srBRec.szBarCodeData, inBarCodeLen);
//			}
//			inPacketSizes += 25;
//			
//			/* 電子錢包業者特店代號 */
//			memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_ChannelId, 50);
//			inPacketSizes += 50;
//			/* 電子錢包交易日期時間 */
//			memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_PaymentDate, 24);
//			inPacketSizes += 24;
//			/* 消費者支付工具 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2)	||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_PayMethod, 1);
//			}
//			inPacketSizes += 1;
//			/* 訂單狀態 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_TransactionType, 2);
//			}
//			inPacketSizes += 2;
//			/* 付款交易編號 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_PaymentNo, 30);
//			}
//			inPacketSizes += 30;
//			/* 是否為重複付款請求 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_IsDuplicatedPaymentRequest, 1);
//			}
//			inPacketSizes += 1;
//			/* 聯名卡代碼 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_CobrandedCode, 11);
//			}
//			inPacketSizes += 11;
//			/* 載具類型 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_EinvoiceCarrierType, 6);
//			}
//			inPacketSizes += 6;
//			/* 折抵金額/退還折抵金額 */
//			if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_SALE_, 2) ||
//			    !memcmp(srECROb->srTransData.szTransType, _ECR_8N1_EW_INQUIRY_TRANSACTION_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_DiscountAmt, 10);
//			}
//			else if (!memcmp(srECROb->srTransData.szTransType, _ECR_8N1_REFUND_, 2))
//			{
//				memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_DiscountAmt, 10);
//			}
//			inPacketSizes += 10;
//			/* 門市代碼 */
//			memcpy(&szDataBuffer[inPacketSizes], pobTran->srEWRec.szEW_Store_no, 24);
//			inPacketSizes += 24;
//			/* 保留 */
//			inPacketSizes += 413;
//		}
//		else if (pobTran->srBRec.uszUPlanECRBit == VS_TRUE)
//		{
//			/* 一維或二維條碼資料長度 */
//			inBarCodeLen = strlen(pobTran->srBRec.szBarCodeData);
//			inPacketSizes += 3;
//			/* 一維或二維條碼資料內容 */
//			inPacketSizes += inBarCodeLen;
//			
//			/* 補滿至980 */
//			inPacketSizes = 980;
//			
//			inPacketSizes += 20;
//		}
//	}
        printf("inPacketSizes : %d\n",inPacketSizes);
	return (VS_SUCCESS);
}