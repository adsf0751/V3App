#include "RS232.h"

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
	printf("InData: \n");
        int i;
        for(i= 0;i<_ECR_8N1_Standard_Data_Size_;i++)
        {
            printf("%C",szDataBuffer[i]);
        }
        printf("\n");
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