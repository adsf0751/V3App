#ifndef RS232_H
#define RS232_H

#ifdef __cplusplus
extern "C" {
#endif


#include "ECR_Struct.h"
#include "../../Trans.h"
#include <string.h>
#include <ctoserr.h>
#include <ctosapi.h>
#include <stdio.h>
#include "ECR.h"
#define _STX_					0x02
#define _ETX_					0x03
#define _ACK_					0x06
#define _NAK_					0x15
#define _ECR_RS232_RETRYTIMES_			_ECR_RETRYTIMES_
#define _ECR_RS232_RECEIVE_TIMER_		_ECR_RECEIVE_TIMER_
#define _ECR_RS232_RECEIVE_REQUEST_TIMEOUT_	_ECR_RECEIVE_REQUEST_TIMEOUT_
#define _ECR_RS232_GET_CARD_TIMEOUT_		_ECR_GET_CARD_TIMEOUT_
#define _ECR_RS232_RECEIVE_ACK_TIMEOUT_		_ECR_RECEIVE_ACK_TIMEOUT_LONG_
#define _ECR_RS232_BUFF_SIZE_			_ECR_BUFF_SIZE_

typedef struct
{
    char  FieldName[100 +1];
    int   Length;
    int   Idx; 
    char* Data;
}MY_ECR_DATA;
extern MY_ECR_DATA myECRTable[43];
int inRS232_ECR_8N1_Standard_Initial(ECR_TABLE *srECROb);
int inRS232_ECR_8N1_Standard_Receive_Packet(TRANSACTION_OBJECT *pobTran, ECR_TABLE * srECROb);
int inRS232_Close(unsigned char uszComport);
int inRS232_Data_Receive(unsigned char uszComPort, unsigned char *uszReceBuff, unsigned short *usReceSize);
int inRS232_Data_Receive_Check(unsigned char uszComPort, unsigned short *usReceiveLen);
int inRS232_Data_Send(unsigned char uszComPort, unsigned char *uszSendBuff, unsigned short usSendSize);
int inRS232_Data_Send_Check(unsigned char uszComPort);
int inRS232_FlushTxBuffer(unsigned char uszComPort);
int inRS232_FlushRxBuffer(unsigned char uszComPort);
int inRS232_ECR_8N1_Standard_Send_Packet(TRANSACTION_OBJECT *pobTran, ECR_TABLE * srECROb);

#ifdef __cplusplus
}
#endif

#endif 