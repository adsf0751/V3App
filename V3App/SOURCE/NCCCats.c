#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "NCCC/NCCCats.h"
#include "FUNCTION/Function.h" //_PADDING_RIGHT_
#include "../Trans.h" // VS_SUCCESS/VS_ERROR
ISO_FIELD_TYPE_NCCC_ATS_TABLE srNCCC_ATS_ISOFieldType[] =
{
        {3,             _NCCC_ATS_ISO_BCD_,         VS_FALSE,       6},
        {4,             _NCCC_ATS_ISO_BCD_,         VS_FALSE,       12},
        {11,            _NCCC_ATS_ISO_BCD_,         VS_FALSE,       6},
//        {12,            _NCCC_ATS_ISO_BCD_,         VS_FALSE,       6},
//        {13,            _NCCC_ATS_ISO_BCD_,         VS_FALSE,       4},
        {22,            _NCCC_ATS_ISO_BCD_,         VS_FALSE,       4},
        {24,            _NCCC_ATS_ISO_BCD_,         VS_FALSE,       4},
        {25,            _NCCC_ATS_ISO_BCD_,         VS_FALSE,       2},
        {32,            _NCCC_ATS_ISO_BYTE_1_,      VS_FALSE,	    6},
        {35,            _NCCC_ATS_ISO_NIBBLE_2_,    VS_FALSE,       64},
        {37,            _NCCC_ATS_ISO_ASC_,         VS_FALSE,       24},
//        {38,            _NCCC_ATS_ISO_ASC_,         VS_FALSE,       6},
//        {39,            _NCCC_ATS_ISO_ASC_,         VS_FALSE,       2},
        {41,            _NCCC_ATS_ISO_ASC_,         VS_FALSE,       16},
        {42,            _NCCC_ATS_ISO_ASC_,         VS_FALSE,       30},
        {48,            _NCCC_ATS_ISO_BYTE_3_,      VS_TRUE,        42},
//        {52,            _NCCC_ATS_ISO_BCD_,         VS_FALSE,       16},
//        {54,            _NCCC_ATS_ISO_BYTE_3_,      VS_TRUE,        0},
        {55,            _NCCC_ATS_ISO_BYTE_3_,      VS_FALSE,       280},
        {57,            _NCCC_ATS_ISO_BYTE_3_,      VS_FALSE,       24},
//        {58,            _NCCC_ATS_ISO_BYTE_3_,      VS_TRUE,        0},
        {59,            _NCCC_ATS_ISO_BYTE_3_,      VS_TRUE,        292},
        {60,            _NCCC_ATS_ISO_BYTE_3_,      VS_TRUE,        12},
        {62,            _NCCC_ATS_ISO_BYTE_3_,      VS_TRUE,        12},
        {63,            _NCCC_ATS_ISO_BYTE_3_,      VS_TRUE,        0},
//        {64,            _NCCC_ATS_ISO_BCD_,         VS_FALSE,       16},
//        {0,             _NCCC_ATS_ISO_BCD_,         VS_FALSE,       0},  /* 最後一組一定要放 0 */
};
MY_FIELD_TYPE_TABLE myFieldTable[] =
{
    { 0,  VS_FALSE, 0, EMPTY },
    { 1,  VS_FALSE, 0, EMPTY },
    { 2,  VS_FALSE, 0, EMPTY },
    { 3,  VS_FALSE, 3, EMPTY },
    { 4,  VS_FALSE, 6, EMPTY },
    { 5,  VS_FALSE, 0, EMPTY },
    { 6,  VS_FALSE, 0, EMPTY },
    { 7,  VS_FALSE, 0, EMPTY },
    { 8,  VS_FALSE, 0, EMPTY },
    { 9,  VS_FALSE, 0, EMPTY },
    {10,  VS_FALSE, 0, EMPTY },
    {11,  VS_FALSE, 3, EMPTY },
    {12,  VS_FALSE, 3, EMPTY },
    {13,  VS_FALSE, 2, EMPTY },
    {14,  VS_FALSE, 0, EMPTY },
    {15,  VS_FALSE, 0, EMPTY },
    {16,  VS_FALSE, 0, EMPTY },
    {17,  VS_FALSE, 0, EMPTY },
    {18,  VS_FALSE, 0, EMPTY },
    {19,  VS_FALSE, 0, EMPTY },
    {20,  VS_FALSE, 0, EMPTY },
    {21,  VS_FALSE, 0, EMPTY },
    {22,  VS_FALSE, 0, EMPTY },
    {23,  VS_FALSE, 0, EMPTY },
    {24,  VS_FALSE, 2, EMPTY },
    {25,  VS_FALSE, 0, EMPTY },
    {26,  VS_FALSE, 0, EMPTY },
    {27,  VS_FALSE, 0, EMPTY },
    {28,  VS_FALSE, 0, EMPTY },
    {29,  VS_FALSE, 0, EMPTY },
    {30,  VS_FALSE, 0, EMPTY },
    {31,  VS_FALSE, 0, EMPTY },

    {32,  VS_FALSE, 2, LLVAR },
    {33,  VS_FALSE, 0, EMPTY },
    {34,  VS_FALSE, 0, EMPTY },
    {35,  VS_FALSE, 0, LLVAR },
    {36,  VS_FALSE, 0, EMPTY },
    {37,  VS_TRUE, 12, EMPTY },
    {38,  VS_TRUE,  6, EMPTY },
    {39,  VS_TRUE,  2, EMPTY },
    {40,  VS_FALSE, 0, EMPTY },
    {41,  VS_TRUE,  8, EMPTY },
    {42,  VS_FALSE, 0, EMPTY },
    {43,  VS_FALSE, 0, EMPTY },
    {44,  VS_FALSE, 0, EMPTY },
    {45,  VS_FALSE, 0, EMPTY },
    {46,  VS_FALSE, 0, EMPTY },
    {47,  VS_FALSE, 0, EMPTY },

    {48,  VS_FALSE, 0, LLLVAR },
    {49,  VS_FALSE, 0, LLLVAR },
    {50,  VS_FALSE, 0, LLLVAR },
    {51,  VS_FALSE, 0, LLLVAR },
    {52,  VS_FALSE, 0, LLLVAR },
    {53,  VS_FALSE, 0, LLLVAR },
    {54,  VS_FALSE, 0, LLLVAR },
    {55,  VS_FALSE, 0, LLLVAR },
    {56,  VS_FALSE, 0, LLLVAR },
    {57,  VS_FALSE, 0, LLLVAR },
    {58,  VS_FALSE, 0, LLLVAR },
    {59,  VS_TRUE,  0, LLLVAR },
    {60,  VS_FALSE, 0, LLLVAR },
    {61,  VS_FALSE, 0, LLLVAR },
    {62,  VS_FALSE, 0, LLLVAR },
    {63,  VS_FALSE, 0, LLLVAR },

    {64,  VS_FALSE, 0, EMPTY },
};


void myPrintData(char *fieldName,int len,char *data)
{
    printf("[%6s] [%3d] [%s]\n",fieldName,len,data);
}
/*
 Describe        :根據isDispAscii旗標判斷 傳入的byte顯示為ascii or byte
 */
void myDataToString(BYTE* inBuffer,int* idx,int len ,char *outBuffer,int isDispAscii)
{
    memset(&outBuffer[0],0x00,strlen(outBuffer));
//    printf("idx is %d , len is %d\n",*idx,len);
    char strData[2+1];
    int i ;
    if(isDispAscii)
    {
        memcpy(&outBuffer[0],&inBuffer[*idx],len);
        outBuffer[len] = '\0';
    }
    else
    {
        for(i=0;i<len;i++)
        {
            memset(strData,0x00,3);
            sprintf(strData,"%02X",inBuffer[*idx+i]);
            strcat(outBuffer,strData);
        }       
    }
    *idx = *idx + len;
}
int myUnPackData(BYTE *rawDataBuffer ,int inReceiveSize)
{
    int msgLength = (rawDataBuffer[0]<<8) | rawDataBuffer[1];
    int  indexTable[64];//類似hashtable的key
    memset(&indexTable[0],0x00,64);
    int szField = 0;//indexTable length
    int idxField =0;//indexTable index
    int filedCnt = 1; //計算當前 bitmap 上 bit所代表欄位
    char strDataBuffer[100+1];//儲存當前欄位data
    memset(&strDataBuffer[0],0x00,100+1);
    int inCnt = 2;//received buffer index
    int i,j;
    char subTagName[100];
    int subTagLen = 0;
    
    if(msgLength == (inReceiveSize-2) )
    {   
        printf("\n\n Receive Length is %d (0x%02X 0x%02X) , Buffer is : \n",msgLength,rawDataBuffer[0],rawDataBuffer[1]);
        for(i=2;i<inReceiveSize;i++)
        {
            printf("0x%02x\t",rawDataBuffer[i]);
        }
        printf("\n\n");
        memset(&strDataBuffer[0],0x00,100+1);
        //Message Length
        sprintf(strDataBuffer,"%d",msgLength);
        myPrintData("Len",msgLength,strDataBuffer);
        
        //Transport Protocol Data Unit (TPDU)
        myDataToString(rawDataBuffer,&inCnt,_NCCC_ATS_TPDU_SIZE_,strDataBuffer,VS_FALSE);
        myPrintData("TPDU",_NCCC_ATS_TPDU_SIZE_,strDataBuffer);
        
        //Message Type Identifier(MTI)
        myDataToString(rawDataBuffer,&inCnt,_NCCC_ATS_MTI_SIZE_,strDataBuffer,VS_FALSE);
        myPrintData("MTI",_NCCC_ATS_MTI_SIZE_,strDataBuffer);
        
        //Primary Bit Map
        for(i=0;i<_NCCC_ATS_BIT_MAP_SIZE_;i++)
        {
            for(j=7;j>=0;j--)
            {
                if(_NCCC_ATS_CHECK_BIT_MAP_(rawDataBuffer[inCnt+i],j))
                {
//                    printf("filedCnt = %d\n",filedCnt);
                    indexTable[idxField++] = filedCnt;                    
                }
                filedCnt++;
            }
        }
        myDataToString(rawDataBuffer,&inCnt,8,strDataBuffer,VS_FALSE);
        myPrintData("Bitmap",8,strDataBuffer); 
        
        char strFieldName[4+1];
        szField = idxField;
        for(i=0;i<szField;i++)
        {
            idxField = indexTable[i];
            if(myFieldTable[idxField].inFormat == EMPTY)
            {
                myDataToString(rawDataBuffer,&inCnt,myFieldTable[idxField].inFieldLen,strDataBuffer,myFieldTable[idxField].isDispAscii);
                
                memset(strFieldName,0x00,5);
                sprintf(strFieldName,"F_%02d",idxField);

                myPrintData(strFieldName,myFieldTable[idxField].inFieldLen,strDataBuffer); 
            }
            else if(myFieldTable[idxField].inFormat == LLVAR)
            {
                printf("---------F_%02d return length is 0x%02X\n",idxField,rawDataBuffer[inCnt++]);
                
                myDataToString(rawDataBuffer,&inCnt,myFieldTable[idxField].inFieldLen,strDataBuffer,myFieldTable[idxField].isDispAscii);
                
                memset(strFieldName,0x00,5);
                sprintf(strFieldName,"F_%02d",idxField);

                myPrintData(strFieldName,myFieldTable[idxField].inFieldLen,strDataBuffer); 
            }
            else // LLLVAR
            {
                memset(strDataBuffer,0x00,strlen(strDataBuffer));
                printf("---------F_%02d return total length is 0x%02X 0x%02X\n",idxField,rawDataBuffer[inCnt],rawDataBuffer[inCnt+1]);
                inFunc_BCD_to_ASCII(strDataBuffer, &rawDataBuffer[inCnt], 2);
                inCnt += 2;
                printf("[  F_%2d] [%3d]\n",idxField,atoi(strDataBuffer));
                switch(idxField)
                {
                    case 48:
                    {
                        break;
                    }
                    case 54:
                    {
                        break;
                    }
                    case 55:
                    {
                        break;
                    }
                     case 57:
                    {
                        break;
                    }
                    case 58:
                    {
                        break;
                    }
                    case 59:
                    {
                       int fieldTotalLen = atoi(strDataBuffer);
                        //printf("fieldTotalLen is %d\n",fieldTotalLen);
                        while(fieldTotalLen > 0)
                        {
        //                    printf("fieldTotalLen is %d\n",fieldTotalLen);
        //                    printf("-------tag name is %c%c\n",rawDataBuffer[inCnt],rawDataBuffer[inCnt+1]);
                            myDataToString(rawDataBuffer,&inCnt,2,subTagName,myFieldTable[idxField].isDispAscii);
                            memset(&strDataBuffer[0],0x00,strlen(strDataBuffer));
        //                    printf("-------tag length is 0x%02X 0x%02x\n",rawDataBuffer[inCnt],rawDataBuffer[inCnt+1]);
                            inFunc_BCD_to_ASCII(strDataBuffer, &rawDataBuffer[inCnt], 2);
                            inCnt +=2;
        //                    printf("===============length is %s\n",strDataBuffer);
                            subTagLen = atoi(strDataBuffer);
        //                    printf("-------tag length is %d\n",subTagLen);
                            myDataToString(rawDataBuffer,&inCnt,subTagLen,strDataBuffer,myFieldTable[idxField].isDispAscii);
                            myPrintData(subTagName,subTagLen,strDataBuffer);

                            fieldTotalLen -= (2 + subTagLen + 2);//sub tag name 和sub field length 目前為2 
                        }
                        break;
                    }
                    case 60:
                    {
                        break;
                    }
                    case 61:
                    {
                        break;
                    }
                    case 62:
                    {
                        break;
                    }
                    case 63:
                    {
                        break;
                    }
                }
            }
        }
    }

}
int myPackData(BYTE *uszPackBuf)
{
    int FieldSz = sizeof(srNCCC_ATS_ISOFieldType)/sizeof(srNCCC_ATS_ISOFieldType[0]);
    int i;
    int inCnt = 0;
    //Message Length
    uszPackBuf[inCnt++] = 0X00;
    uszPackBuf[inCnt++] = 0X00;
    //Transport Protocol Data Unit (TPDU)
    uszPackBuf[inCnt++] = 0X60;
    uszPackBuf[inCnt++] = 0X02;
    uszPackBuf[inCnt++] = 0X96;
    uszPackBuf[inCnt++] = 0X00;
    uszPackBuf[inCnt++] = 0X01;
    //Message Type Identifier(MTI) 
    uszPackBuf[inCnt++] = 0X02;
    uszPackBuf[inCnt++] = 0X00;
    //Primary Bit Map 
    memset(&uszPackBuf[inCnt],0x00,8);//9~16
    inCnt += 8;
    
    for(i=0;i<FieldSz;i++)
    {            
        int Length = srNCCC_ATS_ISOFieldType[i].inFieldLen / 2;
        unsigned char* uszBCD = malloc(sizeof(unsigned char)*(Length+1));
        memset(uszBCD, 0x00, Length+1);
//        printf("field num is %d\n",srNCCC_ATS_ISOFieldType[i].inFieldNum);
        /*===== set a bit in the bitmap field =====*/
        /*參考 inNCCC_ATS_BitMapCheck */
        int offset = (srNCCC_ATS_ISOFieldType[i].inFieldNum -1) / 8;
        uszPackBuf[9 + offset] |= (1U << 8-(srNCCC_ATS_ISOFieldType[i].inFieldNum - 8*offset));
        
        switch(srNCCC_ATS_ISOFieldType[i].inFieldNum)
        {
            case 3:
            {
                //set packbuff is 0x00,0x00,0x00
                memset(&uszPackBuf[inCnt],0x00, _NCCC_ATS_PCODE_SIZE_);
                inCnt +=_NCCC_ATS_PCODE_SIZE_;
                break;
            }
            case 4:
            {
                inFunc_ASCII_to_BCD(&uszBCD[0],"000000000100",Length );
                memcpy((char *) &uszPackBuf[inCnt], (char *) &uszBCD[0], Length);
                inCnt += Length;
                break;
            }
            case 11:
            {
                inFunc_ASCII_to_BCD(&uszBCD[0],"000002",Length );
                memcpy((char *) &uszPackBuf[inCnt], (char *) &uszBCD[0], Length);
                inCnt += Length;
                break;
            }
            case 12:
            {
                inFunc_ASCII_to_BCD(&uszBCD[0],"073012",Length );
                memcpy((char*)&uszPackBuf[inCnt], (char *)&uszBCD[0],Length);
                inCnt +=3;
                break;
            }
            case 13:
            {
                inFunc_ASCII_to_BCD(&uszBCD[0],"0107",Length );
                memcpy((char*)&uszPackBuf[inCnt], (char *)&uszBCD[0],Length);
                inCnt +=3;
                break;
            }
            case 22:
            {
                inFunc_ASCII_to_BCD(&uszBCD[0],"5071",Length );
                memcpy((char *) &uszPackBuf[inCnt], (char *) &uszBCD[0], Length);
                inCnt += Length;
                break;
            }
            case 24:
            {
                inFunc_ASCII_to_BCD(&uszBCD[0],"0296",Length );
                memcpy((char *) &uszPackBuf[inCnt], (char *) &uszBCD[0], Length);
                inCnt += Length;
                break;     
            }
            case 25:
            {     
                uszPackBuf[inCnt++] = 0X00;
                break;            
            }
            case 32:
            {
               /*
                    補充說明： 
                    1. 本欄位請依收單銀行之金融機構代碼上傳(3碼)。 
                    2. 範例： 
                    金融代碼=956 
                    Length (BCD)+ Data(BCD)= 0x02 0x09 0x56 
                */
                inFunc_ASCII_to_BCD(&uszPackBuf[inCnt], "020956", 3);
                inCnt += 3;
                break;        
            }
            case 35:
            {
                uszPackBuf[inCnt++] = 0x021;
                inFunc_ASCII_to_BCD(&uszPackBuf[inCnt], "4761739001010119D15F", 11);
                inCnt += 11;
                break;
            }
            case 37:
            {
                memcpy((char *) &uszPackBuf[inCnt], "99202001003 ", 12);
                inCnt += 12;
                break;
            }
            case 38:
            {
                memcpy((char *) &uszPackBuf[inCnt], "abcd  ", 6);
                inCnt += 6; 
                break;
            }
            case 39:
            {
                break;
            }
            case 41:
            {
                memcpy((char *) &uszPackBuf[inCnt], "13992020", 8);
                inCnt += 8;
                break;
            }
            case 42:
            {
                char	szTemplate[16 + 1] = "1314006807";
                inFunc_PAD_ASCII(szTemplate, szTemplate, ' ', 15, _PADDING_RIGHT_);
                memcpy((char *) &uszPackBuf[inCnt], szTemplate, strlen(szTemplate));
                inCnt += strlen(szTemplate);
                break;
            }
            case 48:
            {
                /*
                    範例： 
                   Length = SubField Length(1 Bytes) + Store ID(6 Bytes) + User 
                   Define(6 Bytes) + Other(6 Bytes) = 19 Bytes (BCD Code = 0x00 0x19) 
                   SubField Length = Store ID(6 Bytes) + User Define(6 Bytes) + Other(6 
                   Bytes) = 18 Bytes (Binary Code = 0x12) 
                   Store ID =123456 
                   User Define = 88888 
                   Other = No Data
                 */
                inFunc_ASCII_to_BCD(&uszPackBuf[inCnt], "001912", 3); //Length + SubField Length
                inCnt += 3;
                memcpy(&uszPackBuf[inCnt],"123456888888",12);//Store ID + User Define
                inCnt += 12;
                memset(&uszPackBuf[inCnt],' ',6);//Other = No Data ，Set  0x20 *6
                inCnt += 6;
//                char	szTemplate[40 + 1];
//                uszPackBuf[inCnt ++] = 0x00;
//		uszPackBuf[inCnt ++] = 0x19;
//		uszPackBuf[inCnt ++] = 0x12;
//
//		memset(szTemplate, 0x00, sizeof(szTemplate));
//		for (i = 0; i < 18; i ++)
//		{
//			if ((szTemplate[i] < 0x20) || (szTemplate[i] > 0x7e))
//			{
//				szTemplate[i] = 0x20;
//			}
//		}
//
//		memcpy((char *)&uszPackBuf[inCnt], szTemplate, 18);
//		inCnt += 18;
                break;
            }
            case 52:
            {
                memcpy(&uszPackBuf[inCnt],"12345678",8);
                inCnt += 8;
                break;
            }
            case 54:
            {
                uszPackBuf[inCnt++] = 0x00;
                uszPackBuf[inCnt++] = 0x12;
                memcpy(&uszPackBuf[inCnt],"000001234500",12);
                inCnt +=12;
                break;
            }
            case 55:
            {
                int lastInCnt =  inCnt ;
                //FIELD_55 DATA LENGTH
                uszPackBuf[inCnt++] = 0x00;
                uszPackBuf[inCnt++] = 0x00;

                //Sub-field = TAG + TAG DATA LENGTH + TAG DATA
                //5F2A
                uszPackBuf[inCnt++] = 0x5F;
                uszPackBuf[inCnt++] = 0x2A;
                
                uszPackBuf[inCnt++] = 0X02;     
                
                uszPackBuf[inCnt++] = 0X09;
                uszPackBuf[inCnt++] = 0x01;
                //5F34
                uszPackBuf[inCnt++] = 0x5F;
                uszPackBuf[inCnt++] = 0X34;
                
                uszPackBuf[inCnt++] = 0X01; 
                
                uszPackBuf[inCnt++] = 0X00;             
                //82
                uszPackBuf[inCnt++] = 0x82;
                
                uszPackBuf[inCnt++] = 0X02;     
                
                uszPackBuf[inCnt++] = 0X20;
                uszPackBuf[inCnt++] = 0x00;
                //95
                uszPackBuf[inCnt++] = 0x82;
                
                uszPackBuf[inCnt++] = 0X07;
                
                uszPackBuf[inCnt++] = 0XA0;
                uszPackBuf[inCnt++] = 0x00;
                uszPackBuf[inCnt++] = 0x00;
                uszPackBuf[inCnt++] = 0x00;
                uszPackBuf[inCnt++] = 0X03;
                uszPackBuf[inCnt++] = 0X10;
                uszPackBuf[inCnt++] = 0X10;
                //9A
                uszPackBuf[inCnt++] = 0x9A;
                
                uszPackBuf[inCnt++] = 0X03;     
                
                uszPackBuf[inCnt++] = 0X26;
                uszPackBuf[inCnt++] = 0X01;
                uszPackBuf[inCnt++] = 0X05;
                //9F02
                uszPackBuf[inCnt++] = 0x9F;
                uszPackBuf[inCnt++] = 0x02;
                
                uszPackBuf[inCnt++] = 0X06;     
                
                uszPackBuf[inCnt++] = 0X00;
                uszPackBuf[inCnt++] = 0X00;
                uszPackBuf[inCnt++] = 0X00;
                uszPackBuf[inCnt++] = 0X00;
                uszPackBuf[inCnt++] = 0X01;
                uszPackBuf[inCnt++] = 0X00;               
                //9F10
                uszPackBuf[inCnt++] = 0x9F;
                uszPackBuf[inCnt++] = 0x10;
                
                uszPackBuf[inCnt++] = 0X07;     
                
                uszPackBuf[inCnt++] = 0X06;
                uszPackBuf[inCnt++] = 0X01;
                uszPackBuf[inCnt++] = 0X11;
                uszPackBuf[inCnt++] = 0X03;
                uszPackBuf[inCnt++] = 0XA0;
                uszPackBuf[inCnt++] = 0X00;
                uszPackBuf[inCnt++] = 0X00;
                
                char totalLength[4+1];
                sprintf(totalLength,"%04d",inCnt - lastInCnt-2);
//                printf("last count is %d , curr count is %d,total length is %s\n",lastInCnt,inCnt,totalLength);
                inFunc_ASCII_to_BCD(&uszPackBuf[lastInCnt], totalLength, 2);
                
                break;
            }
            case 57:
            {
                uszPackBuf[inCnt++] = 0X00;
                uszPackBuf[inCnt++] = 0X12;
                //BITMAP
                uszPackBuf[inCnt++] = 0X00; 
                uszPackBuf[inCnt++] = 0X00; 
                uszPackBuf[inCnt++] = 0X00; 
                uszPackBuf[inCnt++] = 0X00;

                uszPackBuf[inCnt++] = 0X20; 
                uszPackBuf[inCnt++] = 0X00; 
                uszPackBuf[inCnt++] = 0X00;
                uszPackBuf[inCnt++] = 0X00;         
                //Field_35檢查碼
                uszPackBuf[inCnt++] = 0X5C; 
                uszPackBuf[inCnt++] = 0X5E; 
                uszPackBuf[inCnt++] = 0X0E;
                uszPackBuf[inCnt++] = 0X94;
                

                break;
            }
            case 58:
            {
                uszPackBuf[inCnt++] = 0X00; 
                uszPackBuf[inCnt++] = 0X11; 
                memcpy(&uszPackBuf[inCnt],"S1" , 2);
                inCnt += 2;
                uszPackBuf[inCnt++] = 0X08;
                memcpy(&uszPackBuf[inCnt],"00000000" , 8);
                inCnt += 8;               
                break;
            }
            case 59:
            {   
                int lastInCnt = inCnt;
                uszPackBuf[inCnt++] = 0X00;
                uszPackBuf[inCnt++] = 0X00;
                //Tag + Length + Data
                memcpy(&uszPackBuf[inCnt],"N1" , 2);
                inCnt += 2;
                
                uszPackBuf[inCnt++] = 0X00;
                uszPackBuf[inCnt++] = 0X02;
                
                memcpy(&uszPackBuf[inCnt],"10" , 2);
                inCnt += 2;
                //N2
                memcpy(&uszPackBuf[inCnt],"N2" , 2);
                inCnt += 2;
                
                uszPackBuf[inCnt++] = 0X00;
                uszPackBuf[inCnt++] = 0X02;
                
                memcpy(&uszPackBuf[inCnt],"ST" , 2);
                inCnt += 2;
                
                //NF
                memcpy(&uszPackBuf[inCnt],"NF" , 2);
                inCnt += 2;
                
                uszPackBuf[inCnt++] = 0X00;
                uszPackBuf[inCnt++] = 0X04;
                
                memcpy(&uszPackBuf[inCnt],"ATS " , 4);
                inCnt += 4;
                //L1
                memcpy(&uszPackBuf[inCnt],"LI" , 2);
                inCnt += 2;
                
                uszPackBuf[inCnt++] = 0X00;
                uszPackBuf[inCnt++] = 0X23;
                
                memcpy(&uszPackBuf[inCnt],"01399202020260105140942" , 23);               
                inCnt += 23;
                
                char totalLength[4+1];
                sprintf(totalLength,"%04d",inCnt - lastInCnt-2);
//                printf("last count is %d , curr count is %d,total length is %s\n",lastInCnt,inCnt,totalLength);
                inFunc_ASCII_to_BCD(&uszPackBuf[lastInCnt], totalLength, 2);
                break;
            }
            case 60:
            {   
                uszPackBuf[inCnt++] = 0X00;
                uszPackBuf[inCnt++] = 0X06;
                memcpy(&uszPackBuf[inCnt],"654321",6);
                inCnt +=6;
                break;
            }
            case 62:
            {   
                uszPackBuf[inCnt++] = 0X00;
                uszPackBuf[inCnt++] = 0X06;
                memcpy(&uszPackBuf[inCnt],"123456",6);
                inCnt +=6;
                break;
            }
            case 63:
            {
                uszPackBuf[inCnt++] = 0X00;
                uszPackBuf[inCnt++] = 0X15;
                memcpy(&uszPackBuf[inCnt],"123",3);
                inCnt += 3;
                memcpy(&uszPackBuf[inCnt],"ABCDEFGHIJKL",12);
                inCnt += 12;
                break;
            }
            case 64:
            {   
                break;
            }
            default:
                break; 
        }
        free(uszBCD);
    } 
   
    //計算整個buffer總長度，轉成2Byte塞到buffer[0]/buffer[1]
    printf("inCnt  is %d\n",inCnt);
    
    uszPackBuf[0] = (inCnt - 2) / 256;
    uszPackBuf[1] = (inCnt - 2) % 256;  
    printf("uszPackBuf[0] is 0x%02x\n",uszPackBuf[0]);
    printf("uszPackBuf[1] is 0x%02x\n",uszPackBuf[1]);
    
    return inCnt;
}