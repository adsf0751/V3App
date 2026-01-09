#include <ctosapi.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../Trans.h"
#include "../DISPLAY/Display.h"
#include "Ethernet.h"
extern int ginTrans_ClientFd;
extern CPT_REC srCPTRec;
extern EDC_REC srEDCRec;

//Define Ethernet Data //
char ipAddr[] = "10.105.109.155"; //IP Address
char mask[] = "255.255.254.0"; //Mask
char gateWay[] = "10.105.109.254"; //Gateway IP
char hostIp[] = "10.105.108.43"; //Server IP
char hostPort[] = "18012"; //Server Port
/*
Function        :inGetHostIPPrimary
Date&Time       :
Describe        :
*/
int inGetHostIPPrimary(char* szHostIPPrimary)
{
        /* 傳進的指標 不得為空  長度需大於0 小於規定最大值 */
        if (szHostIPPrimary == NULL || strlen(srCPTRec.szHostIPPrimary) <= 0 || strlen(srCPTRec.szHostIPPrimary) > 15)
        {
                return (VS_ERROR);
        }
        memcpy(&szHostIPPrimary[0], &srCPTRec.szHostIPPrimary[0], strlen(srCPTRec.szHostIPPrimary));

        return (VS_SUCCESS);
}
int inGetHostPortNoPrimary(char* szHostPortNoPrimary)
{
        /* 傳進的指標 不得為空  長度需大於0 小於規定最大值 */
        if (szHostPortNoPrimary == NULL || strlen(srCPTRec.szHostPortNoPrimary) <= 0 || strlen(srCPTRec.szHostPortNoPrimary) > 5)
        {
                return (VS_ERROR);
        }
        memcpy(&szHostPortNoPrimary[0], &srCPTRec.szHostPortNoPrimary[0], strlen(srCPTRec.szHostPortNoPrimary));

        return (VS_SUCCESS);
}
int inETHERNET_Open() {
    char szDebugMsg[100 + 1];
    unsigned short usRetVal;

    usRetVal = CTOS_EthernetOpen();
    if (usRetVal == d_OK) {
        //printf("inETHERNET_Open successed\n");
    } else if (usRetVal == d_ETHERNET_ALREADY_OPENED) {
        //printf("usRetVal == d_ETHERNET_ALREADY_OPENED\n");
    } else {
        printf("inETHERNET_Open failed\n");
        return (VS_ERROR);
    }

    return (VS_SUCCESS);
}
/*
Function        :inETHERNET_Cofig_Get
Date&Time       :2018/5/15 下午 5:26
Describe        :
 */
int inETHERNET_Cofig_Get(unsigned char uszTag, unsigned char *uszValue, unsigned char *uszLen) {
    char szDebugMsg[100 + 1] = {0};
    unsigned short usRetVal;

    usRetVal = CTOS_EthernetConfigGet(uszTag, uszValue, uszLen);
    if (usRetVal == d_OK) {
        printf("CTOS_EthernetConfigGet successed\n");
    } else {
        printf("CTOS_EthernetConfigGet failed, retVal is %X\n", usRetVal);
        return (VS_ERROR);
    }

    return (VS_SUCCESS);
}
int inETHERNET_Cofig_Set(unsigned char uszTag, unsigned char *uszValue, unsigned char uszLen) {
    char szDebugMsg[100 + 1];
    unsigned short usRetVal = 0x00;

    usRetVal = CTOS_EthernetConfigSet(uszTag, uszValue, uszLen);
    if (usRetVal == d_OK) {
        printf("CTOS_EthernetConfigSet successed\n");
    } else {
        printf("CTOS_EthernetConfigSet failed,%s,usRetVal:%X\n",uszValue,usRetVal);
        return (VS_ERROR);
    }

    return (VS_SUCCESS);
}

/*
Function        :inETHERNET_END
Date&Time       :2017/7/19 上午 11:09
Describe        :斷線
*/
int inETHERNET_END(void)
{
        /* inETHERNET_END() START! */
	if (inETHERNET_DisConnect_Flow() != VS_SUCCESS)
	{
		return (VS_ERROR);
	}

        /* inETHERNET_END() END! */
        return (VS_SUCCESS);
}
/*
Function        :inETHERNET_DisConnect_By_Native
Date&Time       :2017/8/2 下午 5:37
Describe        :
*/
int inETHERNET_DisConnect_By_Native()
{
	int		inRetVal = VS_ERROR;
	char		szDebugMsg[100 + 1];
		
//	/* 防呆，小於0會存取到錯的記憶體 */
	if (ginTrans_ClientFd > 0)
	{
		inRetVal = close(ginTrans_ClientFd);
	}
	else
	{
		inRetVal = VS_ERROR;
	}
	
	if (inRetVal == 0)
	{
		inRetVal= VS_SUCCESS;
//		inFile_Open_File_Cnt_Decrease();
	}
	else
	{
		return (VS_ERROR);
	}
	return (VS_SUCCESS);
}

/*
Function        :inETHERNET_DisConnect_Flow
Date&Time       :2017/8/2 下午 5:35
Describe        :分流
*/
int inETHERNET_DisConnect_Flow()
{
	int	inRetVal = VS_ERROR;
//	vdUtility_SYSFIN_LogMessage(AT, "Ethernet Disconect:(way:%d)", ginEthernetFlow);
//	
//	if (ginEthernetFlow == _ETHERNET_FLOW_IFES_)
//	{
//		inRetVal = inETHERNET_DisConnect_TLS_Flow();
//		if (inRetVal == VS_SUCCESS)
//		{
//			ginEthernetFlow = _ETHERNET_FLOW_CTOS_;
//		}
//	}
	/* 用native方式建Socket，之後要優化再考慮使用(優點自己決定Timeout等等細微設定) */
//	else if (ginEthernetFlow == _ETHERNET_FLOW_NATIVE_)
//	{   
                //to fix:基本上這個function沒用處，因為未使用到inFile_Open_File_Cnt_Decrease()等
		inRetVal = inETHERNET_DisConnect_By_Native();
//		if (inRetVal == VS_SUCCESS)
//		{
//			ginEthernetFlow = _ETHERNET_FLOW_CTOS_;
//		}
//	}
	/* 虹堡API */
//	else
//	{
//		inRetVal = inETHERNET_DisConnect();
//		if (inRetVal == VS_SUCCESS)
//		{
//			ginEthernetFlow = _ETHERNET_FLOW_CTOS_;
//		}
//	}
	
	if (inRetVal == VS_SUCCESS)
	{
//		vdUtility_SYSFIN_LogMessage(AT, "Ethernet Disconect Success");
		return (VS_SUCCESS);
	}
	else
	{
//		vdUtility_SYSFIN_LogMessage(AT, "Ethernet Disconect Fail");
		return (VS_ERROR);
	}
}
/*
Function        :inETHERNET_Connect_By_Native
Date&Time       :2017/7/31 下午 4:31
Describe        :原生C Socket連線
*/
int inETHERNET_Connect_By_Native(char *szHostIP, char *szPort)
{
	int			inRetVal = 0;
	int			inOption = 0;
	int			inFlags = 0;
	char			szDebugMsg[100 + 1];
	struct sockaddr_in	stAddr;			/* 放Host address的結構 */
	

	/* 初始化address */
	memset(&stAddr, 0x00, sizeof (stAddr)); //clear all zero.
	stAddr.sin_family = AF_INET;
	stAddr.sin_addr.s_addr = inet_addr(szHostIP);
	stAddr.sin_port = htons(atoi(szPort));

	ginTrans_ClientFd = socket(AF_INET, SOCK_STREAM, 0);
	if (ginTrans_ClientFd == -1)
	{
            printf("socket return fd is %d\n",ginTrans_ClientFd);
            return (VS_ERROR);
	}
//	else
//	{
//		inFile_Open_File_Cnt_Increase();
//	}
	
	/* 系统預設的狀態發送與接收一次為8688 Bytes(約為8.5K) 應該夠用，所以不必特別調整buffer大小 */
	
	
	/* 開關 ON */
	inOption = 1;
	/* 允許重用本地位址和埠 */
	setsockopt(ginTrans_ClientFd, SOL_SOCKET, SO_REUSEADDR, &inOption, sizeof(inOption));
	/* 設定MSS(maximum segment size) 預設為536，和520同步解決宏遠電信問題，含header設為1400，扣掉header(54)應該設為1346 */
	inOption = 1346;
	setsockopt(ginTrans_ClientFd, SOL_TCP, TCP_MAXSEG, &inOption, sizeof(inOption));

	/* Linux内核中对connect的超时时间限制是75s， Soliris 9是几分钟，因此通常认为是75s到几分钟不等*/
	/* 師爺給我翻譯翻譯：不用非阻塞Timeut就是75秒，所以一定要用非阻塞 */
	/* 先獲得現在Handle的開關狀態 */
	inFlags = fcntl(ginTrans_ClientFd, F_GETFL, 0);
	/* 把非阻塞開關On起來(做or運算)，並設定回去 */
	fcntl(ginTrans_ClientFd, F_SETFL, inFlags | O_NONBLOCK);
	
	/* 連線Timout 2秒 */
	inRetVal = inDISP_Timer_Start(_TIMER_NEXSYS_1_, 2);
        if(inRetVal==VS_SUCCESS)
            printf("inDISP_Timer_Start successed\n");
        else
            printf("inDISP_Timer_Start failed\n");
	do
	{
            
		/* Timeout */
		if (inTimerGet(_TIMER_NEXSYS_1_) == VS_SUCCESS)
		{
                    printf("inTimerGet timeout\n");
			return (VS_TIMEOUT);
		}
		
		inRetVal = connect(ginTrans_ClientFd, (void*)&stAddr, sizeof(stAddr));
		/* 通常只有同一台機器內Clinet連server，才有可能馬上成功 */
		if (inRetVal == 0)
		{
                    //inETHERNET_Watch_Status();
                    printf("socket connect succcessed!!,ClientFd is %d\n",ginTrans_ClientFd);
                    
		}
		else
		{
                    printf("connect failed\n");
		}
		
	} while (inRetVal != 0);

	return (VS_SUCCESS);
}

/*
Function        :inETHERNET_Connect_Flow
Date&Time       :2017/7/18 下午 4:53
Describe        :分流使用CTOS還是使用原生C的Connect
*/
int inETHERNET_Connect_Flow(char *szHostIP, char *szPort)
{
	int	inRetVal = VS_ERROR;
	char	szI_FES_Mode[2 + 1] = {0};
	char	szTRTFileName[12 + 1] = {0};
	char	szDebugMsg[500 + 1] = {0};
	char	szCFESMode[2 + 1] = {0};


	/* 一律先重置Handle */
//	ginTrans_ClientFd = -1;
	
	/* 用原生C Socket */
//	memset(szI_FES_Mode, 0x00, sizeof(szI_FES_Mode));
//	inGetI_FES_Mode(szI_FES_Mode);
//	memset(szCFESMode, 0x00, sizeof(szCFESMode));
//	inGetCloud_MFES(szCFESMode);
//	memset(szTRTFileName, 0x00, sizeof(szTRTFileName));
//	inGetTRTFileName(szTRTFileName);
//	

	
	/* 判斷是IFES及非大來，其他全跑IFES */
	/* 2018/5/8 下午 2:10 DFS需求不再使用大來主機 */
	/* CFES也要跑TLS */
//	if (memcmp(szI_FES_Mode, "Y", strlen("Y")) == 0	||
//	    memcmp(szCFESMode, "Y", strlen("Y")) == 0)
//	{
//		if (ginDebug == VS_TRUE)
//		{
//			memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//			sprintf(szDebugMsg, "Connect Flow: TLS");
//			inLogPrintf(AT, szDebugMsg);
//		}
//		
//		inRetVal = inETHERNET_Connect_TLS_Flow(szHostIP, szPort);
//		/* 標示現在連線方式，斷線後重新恢復成_ETHERNET_FLOW_CTOS_ */
//		ginEthernetFlow = _ETHERNET_FLOW_IFES_;
//	}
//	/* 用native方式建Socket，之後要優化再考慮使用(優點自己決定Timeout等等細微設定) */
//	else if (1)
//	{
//		if (ginDebug == VS_TRUE)
//		{
//			memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//			sprintf(szDebugMsg, "Connect Flow: Native");
//			inLogPrintf(AT, szDebugMsg);
//		}
		
		inRetVal = inETHERNET_Connect_By_Native(szHostIP, szPort);
                if(inRetVal ==VS_SUCCESS )
                    printf("inETHERNET_Connect_By_Native successed\n");
                else
                    printf("inETHERNET_Connect_By_Native failed\n");
		/* 標示現在連線方式，斷線後重新恢復成_ETHERNET_FLOW_CTOS_ */
//		ginEthernetFlow = _ETHERNET_FLOW_NATIVE_;
//
//	}
	/* 虹堡API */
//	else
//	{
//		if (ginDebug == VS_TRUE)
//		{
//			memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//			sprintf(szDebugMsg, "Connect Flow: CTOS");
//			inLogPrintf(AT, szDebugMsg);
//		}
//		
//		inRetVal = inETHERNET_Connect();
//		/* 標示現在連線方式，斷線後重新恢復成_ETHERNET_FLOW_CTOS_ */
//		ginEthernetFlow = _ETHERNET_FLOW_CTOS_;
//	}
		
//	unsigned int	uiStatus = 0;
//	/* Get the status of the Ethernet */
//	inETHERNET_Get_Status(&uiStatus);
//	memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//	if (uiStatus & d_STATUS_ETHERNET_CONNECTED)
//	{
//		strcat(szDebugMsg, "Connected ");
//	}
//	else
//	{
//		 strcat(szDebugMsg, "NotConnected ");
//	}
//	
//	if (uiStatus & d_STATUS_ETHERNET_PHYICAL_ONLINE)
//	{
//		strcat(szDebugMsg, "Phyical_Online ");
//	}
//	else
//	{
//		 strcat(szDebugMsg, "NotPhyical_Online ");
//	}
//		
//	if (uiStatus & d_STATUS_ETHERNET_RX_READY)
//	{
//		strcat(szDebugMsg, "RxReady ");
//	}
//	else
//	{
//		 strcat(szDebugMsg, "NotRxReady ");
//	}
//	
//	if (uiStatus & d_STATUS_ETHERNET_TX_BUSY)
//	{
//		strcat(szDebugMsg, "TxBusy ");
//	}
//	else
//	{
//		 strcat(szDebugMsg, "NotTxBusy ");
//	}
//        printf("status %s\n",szDebugMsg);
	return (inRetVal);
}


int inETHERNET_SetConfig(void)
{
	int		inRetVal = 0;
	int		inConnectMaxCnt = 1;		/* 連線最大重試次數(含第一次) */
	int		inConnectNowCnt = 0;		/* 連線目前重試次數 */
	char		szHostIPPrimary[16 + 1] = {0};
	char		szHostIPSecond[16 + 1] = {0};
	char		szHostPortNoPrimary[6 + 1] = {0};
	char		szHostPortNoSecond[6 + 1] = {0};
	char		szConfig[16 + 1] = {0};
	char		szDebugMsg[100 + 1] = {0};
	char		szDHCPMode[2 + 1] = {0};
	char		szAutoConnect[10 + 1] = {0};
	char		szTemplate[50 + 1] = {0};
	char		szIFESMode[2 + 1] = {0};
	unsigned char	uszLen = 0;

	/* Set Host IP */
        memset(szHostIPPrimary,0x00,sizeof(szHostIPPrimary));
        if (inGetHostIPPrimary(szHostIPPrimary) == VS_ERROR)
        {
                /* inGetHostIPPrimary ERROR */
                printf("inGetHostIPPrimary failed\n");
                return (VS_ERROR);
        }
        
	inRetVal = inETHERNET_Cofig_Set(d_ETHERNET_CONFIG_HOSTIP, (unsigned char*)szHostIPPrimary, strlen(szHostIPPrimary));
	if (inRetVal != VS_SUCCESS)
	{	
            printf("inETHERNET_Cofig_Set d_ETHERNET_CONFIG_HOSTIP failed\n");
                return (VS_ERROR);
	}
        
	/* Set Host Port */
        memset(szHostPortNoPrimary,0x00,sizeof(szHostPortNoPrimary));
        if (inGetHostPortNoPrimary(szHostPortNoPrimary) == VS_ERROR)
        {
                /* Get HostPortNumber Primary ERROR */
                printf("inGetHostPortNoPrimary failed\n");
                return (VS_ERROR);
        }
        
	inRetVal = inETHERNET_Cofig_Set(d_ETHERNET_CONFIG_HOSTPORT, (unsigned char*)szHostPortNoPrimary, strlen(szHostPortNoPrimary));
	if (inRetVal != VS_SUCCESS)
	{        
                printf("inETHERNET_Cofig_Set d_ETHERNET_CONFIG_HOSTPORT failed\n");
                return (VS_ERROR);
	}
        
        /*
                d_ETHERNET_CONFIG_AUTOCON
                Set Connection Mode.
                = 0 : Auto-connect. When Auto-connect is set, the Ethernet module will automatically try to connect to the host every 5 seconds.
                = 1 : Not support
                = 2 : Manual. The connection must be established manually by calling CTOS_EthernetConnectxxx() function.
        */
        /* 預設成2 */
	memset(szConfig, 0x00, sizeof(szConfig));
//	memset(szAutoConnect, 0x00, sizeof(szAutoConnect));
//	inGetAutoConnect(szAutoConnect);
//	if (memcmp(&szAutoConnect[0], "1", strlen("1")) == 0)
//	{
//		szConfig[0] = '0';
//	}
//	else
//	{
		szConfig[0] = '2';
//	}
	if (inETHERNET_Cofig_Set(d_ETHERNET_CONFIG_AUTOCON, (unsigned char*)szConfig, 1) != VS_SUCCESS)
	{
            printf("inETHERNET_Cofig_Set d_ETHERNET_CONFIG_AUTOCON failed\n");
                return (VS_ERROR);
	}

        /*
                d_ETHERNET_CONFIG_DHCP
                Set IP configuration.
                = 0 : Static. Use the static IP set in the Ethernet configuration.
                = 1 : DHCP. Retrieve the dynamic IP from the DHCP server.
        */
        /* DHCP預設0 */
//	memset(szConfig, 0x00, sizeof(szConfig));
//	memset(szDHCPMode, 0x00, sizeof(szDHCPMode));
//	inGetDHCP_Mode(szDHCPMode);
//	if (memcmp(szDHCPMode, "Y", strlen("Y")) == 0)
//	{
//		szConfig[0] = 0x31;
//	}
//	else
//	{
		szConfig[0] = 0x30;
//	}
        
	if (inETHERNET_Cofig_Set(d_ETHERNET_CONFIG_DHCP, (unsigned char*)szConfig, 1) != VS_SUCCESS)
	{
            printf("inETHERNET_Cofig_Set d_ETHERNET_CONFIG_DHCP failed\n");
                return (VS_ERROR);
	}

        /*
                d_ETHERNET_CONFIG_UPDATE_EXIT
                Save the settings to the non-volatile memory in Ethernet module.      
        */
	memset(szConfig, 0x00, sizeof(szConfig));
	if (inETHERNET_Cofig_Set(d_ETHERNET_CONFIG_UPDATE_EXIT, (unsigned char*)szConfig, 0) != VS_SUCCESS)
	{
            printf("inETHERNET_Cofig_Set d_ETHERNET_CONFIG_UPDATE_EXIT failed\n");
                return (VS_ERROR);
	}

	/* 重置目前重試次數 */
	inConnectNowCnt = 0;
	/* 若連線失敗看是否要重試 */
	do 
	{
		inRetVal = inETHERNET_Connect_Flow(szHostIPPrimary, szHostPortNoPrimary);
		inConnectNowCnt ++;
		
	}while (inRetVal != VS_SUCCESS && inConnectNowCnt < inConnectMaxCnt);
        
	
	/* 第一授權主機連線成功 */
        if (inRetVal == VS_SUCCESS)
        {
            printf(" 第一授權主機連線成功\n");
		/* 如果是DHCP，*/
//		if (memcmp(szDHCPMode, "Y", strlen("Y")) == 0)
//		{
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			uszLen = sizeof(szTemplate);
//			inETHERNET_Cofig_Get(d_ETHERNET_CONFIG_IP, (unsigned char*)szTemplate, &uszLen);
//			inSetTermIPAddress(szTemplate);
//			
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			uszLen = sizeof(szTemplate);
//			inETHERNET_Cofig_Get(d_ETHERNET_CONFIG_MASK, (unsigned char*)szTemplate, &uszLen);
//			inSetTermMASKAddress(szTemplate);
//			
//			memset(szTemplate, 0x00, sizeof(szTemplate));
//			uszLen = sizeof(szTemplate);
//			inETHERNET_Cofig_Get(d_ETHERNET_CONFIG_GATEWAY, (unsigned char*)szTemplate, &uszLen);
//			inSetTermGetewayAddress(szTemplate);
//			
//			inSaveEDCRec(0);
//		}
                /* 看狀態 */
//                inETHERNET_Watch_Status();
                return (VS_SUCCESS);
        }
        else
	{
		inETHERNET_END();
	}

/*目前只有一個host ip and port 以下不實作*/        
        /* 看狀態 */
	//inETHERNET_Watch_Status();
////	inDISP_Clear_Line(_LINE_8_6_, _LINE_8_6_);
////	inDISP_ChineseFont("第二組IP", _FONTSIZE_8X16_, _LINE_8_6_, _DISP_LEFT_);
//	/* 嘗試連線第二授權主機 */
//	/* 更改HostIP */
//	/* Set Host IP */
//        memset(szHostIPSecond,0x00,sizeof(szHostIPSecond));
//        if (inGetHostIPSecond(szHostIPSecond) == VS_ERROR)
//        {
//                /* inGetHostIPSecond ERROR */
//                /* debug */
//                if (ginDebug == VS_TRUE)
//                        inLogPrintf(AT, "inGetHostIPSecond() ERROR!!");
//
//                return (VS_ERROR);
//        }
//        
//	inRetVal = inETHERNET_Cofig_Set(d_ETHERNET_CONFIG_HOSTIP, (unsigned char*)szHostIPSecond, strlen(szHostIPSecond));
//	if (inRetVal != VS_SUCCESS)
//	{
//		if (ginDebug == VS_TRUE)
//                {
//			memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//                        sprintf(szDebugMsg, "d_ETHERNET_CONFIG_HOSTIP Error : 0x%04x", inRetVal);
//                        inLogPrintf(AT, szDebugMsg);
//                }
//	
//                return (VS_ERROR);
//	}
//	
//	/* Set Host Port */
//        memset(szHostPortNoSecond, 0x00, sizeof(szHostPortNoSecond));
//        if (inGetHostPortNoSecond(szHostPortNoSecond) == VS_ERROR)
//        {
//                /* Get HostPortNumber Primary ERROR */
//                /* debug */
//                if (ginDebug == VS_TRUE)
//                        inLogPrintf(AT, "inGetHostPortNoSecond() ERROR!!");
//                
//                return (VS_ERROR);
//        }
//        
//	if (inETHERNET_Cofig_Set(d_ETHERNET_CONFIG_HOSTPORT, (unsigned char*)szHostPortNoSecond, strlen(szHostPortNoSecond)) != VS_SUCCESS)
//	{
//                if (ginDebug == VS_TRUE)
//                        inLogPrintf(AT, "d_ETHERNET_CONFIG_HOSTPORT Error");
//	
//                return (VS_ERROR);
//	}
//	
//	/* 紀錄在記憶體內 */
//	memset(szConfig, 0x00, sizeof(szConfig));
//	if (inETHERNET_Cofig_Set(d_ETHERNET_CONFIG_UPDATE_EXIT, (unsigned char*)szConfig, 0) != VS_SUCCESS)
//	{
//                if (ginDebug == VS_TRUE)
//                        inLogPrintf(AT, "d_ETHERNET_CONFIG_UPDATE_EXIT Error");
//	
//                return (VS_ERROR);
//	}
//	
//	/* 重置目前重試次數 */
//	inConnectNowCnt = 0;
//	/* 若連線失敗看是否要重試 */
//	do 
//	{
//		inRetVal = inETHERNET_Connect_Flow(szHostIPSecond, szHostPortNoSecond);
//		inConnectNowCnt ++;
//	}while (inRetVal != VS_SUCCESS && inConnectNowCnt < inConnectMaxCnt);
//
//	/* 第二授權主機連線成功 */
//        if (inRetVal == VS_SUCCESS)
//        {
//                return (VS_SUCCESS);
//        }
//        else
//	{
//               /* 若第二授權主機連線不成功，直接斷線 */ 
//	}
//	
//	/* 第二授權IP失敗 */
//	if (ginDebug == VS_TRUE)
//	{
//		memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//		sprintf(szDebugMsg, "Second IP Conenct Fail");
//		inLogPrintf(AT, szDebugMsg);
//	}
//	
//	/* 到最後還失敗，斷線 */
//	inETHERNET_END();
//	
//        if (ginDebug == VS_TRUE)
//                inLogPrintf(AT, "inETHERNET_SetConfig() END!");
//        
//	/* 若連線成功，中間就會Return出去，若一直失敗，則最後一定回傳失敗 */
//        return (VS_ERROR);
}
/*
Function        :inETHERNET_Send_Ready_By_Native
Date&Time       :2017/8/1 下午 1:40
Describe        :
*/
int inETHERNET_Send_Ready_By_Native(int inFileHandle)
{
	fd_set		stWfd;			/* 用來判斷是否可以寫的Fd */
	struct timeval	stTimeVal;
	
	stTimeVal.tv_sec = 0;
	stTimeVal.tv_usec = 0;
	
	/* 將 set 整個清為零。 */
	FD_ZERO(&stWfd);
	/* 將 fd 新增到 set。 */
	FD_SET(inFileHandle, &stWfd);
	
	
	/* nfds is the highest-numbered file descriptor in any of the three sets, plus 1. FD最高位再加一 */
	/* 若Timeout仍未有Fd，則回傳0 */
	select(inFileHandle + 1, NULL, &stWfd, NULL, &stTimeVal);
	if (FD_ISSET(inFileHandle, &stWfd))
	{
		
	}
	else
	{
//		inUtility_StoreTraceLog_OneStep("inETHERNET_Send_Ready_By_Native Fail, errno:%d", errno);
		return (VS_ERROR);
	}
	
	return (VS_SUCCESS);
}


/*
Function        :inETHERNET_Initial
Date&Time       :2017/7/18 下午 3:37
Describe        :
 */
int inETHERNET_Initial(void) {
    int inRetVal = VS_ERROR;
    char szEDCIPAddress[16 + 1];
    char szSubMask[16 + 1];
    char szGatWay[16 + 1];
    char szConfig[16 + 1];
    char szDHCPMode[2 + 1];
    unsigned short usRetVal = 0x00;
    char szDispMsg[16 + 1] = {0};
    unsigned char uszLen = 0;
    
    inSetTermIPAddress(ipAddr);
    inSetTermMASKAddress(mask);
    inSetTermGetewayAddress(gateWay);
    memcpy(&srCPTRec.szHostIPPrimary[0], &hostIp[0], strlen(hostIp));
    memcpy(&srCPTRec.szHostPortNoPrimary[0], &hostPort[0], strlen(hostPort));
    //暫不實現
    //	inDISP_ClearAll();
    //	inFunc_Display_LOGO( 0,  _COORDINATE_Y_LINE_16_2_);				/* 第一層顯示 LOGO */
    //	inDISP_PutGraphic(_MENU_SET_COMM_TITLE_, 0, _COORDINATE_Y_LINE_8_3_);		/* 第三層顯示 <通訊設定> */

    //	inDISP_Timer_Start(_TIMER_NEXSYS_1_, _EDC_TIMEOUT_);

    /* 打開TCP IP */
    /* Portable 機型若沒接上底座再開Ethernet會失敗 */
    /* 這邊要分是否為有底座機型，有底座要先接回才能Initial */
    inRetVal = inETHERNET_Open();

    if (inRetVal == VS_SUCCESS) {
        //printf("inETHERNET_Open  successed\n");
    } else {
        printf("inETHERNET_Open  failed,ret is %d\n",inRetVal);
        return (VS_ERROR);
    }
    /* Set EDC IP Address */
    if (inETHERNET_Cofig_Set(d_ETHERNET_CONFIG_IP, (unsigned char*) srEDCRec.szTermIPAddress, strlen(srEDCRec.szTermIPAddress)) != VS_SUCCESS) {
        printf("inETHERNET_Cofig_Set d_ETHERNET_CONFIG_IP  failed\n");
        return (VS_ERROR);
    }
    /* Set Geteway IP */
    if (inETHERNET_Cofig_Set(d_ETHERNET_CONFIG_GATEWAY, (unsigned char*) srEDCRec.szTermGetewayAddress, strlen(srEDCRec.szTermGetewayAddress)) != VS_SUCCESS) {
        printf("inETHERNET_Cofig_Set d_ETHERNET_CONFIG_GATEWAY  failed\n");
        return (VS_ERROR);
    }

    /* Set EDC SUB Mask */
    if (inETHERNET_Cofig_Set(d_ETHERNET_CONFIG_MASK, (unsigned char*) srEDCRec.szTermMASKAddress, strlen(srEDCRec.szTermMASKAddress)) != VS_SUCCESS) {
        printf("inETHERNET_Cofig_Set d_ETHERNET_CONFIG_MASK  failed\n");
        return (VS_ERROR);
    }
    /*
            d_ETHERNET_CONFIG_UPDATE_EXIT
            Save the settings to the non-volatile memory in Ethernet module.      
     */
    /* 儲存設定 */
    memset(szConfig, 0x00, sizeof (szConfig));
    if (inETHERNET_Cofig_Set(d_ETHERNET_CONFIG_UPDATE_EXIT, (unsigned char*) szConfig, 0) != VS_SUCCESS) {
        printf("inETHERNET_Cofig_Set  d_ETHERNET_CONFIG_UPDATE_EXIT  failed\n");
        return (VS_ERROR);
    }

    /* 魔法，勿動 START 
     * 這個Bug的發生時機在當機器直接斷電時，下次開機後，
     * 連線狀態會一直Timeout。
     * 正規解法是把設定IP、Submask、gateway 放到inETHERNET_SetConfig 再設定
     */
    {
//        memset(szGatWay, 0x00, sizeof (szGatWay));
//        inETHERNET_Cofig_Get(d_ETHERNET_CONFIG_GATEWAY, (unsigned char*) szGatWay, &uszLen);

        //		       if (inGetTermGetewayAddress(szGatWay) == VS_ERROR)
        //		       {
        //                            printf("inGetTermGetewayAddress   failed\n");
        //			       return (VS_ERROR);
        //		       }

        /* Set Geteway IP */
        if (inETHERNET_Cofig_Set(d_ETHERNET_CONFIG_GATEWAY, (unsigned char*) srEDCRec.szTermGetewayAddress, strlen(srEDCRec.szTermGetewayAddress)) != VS_SUCCESS) {
            printf("inETHERNET_Cofig_Set  d_ETHERNET_CONFIG_GATEWAY  again failed\n");
            return (VS_ERROR);
        }
    }
    /* 魔法，勿動 End */
    //	}
    return (VS_SUCCESS);
}

/*
Function        :inETHERNET_Get_Status
Date&Time       :2018/3/9 下午 1:23
Describe        :
*/
int inETHERNET_Get_Status(unsigned int *uiStatus)
{
	unsigned short  usRetval;
	
	if (uiStatus == NULL)
		return (VS_ERROR);
	
	usRetval = CTOS_EthernetStatus(uiStatus);
	
	if (usRetval != d_OK)
	{
            printf( "CTOS_EthernetStatus Error : 0x%04x", usRetval);
	}
	
	return (VS_SUCCESS);
}

/*
Function        :inETHERNET_Watch_Status
Date&Time       :2016/10/11 下午 3:50
Describe        :
*/
int inETHERNET_Watch_Status(void)
{
	unsigned char	uszTemplate[40 + 1];
	unsigned char	uszLen = 0;
	DWORD	uiStatus = 0;
	/* Get the status of the Ethernet */
	inETHERNET_Get_Status(&uiStatus);
	memset(uszTemplate, 0x00, sizeof(uszTemplate));
	if (uiStatus & d_STATUS_ETHERNET_CONNECTED)
	{
		strcat(uszTemplate, "Connected ");
	}
	else
	{
		 strcat(uszTemplate, "NotConnected ");
	}
	
	if (uiStatus & d_STATUS_ETHERNET_PHYICAL_ONLINE)
	{
		strcat(uszTemplate, "Phyical_Online ");
	}
	else
	{
		 strcat(uszTemplate, "NotPhyical_Online ");
	}
		
	if (uiStatus & d_STATUS_ETHERNET_RX_READY)
	{
		strcat(uszTemplate, "RxReady ");
	}
	else
	{
		 strcat(uszTemplate, "NotRxReady ");
	}
	
	if (uiStatus & d_STATUS_ETHERNET_TX_BUSY)
	{
		strcat(uszTemplate, "TxBusy ");
	}
	else
	{
		 strcat(uszTemplate, "NotTxBusy ");
	}
        printf("status %s\n",uszTemplate);
//	/* Get the status of the Ethernet */
//	inETHERNET_Get_Status(&uiStatus);
//
//	/* if Ethernet is connected */
//	if (uiStatus & d_STATUS_ETHERNET_CONNECTED)
//	{
//		if (ginDebug == VS_TRUE)
//		{
//			inLogPrintf(AT, "EthernetStatus : Connected");
//		}
//	}
//	/* if Ethernet is command mode */
//	if (uiStatus & d_STATUS_ETHERNET_COMMAND_MODE)
//	{
//		if (ginDebug == VS_TRUE)
//		{
//			inLogPrintf(AT, "EthernetStatus : Command Mode");
//		}
//	}
//	
//	/* if Ethernet is phyical online */
//	if (uiStatus & d_STATUS_ETHERNET_PHYICAL_ONLINE)
//	{
//		if (ginDebug == VS_TRUE)
//		{
//			inLogPrintf(AT, "EthernetStatus : Phyical Online");
//		}
//	}
//	else
//	{
//		if (ginDebug == VS_TRUE)
//		{
//			inLogPrintf(AT, "網路線沒插好！！");
//		}
//	}
//	
//	/* if Ethernet is Rx readey */
//	if (uiStatus & d_STATUS_ETHERNET_RX_READY)
//	{
//		if (ginDebug == VS_TRUE)
//		{
//			inLogPrintf(AT, "EthernetStatus : Rx Ready");
//		}
//	}
//	
//	/* if Ethernet is Tx busy */
//	if (uiStatus & d_STATUS_ETHERNET_TX_BUSY)
//	{
//		if (ginDebug == VS_TRUE)
//		{
//			inLogPrintf(AT, "EthernetStatus : Tx Busy");
//		}
//	}
	
//	/* IP */
//	memset(uszTemplate, 0x00, sizeof(uszTemplate));
//	uszLen = sizeof(uszTemplate);
//	inETHERNET_Cofig_Get(d_ETHERNET_CONFIG_IP, uszTemplate, &uszLen);
//	
//	/* MASK */
//	memset(uszTemplate, 0x00, sizeof(uszTemplate));
//	uszLen = sizeof(uszTemplate);
//	inETHERNET_Cofig_Get(d_ETHERNET_CONFIG_MASK, uszTemplate, &uszLen);
//	
//	/* GATEWAY */
//	memset(uszTemplate, 0x00, sizeof(uszTemplate));
//	uszLen = sizeof(uszTemplate);
//	inETHERNET_Cofig_Get(d_ETHERNET_CONFIG_GATEWAY, uszTemplate, &uszLen);
//	
//	/* HOSTIP */
//	memset(uszTemplate, 0x00, sizeof(uszTemplate));
//	uszLen = sizeof(uszTemplate);
//	inETHERNET_Cofig_Get(d_ETHERNET_CONFIG_HOSTIP, uszTemplate, &uszLen);
//	
//	/* HOST PORT */
//	memset(uszTemplate, 0x00, sizeof(uszTemplate));
//	uszLen = sizeof(uszTemplate);
//	inETHERNET_Cofig_Get(d_ETHERNET_CONFIG_HOSTPORT, uszTemplate, &uszLen);
//	
//	/* CONFIG VERSION */
//	memset(uszTemplate, 0x00, sizeof(uszTemplate));
//	uszLen = sizeof(uszTemplate);
//	inETHERNET_Cofig_Get(d_ETHERNET_CONFIG_VERSION, uszTemplate, &uszLen);
//	
//	/* CONFIG MAC */
//	memset(uszTemplate, 0x00, sizeof(uszTemplate));
//	uszLen = sizeof(uszTemplate);
//	inETHERNET_Cofig_Get(d_ETHERNET_CONFIG_MAC, uszTemplate, &uszLen);
//	
//	/* CONFIG DHCP */
//	memset(uszTemplate, 0x00, sizeof(uszTemplate));
//	uszLen = sizeof(uszTemplate);
//	inETHERNET_Cofig_Get(d_ETHERNET_CONFIG_DHCP, uszTemplate, &uszLen);
//	
//	/* CONFIG DNSIP */
//	memset(uszTemplate, 0x00, sizeof(uszTemplate));
//	uszLen = sizeof(uszTemplate);
//	inETHERNET_Cofig_Get(d_ETHERNET_CONFIG_DNSIP, uszTemplate, &uszLen);
//	
//	/* CONFIG URL*/
//	memset(uszTemplate, 0x00, sizeof(uszTemplate));
//	uszLen = sizeof(uszTemplate);
//	inETHERNET_Cofig_Get(d_ETHERNET_CONFIG_HOSTURL, uszTemplate, &uszLen);
//	
//	/* CONFIG AUTOCON */
//	memset(uszTemplate, 0x00, sizeof(uszTemplate));
//	uszLen = sizeof(uszTemplate);
//	inETHERNET_Cofig_Get(d_ETHERNET_CONFIG_AUTOCON, uszTemplate, &uszLen);
  
	return (VS_SUCCESS);
}
/* Describe:from example  */
void EthernetPing(const char* ip)
{
  BYTE PingBuff[15+1];
  sprintf(PingBuff,ip);
  unsigned short ret = CTOS_EthernetPing(PingBuff, strlen(PingBuff));
  if (ret == d_OK){
      printf("ping %s successed\n",PingBuff);
  }else{
      printf("ping %s failed,ret is %X\n",PingBuff,ret);
  }
}
/*
Function        :inETHERNET_Send_Ready_Flow
Date&Time       :2017/8/1 下午 2:19
Describe        :分流
*/
int inETHERNET_Send_Ready_Flow()
{
	int	inRetVal = VS_ERROR;
	
//	if (ginEthernetFlow == _ETHERNET_FLOW_IFES_)
//	{
//                if (_TLS_API_WAY_ == _TLS_API_WAY_OPENSSL_)
//                {
//                       inRetVal = inETHERNET_Send_Ready_TLS_OPENSSL(&gSsl); 
//                }
//                else
//                {
//                        inRetVal = inETHERNET_Send_Ready_TLS_CTOS(guiSSL_ID);
//                }
//	}
	/* 用native方式建Socket，之後要優化再考慮使用(優點自己決定Timeout等等細微設定) */
//	else if (ginEthernetFlow == _ETHERNET_FLOW_NATIVE_)
//	{
		inRetVal = inETHERNET_Send_Ready_By_Native(ginTrans_ClientFd);
//	}
//	/* 虹堡API */
//	else
//	{
//		inRetVal = inETHERNET_Send_Ready();
//	}
	
	return (inRetVal);
}

/*
Function        :inETHERNET_Send_Data_By_Native
Date&Time       :2017/8/1 下午 2:28
Describe        :傳送資料
*/
int inETHERNET_Send_Data_By_Native(int inFileHandle, unsigned char* uszData, unsigned short usLen)
{
	int	inTempLen = 0;
	char	szDebugMsg[100 + 1];
	
	/* Send message */  
	inTempLen = send(inFileHandle, uszData, usLen, 0);
	
	if (inTempLen >= 0)
	{
            
		return (VS_SUCCESS);
	}
	else
	{
//		if (ginDebug == VS_TRUE)
//		{
//			memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//			sprintf(szDebugMsg, "send Errno: %d", errno);
//			inLogPrintf(AT, szDebugMsg);
//		}
//		inUtility_StoreTraceLog_OneStep("inETHERNET_Send_Data_By_Native Fail, Errno: %d", errno);
//		
		return (VS_ERROR);
	}
	
}
/*
Function        :inETHERNET_Send_Data_Flow
Date&Time       :2017/8/1 下午 2:19
Describe        :分流
*/
int inETHERNET_Send_Data_Flow(unsigned char* uszData, unsigned short usLen)
{
	int	inRetVal = VS_ERROR;
	
//	if (ginEthernetFlow == _ETHERNET_FLOW_IFES_)
//	{
//                if (_TLS_API_WAY_ == _TLS_API_WAY_OPENSSL_)
//                {
//                    inRetVal = inETHERNET_Send_Data_TLS_OPENSSL(&gSsl, uszData, usLen);
//                }
//                else
//                {
//                    inRetVal = inETHERNET_Send_Data_TLS_CTOS(guiSSL_ID, uszData, usLen);
//                }
//	}
	/* 用native方式建Socket，之後要優化再考慮使用(優點自己決定Timeout等等細微設定) */
//	else if (ginEthernetFlow == _ETHERNET_FLOW_NATIVE_)
//	{
		inRetVal = inETHERNET_Send_Data_By_Native(ginTrans_ClientFd, uszData, usLen);
//	}
//	else
//	{
//		inRetVal = inETHERNET_Send_Data(uszData, usLen);
//	}
	
	return (inRetVal);
}

/*
Function        :inETHERNET_Send
Date&Time       :2017/7/18 下午 5:25
Describe        :送電文，並根據電文格式前面加長度
*/
int inETHERNET_Send(unsigned char *uszSendBuff, int inSendSize, int inSendTimeout)
{
        char            szDataHead[1 + 1];
	char		szDebugMsg[100 + 1];
        unsigned char   szSendData[inSendSize + 32];

//        memset(szSendData, 0x00, sizeof(szSendData));
//        memset(szDataHead, 0x00, sizeof(szDataHead));
//
//        /* inETHERNET_Send() START! */
//        if (ginDebug == VS_TRUE)
//                inLogPrintf(AT, "inETHERNET_Send() START!");
//
//        /* Get HeadFormat */
//        if (inGetTCPHeadFormat(szDataHead) == VS_ERROR)
//        {
//                /* inGetTCPHeadFormat ERROR */
//                if (ginDebug == VS_TRUE)
//                        inLogPrintf(AT, "inGetTCPHeadFormat Error!");
//                
//                return (VS_ERROR);
//        }
	
	/*	範例:Size = 1024
	 * 	H:0x04 0x00
	 *	B:0x10 0x24
	 */
//        if (szDataHead[0] == 'H')
//        {
//		szSendData[0] = ((inSendSize & 0x0000FF00) >> 8);
//		szSendData[1] = (inSendSize & 0x000000FF);
//        }
//        else
//        {
//		szSendData[0] = (inSendSize / 100) / 10 * 16 + (inSendSize / 100) % 10;
//		szSendData[1] = (inSendSize % 100) / 10 * 16 + (inSendSize % 100) % 10;
//        }
	
//	if (ginISODebug == VS_TRUE)
//	{
//		inPRINT_ChineseFont("----------------------------------------",_PRT_ISO_);
//		memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//		sprintf(szDebugMsg, "%02X %02X :length = %d", szSendData[0], szSendData[1], inSendSize);
//		inPRINT_ChineseFont_Format(szDebugMsg, "  ", 34, _PRT_ISO_);
//		inPRINT_ChineseFont("----------------------------------------", _PRT_ISO_);
//	}
//	if (ginDebug == VS_TRUE)
//	{
//		inLogPrintf(AT, "----------------------------------------");
//		memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//		sprintf(szDebugMsg, "%02X %02X :length = %d", szSendData[0], szSendData[1], inSendSize);
//		inLogPrintf(AT, szDebugMsg);
//		inLogPrintf(AT, "----------------------------------------");
//	}

        memcpy(&szSendData[2], uszSendBuff, inSendSize);

        /* 因為多放長度所以要加2Byte */
        inSendSize += 2;

	/* 有Timeout，要計時 */
	if (inSendTimeout != 0)
                inDISP_Timer_Start(_TIMER_NEXSYS_1_, inSendTimeout);

//	while (1)
//	{
//		if (inSendTimeout != 0)
//                {
//                        if (inTimerGet(_TIMER_NEXSYS_1_) == VS_SUCCESS)
//                        {
////                                if (ginDebug == VS_TRUE)
////                                {
////                                        inLogPrintf(AT, "inETHERNET_Send() TIMEOUT");
////				}
//				
//				return (VS_ERROR);
//			}
//		}

		/* 如果可以Send就Send */
		if (inETHERNET_Send_Ready_Flow()== VS_SUCCESS)
		{
//			if (inETHERNET_Send_Data_Flow(szSendData, inSendSize) != VS_SUCCESS)
                    
                        if (inETHERNET_Send_Data_Flow(uszSendBuff, inSendSize) != VS_SUCCESS)
			{
                            printf("send data failed\n");
//				continue;
			}
			else
                            printf("send data successed\n"); 
//				break;
		}
//	}

//        /* inETHERNET_Send() END! */
//        if (ginDebug == VS_TRUE)
//                inLogPrintf(AT, "inETHERNET_Send() END!");

        return (VS_SUCCESS);
}

/*
Function        :inETHERNET_Receive_Ready_By_Native
Date&Time       :2017/8/1 下午 3:10
Describe        :
*/
int inETHERNET_Receive_Ready_By_Native(int inFileHandle, unsigned short *usLen)
{
	fd_set		stRfd;			/* 用來判斷是否可以讀的Fd */
	struct timeval	stTimeVal;
	
	stTimeVal.tv_sec = 0;
	stTimeVal.tv_usec = 0;
	
	/* 將 set 整個清為零。 */
	FD_ZERO(&stRfd);
	/* 將 fd 新增到 set。 */
	FD_SET(inFileHandle, &stRfd);
	
	/* 初始化 */
	*usLen = 0;
	/* nfds is the highest-numbered file descriptor in any of the three sets, plus 1. FD最高位再加一 */
	/* 若Timeout仍未有Fd，則回傳0 */
	select(inFileHandle + 1, &stRfd, NULL, NULL, &stTimeVal);
	if (FD_ISSET(inFileHandle, &stRfd))
	{
		/* Native沒有check功能，所以直接設大一點，等到read時再根據實際回傳讀取 */
		*usLen = _COMM_RECEIVE_MAX_LENGTH_;
	}
	else
	{
		return (VS_ERROR);
	}
	
	return (VS_SUCCESS);
}
/*
Function        :inETHERNET_Receive_Ready_Flow
Date&Time       :2017/8/1 下午 2:47
Describe        :目前只有虹堡API才有receive check的功能，用native沒辦法實做，所以決定不使用
 */
int inETHERNET_Receive_Ready_Flow(unsigned short *usLen)
{
	int		inRetVal;
	unsigned short	usBufferMaxLen = 0;

//	if (ginEthernetFlow == _ETHERNET_FLOW_IFES_)
//	{
//                if (_TLS_API_WAY_ == _TLS_API_WAY_OPENSSL_)
//                {
//                        inRetVal = inETHERNET_Receive_Ready_TLS_OPENSSL(&gSsl, usLen);
//                }
//                else
//                {
//                        inRetVal = inETHERNET_Receive_Ready_TLS_CTOS(guiSSL_ID, usLen);
//                }
//	}
	/* 用native方式建Socket，之後要優化再考慮使用(優點自己決定Timeout等等細微設定) */
//	else if (ginEthernetFlow == _ETHERNET_FLOW_NATIVE_)
//	{
		inRetVal = inETHERNET_Receive_Ready_By_Native(ginTrans_ClientFd, usLen);
//	}
//	else
//	{
//		usBufferMaxLen = *usLen;
//		inRetVal = inETHERNET_Receive_Ready(usLen);
//		if (*usLen > usBufferMaxLen)
//		{
//			*usLen = usBufferMaxLen;
//		}
//	}
	
	return (inRetVal);
}
/*
Function        :inETHERNET_Receive_Data_By_Native
Date&Time       :2017/8/1 下午 3:45
Describe        :
*/
int inETHERNET_Receive_Data_By_Native(int inFileHandle, unsigned char* uszData, unsigned short *usLen)
{
	int	inTempLen = 0;
	
	
	inTempLen = recv(inFileHandle, uszData, *usLen, 0);
	
	/* 如果回傳-1會變成65535，所以要做安全檢核 */
	if (inTempLen > 0)
	{
            printf("receive successed, ret length is %d\n",inTempLen);
            *usLen = (unsigned short)inTempLen;
	}
	else
	{
            *usLen = 0;
		
//		char	szDebugMsg[100 + 1];
//		if (ginDebug == VS_TRUE)
//		{
//			memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//			sprintf(szDebugMsg, "recv Errno: %d", errno);
//			inLogPrintf(AT, szDebugMsg);
//		}
		
		return (VS_ERROR);
	}

	
	
	return (VS_SUCCESS);
}

/*
Function        :inETHERNET_Receive_Data_Flow
Date&Time       :2017/8/1 下午 3:43
Describe        :分流
*/
int inETHERNET_Receive_Data_Flow(unsigned char* uszData, unsigned short *usLen)
{
	int	inRetVal = VS_ERROR;
	
//	if (ginEthernetFlow == _ETHERNET_FLOW_IFES_)
//	{
//                if (_TLS_API_WAY_ == _TLS_API_WAY_OPENSSL_)
//                {
//                    inRetVal = inETHERNET_Receive_Data_TLS_OPENSSL(&gSsl, uszData, usLen);
//                }
//                else
//                {
//                    inRetVal = inETHERNET_Receive_Data_TLS_CTOS(guiSSL_ID, uszData, usLen);
//                }
//	}
	/* 用native方式建Socket，之後要優化再考慮使用(優點自己決定Timeout等等細微設定) */
//	else if (ginEthernetFlow == _ETHERNET_FLOW_NATIVE_)
//	{
		inRetVal = inETHERNET_Receive_Data_By_Native(ginTrans_ClientFd, uszData, usLen);
//	}
//	else
//	{
//		inRetVal = inETHERNET_Receive_Data(uszData, usLen);
//	}
	
	
	return (inRetVal);
}

/*
Function        :inETHERNET_Receive
Date&Time       :2017/7/18 下午 5:55
Describe        :
*/
int inETHERNET_Receive(unsigned char *uszReceiveBuff, int inReceiveSize, int inReceiveTimeout)
{
	int		inRetVal = 0;
        int     	inReceivelen = 0;			/* Comport當前收到的長度 */
        int     	inDataLength = 0;			/* 收到的資料長度(不含Head) */
	int		inExpectLength = 0;
        char    	szDataHead[1 + 1];
//        char    	szDebugMsg[_DEBUG_MESSAGE_SIZE_ + 1];   /* debug message */
        unsigned char 	uszRawBuffer[inReceiveSize + 1];
	unsigned short 	usOutputLen = 0;			/* 目前comport可以讀取的資料長度 */
	
        /* inETHERNET_Receive() START! */
	memset(uszRawBuffer, 0x00, sizeof(uszRawBuffer));

        /* 設定接收Timeout時間 */
	if (inReceiveTimeout != 0)
                inDISP_Timer_Start(_TIMER_NEXSYS_1_, inReceiveTimeout);

	/* 第一段先收到comport沒資料為止，或Timeout */
	while (1)
	{
		if (inReceiveTimeout != 0)
                {
                        if (inTimerGet(_TIMER_NEXSYS_1_) == VS_SUCCESS)
                        {
                            printf("inETHERNET_Receive TimeOut 1\n");
                            return (VS_TIMEOUT);
                        }
                }
		
		/* 設定為剩餘的Buffer空間大小，才符合API的用法 */
		usOutputLen = inReceiveSize - inReceivelen;
		
		if (inETHERNET_Receive_Ready_Flow(&usOutputLen) == VS_SUCCESS)
		{
			inRetVal = inETHERNET_Receive_Data_Flow(&uszRawBuffer[inReceivelen], &usOutputLen);
			if (inRetVal == VS_SUCCESS)
			{
                                printf("inETHERNET_Receive_Data_Flow successed\n");
				inReceivelen = inReceivelen + (int)usOutputLen;
			}
		}

		/* 沒有可以收的就跳出去(usOutputLen == 0代表沒有可已從buffer中讀取的資料) */
		if (inReceivelen > 0 && usOutputLen == 0)
		{
			break;
		}
	}
        memcpy(&uszReceiveBuff[0],&uszRawBuffer[0],inReceivelen);
        return inReceivelen;
        /* Get HeadFormat */
	/*	範例:Size = 1024
	 * 	H:0x04 0x00
	 *	B:0x10 0x24
	 */
//        memset(szDataHead, 0x00, sizeof(szDataHead));
//        if (inGetTCPHeadFormat(szDataHead) == VS_ERROR)
//        {
//                /* inGetTCPHeadFormat ERROR */
//                /* debug */
//                if (ginDebug == VS_TRUE)
//		{
//			inLogPrintf(AT, "inGetTCPHeadFormat() ERROR!!");
//		}
//		inUtility_StoreTraceLog_OneStep("inGetTCPHeadFormat() ERROR!!");
//
//                return (VS_ERROR);
//        }
//	
//	/* 如果有收到，就解析Head的封包長度並確認是否全收到 */
//	/* 因為封包欄位佔2bytes，所以至少應該收到2byte */
//	if (inReceivelen >= 2)
//	{
//		/* 根據Head算出預期長度 */
//		if (szDataHead[0] == 'H')
//		{
//			inExpectLength = uszRawBuffer[0] * 256 + uszRawBuffer[1];
//		}
//		else
//		{
//			inExpectLength = ((uszRawBuffer[0] / 16 * 10 + uszRawBuffer[0] % 16 ) * 100) + (uszRawBuffer[1] / 16 * 10 + uszRawBuffer[1] % 16);
//		}
//		
//		if (ginISODebug == VS_TRUE)
//		{
//			inPRINT_ChineseFont("----------------------------------------",_PRT_ISO_);
//			memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//			sprintf(szDebugMsg, "%02X %02X :length = %d", uszRawBuffer[0], uszRawBuffer[1], inExpectLength);
//			inPRINT_ChineseFont_Format(szDebugMsg, "  ", 34, _PRT_ISO_);
//			inPRINT_ChineseFont("----------------------------------------", _PRT_ISO_);
//		}
//		if (ginDebug == VS_TRUE)
//		{
//			inLogPrintf(AT, "----------------------------------------");
//			memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//			sprintf(szDebugMsg, "%02X %02X :length = %d", uszRawBuffer[0], uszRawBuffer[1], inExpectLength);
//			inLogPrintf(AT, szDebugMsg);
//			inLogPrintf(AT, "----------------------------------------");
//		}
//
//		
//		memcpy(&uszReceiveBuff[inDataLength], &uszRawBuffer[2], inReceivelen - 2);
//		/* inDataLength為目前收到的封包數 */
//		inDataLength += inReceivelen - 2;
//		
//		/* 比對目前接收的長度(含Length)是否與主機下的長度一致(inDataLength = inExpectLength)，若小於應接收長度則繼續接收剩下的封包 */
//		while (inDataLength < inExpectLength)
//		{
//			/* 準備收下一次，清空暫存Buffer */
//			inReceivelen = 0;
//			memset(uszRawBuffer, 0x00, sizeof(uszRawBuffer));
//			
//                        while (1)
//                        {
//				/* Timeout的話 */
//                                if (inReceiveTimeout != 0)
//                                {
//                                        if (inTimerGet(_TIMER_NEXSYS_1_) == VS_SUCCESS)
//                                        {
//						if (ginDebug == VS_TRUE)
//						{
//							if (inReceiveTimeout != 0)
//							{
//								inLogPrintf(AT, "inETHERNET_Receive TimeOut 2");
//								memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//								sprintf(szDebugMsg, "Receve Len: %d", inDataLength);
//								inLogPrintf(AT, szDebugMsg);
//							}
//
//							inUtility_StoreTraceLog_OneStep("inETHERNET_Receive TimeOut 2");
//						}
//                                                return (VS_TIMEOUT);
//                                        }
//                                }
//
//				/* 設定為剩餘的Buffer空間大小，才符合API的用法 */
//				usOutputLen = inReceiveSize - inReceivelen;
//				
//				if (inETHERNET_Receive_Ready_Flow(&usOutputLen) == VS_SUCCESS)
//				{
//					inRetVal = inETHERNET_Receive_Data_Flow(&uszRawBuffer[inReceivelen], &usOutputLen);
//					if (inRetVal == VS_SUCCESS)
//					{
//						inReceivelen = inReceivelen + (int)usOutputLen;
//					}
//				}
//
//				/* 沒有可以收的就跳出去(usOutputLen == 0代表沒有可已從buffer中讀取的資料) */
//				if (inReceivelen > 0 && usOutputLen == 0)
//				{
//					break;
//				}
//                        }
//
//                        memcpy(&uszReceiveBuff[inDataLength], &uszRawBuffer[0], inReceivelen);
//                        inDataLength += inReceivelen;
//		}
//
//	}
//	/* 連Head都沒收到 */
//	else
//	{
//		return (VS_ERROR);
//	}
	     

	return (inDataLength);
}