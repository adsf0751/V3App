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
        {38,            _NCCC_ATS_ISO_ASC_,         VS_FALSE,       6},
//        {39,            _NCCC_ATS_ISO_ASC_,         VS_FALSE,       2},
        {41,            _NCCC_ATS_ISO_ASC_,         VS_FALSE,       16},
        {42,            _NCCC_ATS_ISO_ASC_,         VS_FALSE,       30},
        {48,            _NCCC_ATS_ISO_BYTE_3_,      VS_TRUE,        42},
        {52,            _NCCC_ATS_ISO_BCD_,         VS_FALSE,       16},
//        {54,            _NCCC_ATS_ISO_BYTE_3_,      VS_TRUE,        0},
        {55,            _NCCC_ATS_ISO_BYTE_3_,      VS_FALSE,       280},
        {57,            _NCCC_ATS_ISO_BYTE_3_,      VS_FALSE,       24},
        {58,            _NCCC_ATS_ISO_BYTE_3_,      VS_TRUE,        0},
        {59,            _NCCC_ATS_ISO_BYTE_3_,      VS_TRUE,        292},
//        {60,            _NCCC_ATS_ISO_BYTE_3_,      VS_TRUE,        12},
        {62,            _NCCC_ATS_ISO_BYTE_3_,      VS_TRUE,        12},
        {63,            _NCCC_ATS_ISO_BYTE_3_,      VS_TRUE,        0},
//        {64,            _NCCC_ATS_ISO_BCD_,         VS_FALSE,       16},
//        {0,             _NCCC_ATS_ISO_BCD_,         VS_FALSE,       0},  /* 最後一組一定要放 0 */
};
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
    //3 4 11 22 24 25 32  37 38 41 42 48 55 
    uszPackBuf[inCnt++] = 0X30; // 1~ 8
    uszPackBuf[inCnt++] = 0X20; // 9~16
    uszPackBuf[inCnt++] = 0X05; //17~24
    uszPackBuf[inCnt++] = 0X81; //25~32
    uszPackBuf[inCnt++] = 0X2C; //33~40
    uszPackBuf[inCnt++] = 0XC1; //41~48
    uszPackBuf[inCnt++] = 0X12; //49~56
    uszPackBuf[inCnt++] = 0XF2; //57~64
    
    for(i=0;i<FieldSz;i++)
    {            
        int Length = srNCCC_ATS_ISOFieldType[i].inFieldLen / 2;
        unsigned char* uszBCD = malloc(sizeof(unsigned char)*(Length+1));
        memset(uszBCD, 0x00, Length+1);
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
                memcpy((char*)&uszPackBuf[inCnt],&uszBCD[0],Length);
                inCnt +=3;
                break;
            }
            case 13:
            {
                inFunc_ASCII_to_BCD(&uszBCD[0],"0107",Length );
                memcpy((char*)&uszPackBuf[inCnt],&uszBCD[0],Length);
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
                memcpy(&uszPackBuf[inCnt],"1234567812345678",8);
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
    unsigned int Len = inCnt -2 ;//扣掉buffer[0]/buffer[1] 
    printf("inCnt  is %d\n",inCnt);
    //Len 是非負整數前提下
    //除2^n     等價於Len >>  n
    //求2^n餘數 等價於Len & (2^n -1)
//    uszPackBuf[0] = (Len/16);
//    uszPackBuf[1] = Len%16; 
    uszPackBuf[0] = Len >> 4;
    uszPackBuf[1] = Len & (16-1); 
//    printf("uszPackBuf[0] is %x\n",uszPackBuf[0]);
//    printf("uszPackBuf[1] is %x\n",uszPackBuf[1]); 
    return inCnt;
}