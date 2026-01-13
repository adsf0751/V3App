#include "../../EDC.h" // For  EDC_REC
#define _COMM_RECEIVE_MAX_LENGTH_	1536
// Define Ethernet Data //
#define IPADDR      "10.105.109.155"
#define MASK        "255.255.254.0"
#define GATEWAY     "10.105.109.254"
#define HOSTIP      "10.105.108.22"
#define HOSTPORT    "18012"
typedef struct
{
        char szCommIndex[2 + 1];                /* 通訊參數索引 */
        char szTPDU[10 + 1];                    /* 60nnnnnnnn */
        char szNII[3 + 1];                      /* 網路識別碼 */
        char szHostTelPrimary[15 + 1];          /* 第一授權撥接電話 */
        char szHostTelSecond[15 + 1];           /* 第二授權撥接電話 */
        char szReferralTel[15 + 1];             /* Call Bank 撥接電話 */
        char szHostIPPrimary[15 + 1];           /* 第一授權主機 IP Address  */
        char szHostPortNoPrimary[5 + 1];        /* 第一授權主機 Port No. */
        char szHostIPSecond[15 + 1];            /* 第二授權主機 IP Address */
        char szHostPortNoSecond[5 + 1];         /* 第二授權主機 Port No. */
        char szTCPHeadFormat[1 + 1];            /* TCP 電文長度之格式。( B=BCD，H=Binary)，預設值= H。 */
        char szCarrierTimeOut[2 + 1];           /* 連線等候時間。(超過等候時間，自動撥第二授權電話或IP Address) */
        char szHostResponseTimeOut[2 + 1];      /* 授權等候時間。(超過等候時間，自動斷線) */
}CPT_REC;

CPT_REC srCPTRec;	/* construct CPT record */
EDC_REC srEDCRec;
int ginTrans_ClientFd;
int inGetHostIPPrimary(char* );
int inGetHostPortNoPrimary(char* );
int inETHERNET_Open(void);
int inETHERNET_END(void);
int inETHERNET_Cofig_Get(unsigned char , unsigned char *, unsigned char *);
int inETHERNET_Cofig_Set(unsigned char , unsigned char *, unsigned char );
int inETHERNET_DisConnect_By_Native(void);
int inETHERNET_DisConnect_Flow(void);
int inETHERNET_Connect_By_Native(char *, char *);
int inETHERNET_Connect_Flow(char *, char *);
int inETHERNET_SetConfig(void);
int inETHERNET_Initial(void);
int inETHERNET_Get_Status(unsigned int *);
int inETHERNET_Watch_Status(void);
void EthernetPing(const char*);
int inETHERNET_Send(unsigned char *, int , int );
int inETHERNET_Send_Data_Flow(unsigned char* , unsigned short );
int inETHERNET_Send_Ready_Flow(void);
int inETHERNET_Send_Ready_By_Native(int );
int inETHERNET_Send_Data_By_Native(int , unsigned char* , unsigned short );
int inETHERNET_Receive(unsigned char *, int , int );
int inETHERNET_Receive_Ready_Flow(unsigned short *);
int inETHERNET_Receive_Ready_By_Native(int , unsigned short *);
int inETHERNET_Receive_Data_Flow(unsigned char* , unsigned short *);