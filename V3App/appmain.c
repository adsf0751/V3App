
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctosapi.h>
#include <fcntl.h>
#include "Trans.h"
#include "Print.h"
#include "PrtMsg.h"

#include "SOURCE/FUNCTION/Function.h"
#include "SOURCE/FUNCTION/File.h"
#include "SOURCE/COMM/Ethernet.h"
#include "SOURCE/NCCC/NCCCats.h"
#include "SOURCE/PRINT/Print.h"
#include "SOURCE/FUNCTION/ECR.h"
#include "SOURCE/ECR_Struct.h"
#include "SOURCE/CREDIT/CreditptrByBuffer.h"

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

#define _EDC_TIMEOUT_           -1  /* 由EDC.dat控制 */
#define AF_INET                 PF_INET
#define	PF_INET                 2	/* IP protocol family.  */
#define SOL_SOCKET              1
#define SO_REUSEADDR            2
#define SOL_TCP                 6	/* TCP level */
#define	TCP_MAXSEG              2	/* Set maximum segment size  */
extern MY_ECR_DATA myECRTable[43];
/*
Function        :inFunc_Booting_Flow_Print_Image_Initial
Date&Time       :2018/6/5 下午 6:27
Describe        :開機流程列印圖片初始化，為了避免開機更新導致抓不到圖片高度
 */
int inFunc_Booting_Flow_Print_Image_Initial(TRANSACTION_OBJECT *pobTran,BMPHeight* gsrBMPHeight) {
    /* 若沒下TMS會沒圖片抓高度，所以不判斷成功或失敗 */
    inPRINT_Buffer_GetHeightFlow(gsrBMPHeight);
    return (VS_SUCCESS);
}

void vdEthernetMenu(void)
{
    BYTE key;
    int breakFlag = 0;
    int inRetVal = VS_ERROR;
    BYTE uszPackBuf[984];
    memset(uszPackBuf,0x00,sizeof(uszPackBuf));
    int inCnt = myPackData(uszPackBuf);
    unsigned char   uszTemplate[60 + 1];
    BYTE uszRecvPacket[_NCCC_ATS_ISO_SEND_ + 1];
    int  inReceiveTimeout = 10;
    int	 inReceiveSize = _COMM_RECEIVE_MAX_LENGTH_;
    while(1)
    {
        CTOS_LCDTClearDisplay();
        CTOS_LCDTPrintXY(1, 1, "Ethernet Menu");
        CTOS_LCDTPrintXY(1, 2, "1:Set Config");
        CTOS_LCDTPrintXY(1, 3, "2:Get Config");
        CTOS_LCDTPrintXY(1, 4, "3:Connect");
        CTOS_LCDTPrintXY(1, 5, "4:Tx Data");
        CTOS_LCDTPrintXY(1, 6, "5.Rx Data");
//        CTOS_LCDTPrintXY(1, 7, "6.Ping");
        CTOS_LCDTPrintXY(1, 16, "X.Exit");
        CTOS_KBDGet(&key);
        breakFlag = 0;
        switch(key)
        { 
            case d_KBD_1: 
            { 
                CTOS_LCDTClearDisplay();               
                inRetVal = inETHERNET_Initial();
                if(inRetVal == VS_SUCCESS)
                {
                    CTOS_LCDTPrintXY(1, 1, "Config Set Success!!!");
                    CTOS_Delay(2000);
                }
                else
                {
                    CTOS_LCDTPrintXY(1, 1, "Config Set Fail!!!");
                    CTOS_Delay(2000);
                }
                break;                
            }
            case d_KBD_2: 
            {   
                CTOS_LCDTClearDisplay();
                memset(uszTemplate, 0x00, sizeof(uszTemplate));               
                memcpy(uszTemplate,"IP:", 3);
                vdEthernetGetNetWorkValue(d_ETHERNET_CONFIG_IP,(unsigned char*)uszTemplate);
                CTOS_LCDTPrintXY(1,1,uszTemplate);

                memset(uszTemplate, 0x00, sizeof(uszTemplate));               
                memcpy(uszTemplate,"GW:", 3);
                vdEthernetGetNetWorkValue(d_ETHERNET_CONFIG_GATEWAY,(unsigned char*)uszTemplate);
                CTOS_LCDTPrintXY(1,2,uszTemplate);

                memset(uszTemplate, 0x00, sizeof(uszTemplate));               
                memcpy(uszTemplate,"MASK:", 5);
                vdEthernetGetNetWorkValue(d_ETHERNET_CONFIG_MASK,(unsigned char*)uszTemplate);
                CTOS_LCDTPrintXY(1,3,uszTemplate);

                CTOS_Delay(2000);
                break;
            }
            case d_KBD_3: 
            { 
                CTOS_LCDTClearDisplay();
                if(inETHERNET_SetConfig() == VS_SUCCESS)
                {
                    CTOS_LCDTPrintXY(1,1,"Connect Host Success!!!");
                    CTOS_Delay(2000);
                }
                else
                {
                    CTOS_LCDTPrintXY(1, 1, "Connect Host Fail!!!");
                    CTOS_Delay(2000);
                }
                break;
            }
            case d_KBD_4: 
            {
                CTOS_LCDTClearDisplay(); 
                /*
                 * 原先傳到主機的長度對不起來，原因是inCnt已包含電文前面的長度
                 * 但inETHERNET_Send預設傳入inSendSize 是未包含Message Length，
                 * 所以在此function會自動+2，為了不改動用到function，故再此先讓inSendSize -2
                 */
                if( inETHERNET_Send(uszPackBuf,inCnt-2,0) == VS_SUCCESS)
                {
                    CTOS_LCDTPrintXY(1,1,"EthernetTx Success!!!");
                    CTOS_Delay(2000);
                }
                else
                {
                    CTOS_LCDTPrintXY(1,1,"EthernetTx Fail!!!");
                    CTOS_Delay(2000);
                }
                break;
            }
            case d_KBD_5: 
            {   //BUG:Tx完按Rx可以正常使用，但只按RX多次會報錯，先記錄，暫時不改
                CTOS_LCDTClearDisplay(); 
                inReceiveSize = inETHERNET_Receive(uszRecvPacket,inReceiveSize,inReceiveTimeout);
                if(inReceiveSize > 0 )
                {
                    myUnPackData( uszRecvPacket , inReceiveSize);
                    CTOS_LCDTPrintXY(1,1,"EthernetRx Success!!!");
                    CTOS_Delay(2000);
                }
                else
                {
                   CTOS_LCDTPrintXY(1,1,"EthernetRx Fail!!!");
                   CTOS_Delay(2000); 
                }
                break;
            }
            case d_KBD_6: 
            {   //TODO:未使用到ETHERNET_PING()
                break;
            }
            case d_KBD_CANCEL: 
            { 
                breakFlag = 1;
                CTOS_LCDTClearDisplay(); 
                if(inETHERNET_END() == VS_SUCCESS)
                {
                    CTOS_LCDTPrintXY(1,1,"EthernetDisconnSuccess!!!");
                    CTOS_Delay(2000);
                }
                else
                {
                    CTOS_LCDTPrintXY(1,1,"EthernetDisconnFail!!!");
                    CTOS_Delay(2000);
                }
                break;
            }
        }
        if(breakFlag)
            break;
    }
}

void vdECRMenu(TRANSACTION_OBJECT* pobTran)
{
    BYTE key;
    int breakFlag = 0;
    int inRetVal = 0;
    ECR_TABLE   gsrECROb = {.srSetting.uszComPort = d_COM2};
    while(1)
    {
        CTOS_LCDTClearDisplay();
        CTOS_LCDTPrintXY(1, 1, "Ecr Menu");
        CTOS_LCDTPrintXY(1, 2, "1:Set Config");
        
        CTOS_LCDTPrintXY(1, 3, "2:Rx Data");
        CTOS_LCDTPrintXY(1, 4, "3:Tx Data");
        CTOS_LCDTPrintXY(1, 16, "X.Exit");
        CTOS_KBDGet(&key);
        breakFlag = 0;
        switch(key)
        { 
            case d_KBD_1: 
            { 
                CTOS_LCDTClearDisplay();               
                if(inECR_Initial() == VS_SUCCESS)
                {
                    CTOS_LCDTPrintXY(1, 1, "RS232ConnSuccess!!!");
                    CTOS_Delay(2000);
                }
                else
                {
                    CTOS_LCDTPrintXY(1, 1, "RS232ConnFail!!!");
                    CTOS_Delay(2000);
                }
                break;                
            }
            case d_KBD_2: 
            {   
                CTOS_LCDTClearDisplay();
                CTOS_LCDTPrintXY(1, 1, "Processing....");
                inRetVal = inRS232_ECR_8N1_Standard_Receive_Packet(pobTran, &gsrECROb);
                CTOS_LCDTClearDisplay();
                if( inRetVal == VS_SUCCESS)
                {
                    CTOS_LCDTPrintXY(1,1,"Rx Data Success!!!");
                    CTOS_Delay(2000);
                }
                else
                {
                    CTOS_LCDTPrintXY(1,1,"Rx Data Fail!!!");
                    CTOS_Delay(2000);
                }
                printf("***-------------------------***\n");
                int i;
                for(i= 0; i< 43 ;i++)
                {
                    printf("%s:[%s]\n",myECRTable[i].FieldName,myECRTable[i].Data);
                }

                printf("***-------------------------***\n");
                break;
            }
            //BUG:單純做Tx會傳送電文時失敗, 超過重試次數，但做Rx+Tx可以，不確定是否為正常流程
            case d_KBD_3: 
            {              
                CTOS_LCDTClearDisplay();
                CTOS_LCDTPrintXY(1, 1, "Processing....");
                inRetVal = inRS232_ECR_8N1_Standard_Send_Packet(&pobTran, &gsrECROb);
                CTOS_LCDTClearDisplay();
                if(  inRetVal == VS_SUCCESS)
                {
                    CTOS_LCDTPrintXY(1,1,"Tx Data Success!!!");
                    CTOS_Delay(2000);
                }
                else
                {
                    CTOS_LCDTPrintXY(1,1,"Tx Data Fail!!!");
                    CTOS_Delay(2000);
                }
               
                break;
            }
            
            case d_KBD_CANCEL: 
            { 
                breakFlag = 1;
                CTOS_LCDTClearDisplay(); 
                if(inRS232_Close(gsrECROb.srSetting.uszComPort) == VS_SUCCESS)
                {
                    CTOS_LCDTPrintXY(1,1,"RS232DisconnSuccess!!!");
                    CTOS_Delay(2000);
                }
                else
                {
                    CTOS_LCDTPrintXY(1,1,"RS232DisconnFail!!!");
                    CTOS_Delay(2000);
                }
                break;
            }
        }
        if(breakFlag)
            break;
    }
}
int main(int argc, char *argv[]) {
    BYTE key;
    CTOS_LCDTClearDisplay();
    int  inRetVal = 0;
    unsigned char uszBuffer[PB_CANVAS_X_SIZE * 8 * _BUFFER_MAX_LINE_];
    BufferHandle srBhandle;
    FONT_ATTRIB srFont_Attrib;
    TRANSACTION_OBJECT pobTran;
    BMPHeight gsrBMPHeight;
    /*可查看是否有fs_data路徑(是否有Load img.mci)*/
//    inFunc_ls("-R -l", _AP_ROOT_PATH_); 
    
    CTOS_LCDTPrintXY(1, 1, "System Startup");
    CTOS_LCDTPrintXY(1, 2, "Loading Image...");
    CTOS_LCDTPrintXY(1, 3, "Configuring Printer...");
    CTOS_Delay(1000);
//    CTOS_KBDGet(&key);
    //在Load mci時，要先Load APP.mci，再來才是img.mci
    inFunc_Booting_Flow_Print_Image_Initial(&pobTran,&gsrBMPHeight);
    printf("初始化圖片預設高度\n");
    printf("%s:inBankLogoHeight is %d \n",     _BANK_LOGO_,      gsrBMPHeight.inBankLogoHeight);
    printf("%s:inMerchantLogoHeight is %d \n", _MERCHANT_LOGO_,  gsrBMPHeight.inMerchantLogoHeight);
    printf("%s:inTitleNameHeight is %d \n",    _NAME_LOGO_,      gsrBMPHeight.inTitleNameHeight);
    /*============印表機設定============*/
    CTOS_PrinterFontSelectMode(d_FONT_TTF_MODE);
    //    CTOS_FontTTFSelectFontFile(d_FONT_DEVICE_PRINTER, "ca_default.ttf", 0);
    CTOS_FontTTFSelectFontFile(d_FONT_DEVICE_PRINTER, "tsuyuan.ttf", 0);
    CTOS_FontTTFSelectStyle(d_FONT_DEVICE_PRINTER, d_FONT_STYLE_NORMAL);
    /*============印表機設定============*/
    int breakFlag = 0;
    inPRINT_Buffer_Initial(uszBuffer, _BUFFER_MAX_LINE_, &srFont_Attrib, &srBhandle);

    while(1)
    {
        CTOS_LCDTClearDisplay();
        CTOS_LCDTPrintXY(1, 1, "Main Menu");
        CTOS_LCDTPrintXY(1, 2, "1:Print Receipt");
        CTOS_LCDTPrintXY(1, 3, "2:Ethernet COMM");
        CTOS_LCDTPrintXY(1, 4, "3:Rs232 COMM");
        CTOS_LCDTPrintXY(1, 16, "X:Exit");
        CTOS_KBDGet(&key);
        breakFlag = 0;
        switch(key)
        { 
            case d_KBD_1: 
            { 
                /*============假資料============*/
                pobTran.srBRec.inPrintOption = _PRT_CUST_;
                strcpy(pobTran.srBRec.szCardLabel, "9中9文9字9"); //卡別
                strcpy(pobTran.srBRec.szPAN, "252500001616"); //卡號
                strcpy(pobTran.srBRec.szDate, "1223"); //日期
                strcpy(pobTran.srBRec.szTime, "143059"); //時間
                pobTran.srBRec.lnOrgInvNum = 2;
                pobTran.srBRec.lnBatchNum = 3;
                strcpy(pobTran.srBRec.szAuthCode, "123456");
                strcpy(pobTran.srBRec.szRefNo, "999999");
                /*============假資料============*/
                if ((inRetVal = inCREDIT_PRINT_Logo_ByBuffer(&pobTran, uszBuffer, &srFont_Attrib, &srBhandle,&gsrBMPHeight)) != VS_SUCCESS)
                    printf("inCREDIT_PRINT_Logo_ByBuffer failed, ret=%d\n",inRetVal);
                if ((inRetVal = inCREDIT_PRINT_Tidmid_ByBuffer(&pobTran, uszBuffer, &srFont_Attrib, &srBhandle)) != VS_SUCCESS)
                    printf("inCREDIT_PRINT_Tidmid_ByBuffer failed, ret=%d\n",inRetVal);
                if ((inRetVal = inCREDIT_PRINT_Data_ByBuffer(&pobTran, uszBuffer, &srFont_Attrib, &srBhandle)) != VS_SUCCESS)
                    printf("inCREDIT_PRINT_Data_ByBuffer failed, ret=%d\n",inRetVal);
                if ((inRetVal = inPRINT_Buffer_OutPut(uszBuffer, &srBhandle)) != VS_SUCCESS)
                    printf("inPRINT_Buffer_OutPut failed, ret=%d\n",inRetVal);
               break;
            }
            //BUG:進入選單直接選擇跳出會因為 跳出做disconnet 但因為沒有open過而顯示diconn fail
            case d_KBD_2: 
            {   
                vdEthernetMenu();
                break;
            }
            case d_KBD_3: 
            { 
                vdECRMenu(&pobTran);
                break;
            }
            case d_KBD_CANCEL: 
            { 
                breakFlag = 1;
                break;
            }
        }
        if(breakFlag)
            break;
    }
    exit(0);
}
