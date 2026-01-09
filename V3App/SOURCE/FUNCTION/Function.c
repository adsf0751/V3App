#include "../../Trans.h" // FOR VS_ERROR/VS_SUCCESS
#include <stdio.h> // FOR NULL
#include "Function.h" //FOR _PADDING_RIGHT_ /_PADDING_LEFT_
#include <string.h>
#include <stdlib.h>
/*
Function	:inFunc_ASCII_to_BCD
Date&Time	:2015/7/13 下午 2:02
Describe        :ASCII轉BCD，inLength指要轉換成的byte數(ASCII字串的1/2長度)
		注意:BCD的空間長度至少要為ASCII的1/2倍，否則會溢位。(2byte ascii可轉換成1byte bcd)
*/
int inFunc_ASCII_to_BCD(unsigned char *uszBCD, char *szASCII, int inLength)
{
        unsigned char uszTemplate = 0;
        int	i;

        /* 防呆 inLength須大於零 */
        if (inLength <= 0)
                return (VS_ERROR);

        for (i = 0; i < inLength ; i ++)
        {
                /* 字元做運算 */
                switch (*szASCII)
                {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                                uszTemplate = *szASCII - '0';
                                break;
                        case 'A':
                        case 'B':
                        case 'C':
                        case 'D':
                        case 'E':
                        case 'F':
                                uszTemplate = *szASCII - 'A' + 10;
                                break;
                        case 'a':
                        case 'b':
                        case 'c':
                        case 'd':
                        case 'e':
                        case 'f':
                                uszTemplate = *szASCII - 'a' + 10;
                                break;
                        default:
                                break;
                }

                *uszBCD = (unsigned char) (uszTemplate << 4); /* 向左移動四位元 */
                szASCII ++;

                switch (*szASCII)
                {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                                uszTemplate = *szASCII - '0';
                                break;
                        case 'A':
                        case 'B':
                        case 'C':
                        case 'D':
                        case 'E':
                        case 'F':
                                uszTemplate = *szASCII - 'A' + 10;
                                break;
                        case 'a':
                        case 'b':
                        case 'c':
                        case 'd':
                        case 'e':
                        case 'f':
                                uszTemplate = *szASCII - 'a' + 10;
                                break;
                        default:
                                break;
                }

                szASCII ++;
                *uszBCD = *uszBCD | uszTemplate;
                uszBCD ++;
        }

        return (VS_SUCCESS);
}
/*
Function	:inFunc_BCD_to_ASCII
Date&Time	:2015/7/13 下午 2:02
Describe	:BCD轉ASCII 注意:ASCII的空間長度要為BCD的兩倍，否則會溢位。
*/
int inFunc_BCD_to_ASCII(char *szASCII, unsigned char *uszBCD, int inLength)
{
	unsigned char uszTemplate, uszNibble;
        int i;

	for(i = 0; i < inLength; ++i)
	{
		uszTemplate = *uszBCD++;
                uszNibble = (uszTemplate / 16);         /* 第一個字  */

                /* 16進位A = 10進位的10，做運算 */
                if (uszNibble < 10)
                {
                    *szASCII = (char)('0' + uszNibble);
                    szASCII++;
		}
                else                                    /* A~F */
                {
                    *szASCII = (char)('A' + (uszNibble - 10));
                    szASCII++;
                }

                uszNibble = (uszTemplate % 16);         /* 第二個字  */

                if (uszNibble < 10)
                {
                        *szASCII = (char)('0' + uszNibble);
                        szASCII++;
		}
                else
                {
                        *szASCII = (char)('A' + (uszNibble - 10));
                        szASCII++;
                }
	}

	/* 不要補0比較好用 */
//	*szASCII = 0;

	return (VS_SUCCESS);
}
int inFunc_ShellCommand_System(char *szCommand) {
    int inRetVal;
    char szDebugMsg[100 + 1];
    if (szCommand == NULL) {
        return (VS_ERROR);
    }
    inRetVal = system(szCommand);
    /* 在C 程式裡其中 ret 值要除以256才會得到與shell 傳回相符的值 */
    inRetVal >>= 8;

    if (inRetVal != 0) {
        return (VS_ERROR);
    }
    return (VS_SUCCESS);
}
int inFunc_ls(char* szParameter1, char* szSource) {
    int inRetVal = VS_SUCCESS;
    char szDebugMsg[100 + 1] = {0};
    char szCommand[100 + 1] = {0}; /* Shell Command*/

    /* 組命令 */
    if (strlen(szSource) != 0) {
        memset(szCommand, 0x00, sizeof (szCommand));
        sprintf(szCommand, "ls ");

        if (strlen(szParameter1) != 0) {
            strcat(szCommand, szParameter1);
            strcat(szCommand, " ");
        }

        if (strlen(szSource) != 0) {
            strcat(szCommand, szSource);
        }
    }/* 沒有來源檔 */
    else {
        return (VS_ERROR);
    }
    //printf("szCommand is %s\n",szCommand);
    /* 執行命令 */
    inRetVal = inFunc_ShellCommand_System(szCommand);
    if (inRetVal != VS_SUCCESS) {
        return (inRetVal);
    }

    return (VS_SUCCESS);
}

/*
Function	:inFunc_PAD_ASCII
Date&Time	:2015/6/24 下午 4:14
Describe	:靠左靠右補字或空白
 *注意		:若靠右，且pad 0x00會抓不到字串長度，建議不要包含中文字串，中文字串會算三個字元
*/
int inFunc_PAD_ASCII(char *szStr_out, char *szStr_in, char szPad_char, int inPad_size, int inAlign)
{
        int inPADlen = 0;
        char szTemplate[256 + 1];

        /* 第一步:計算要補多少字元 */
        inPADlen = inPad_size - strlen(szStr_in);

        /* 第二步:靠左靠右 */
	if (inPADlen > 0)
	{
                /* 將szTemplate初始化成要補的字元 */
                memset(szTemplate, szPad_char, sizeof(szTemplate));

        	if (inAlign == _PADDING_LEFT_)
                        memcpy(&szTemplate[inPADlen], &szStr_in[0], inPad_size); /* 字靠右 */
                else if (inAlign == _PADDING_RIGHT_)
                        memcpy(&szTemplate[0], &szStr_in[0], strlen(szStr_in)); /* 字靠左 */

                /* 補結束字元 & 傳回szStr_out */
                szTemplate[inPad_size] = '\0';
                memset(szStr_out, 0x00, sizeof(szStr_out));
                strcpy(szStr_out, szTemplate);
	}
        else if (inPADlen == 0)
        {
                /* inPADlen = 0 的狀況，szStr_in將字串給szStr_out */
                memset(szTemplate, 0x00, sizeof(szTemplate));
                memcpy(&szTemplate[0], &szStr_in[0], strlen(szStr_in));
                memset(szStr_out, 0x00, sizeof(szStr_out));
                strcpy(szStr_out, szTemplate);
        }
        else
        {
                /* inPADlen < 0 清空szStr_out Return Error */
                memset(szStr_out, 0x00, sizeof(szStr_out));	
                return(VS_ERROR);
        }

	return (VS_SUCCESS);
}