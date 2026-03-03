#include <ctosapi.h>
#include <stdio.h>
#include <string.h>
#include "../../Trans.h"
#include "Display.h"
#include "../FUNCTION/File.h"
#include "../INCLUDE/Define_1.h"
#include "../FUNCTION/Function.h"
int		ginSetFont = -1;			/* 判斷目前SetFont */

/*
Function	:inDISP_ClearAll
Date&Time	:2015/6/7 下午 8:20
Describe	:顯示參數初始化
*/
int inDISP_Initial(void)
{
        /* 步驟一 設定LCD顯示畫面大小格式 */
        CTOS_LCDSelectMode(d_LCD_GRAPHIC_HIGH_RES_MODE);

        /* 步驟二 設定LCD顯示畫面字型格式 */
	CTOS_LCDFontSelectMode(d_FONT_TTF_MODE);

        /* 步驟三 設定TTF字型及Style */
        inDISP_TTF_SetFont(_DISP_CHINESE_, _FONT_DISPLAY_REGULAR_);               /* 微軟正黑體 */
        /*非原本程式設定，因為在inDISP_EnglishFont_Color()會修改別的字型大小，所以這邊先設定一個原始大小*/
        CTOS_LCDTSelectFontSize(_CHINESE_FONT_8X22_);
        return (VS_SUCCESS);
}
/*
Function	:inDISP_Clear_Area
Date&Time	:2015/6/7 下午 6:41
Describe	:以8X16格式來指定位置清除螢幕
*/
 int inDISP_Clear_Area(int inXL, int inYL, int inXR, int inYR, int inFoneSize)
{
        int	i = 0;

//	if (ginHalfLCD == VS_TRUE)
//	{
//		if (inFoneSize == _ENGLISH_FONT_8X16_)
//			CTOS_LCDTSelectFontSize(_ENGLISH_FONT_8X16_HALF_);
//		else if (inFoneSize == _ENGLISH_FONT_8X22_)
//			CTOS_LCDTSelectFontSize(_ENGLISH_FONT_8X22_HALF_);
//	}
//	else
//	{
        	if (inFoneSize == _ENGLISH_FONT_8X16_)
        	        CTOS_LCDTSelectFontSize(_ENGLISH_FONT_8X16_);
        	else if (inFoneSize == _ENGLISH_FONT_8X22_)
                	CTOS_LCDTSelectFontSize(_ENGLISH_FONT_8X22_);
//	}

	for (i = inYL; i <= inYR; i++)
	{
                if ( i == inYL)
                        CTOS_LCDTGotoXY(inXL, i);
	else
                        CTOS_LCDTGotoXY(1, i);

		CTOS_LCDTClear2EOL();
	}

        return (VS_SUCCESS);
}
/*
Function        :inKBD_Key_IsPressed
Date&Time       :2017/12/18 下午 5:05
Describe        :
*/
int inKBD_Key_IsPressed(void)
{
        VS_BOOL		fKeyPressed = VS_FALSE;

        /* 判斷按鍵觸發，若觸發fKeyPressed為d_TRUE */
        CTOS_KBDInKey(&fKeyPressed);
	
	if (fKeyPressed == d_TRUE)
        {
		return (VS_SUCCESS);
	}
	else
	{
		return (VS_ERROR);
	}
}

unsigned char uszKBD_Key(void)
{
	int		inRetVal = VS_SUCCESS;
        unsigned char	uszKey = 0;

        /* 判斷按鍵觸發，若觸發fKeyPressed為d_TRUE */
        inRetVal = inKBD_Key_IsPressed();

        if (inRetVal == VS_SUCCESS)
        {
		uszKey = uszKBD_Key_Hit();
                if (uszKey != 0)
                {
                        return (uszKey);
                }
        }

        return uszKey;
}

/*
Function        :szKBD_Key_Hit
Date&Time       :2017/12/18 下午 4:50
Describe        :偵測壓過的key並從Buffer移除
*/
/* 檢查key buffer是否有按鍵，並回傳最後一個按鍵，
 * 如果key buffer為空，則帶入的參數被賦予值d_KBD_INVALID 
 */
unsigned char uszKBD_Key_Hit(void)
{
        unsigned char	uszKey = 0;

	if (CTOS_KBDHit(&uszKey) == d_OK)
	{
		if (uszKey == d_KBD_INVALID)
		{
			uszKey = 0;
			return (uszKey);
		}
		else	
		{
			return (uszKey);
		}
	}
	else
	{
		return (uszKey);
	}
}
/*
Function        :inDISP_Select_FontSize
Date&Time       :2017/10/25 下午 4:47
Describe        :
*/
int inDISP_Select_FontSize(int inFontSize, int inLanguage)
{
	unsigned short	usFontSize = 0;
	
	if (inDISP_Decide_FontSize(inFontSize, inLanguage, &usFontSize) != VS_SUCCESS)
	{
		return (VS_ERROR);
	}
	else
	{
		CTOS_LCDTSelectFontSize(usFontSize);
	}
	
	return (VS_SUCCESS);
}
/*
Function        :inDISP_Decide_FontSize
Date&Time       :2018/4/12 上午 10:57
Describe        :
*/
int inDISP_Decide_FontSize(int inFontSize, int inLanguage, unsigned short* usFontsize)
{
	/* 小螢幕直接塞值 */
//	if (ginHalfLCD == VS_TRUE)
//	{
//		if (inLanguage == _DISP_CHINESE_)
//		{
//			switch (inFontSize)
//			{
//				case _FONTSIZE_8X16_ :
//					*usFontsize = _CHINESE_FONT_8X16_HALF_;
//					break;
//				case _FONTSIZE_8X22_ :
//					*usFontsize = _CHINESE_FONT_8X22_HALF_;
//					break;
//				case _FONTSIZE_8X33_ :
//					*usFontsize = _CHINESE_FONT_8X33_HALF_;
//					break;
//                                case _FONTSIZE_8X34_ :
//                                        *usFontsize = _CHINESE_FONT_8X34_HALF_;
//                                        break;
//				case _FONTSIZE_8X44_ :
//					*usFontsize = _CHINESE_FONT_8X44_HALF_;
//					break;
//				case _FONTSIZE_12X19_ :
//					*usFontsize = _CHINESE_FONT_12X19_HALF_;
//					break;
//				case _FONTSIZE_16X16_ :
//					*usFontsize = _CHINESE_FONT_16X16_HALF_;
//					break;
//				case _FONTSIZE_16X22_ :
//					*usFontsize = _CHINESE_FONT_16X22_HALF_;
//					break;
//				case _FONTSIZE_16X33_ :
//					*usFontsize = _CHINESE_FONT_16X33_HALF_;
//					break;
//				case _FONTSIZE_16X44_ :
//					*usFontsize = _CHINESE_FONT_16X44_HALF_;
//					break;
//				case _FONTSIZE_24X22_ :
//					*usFontsize = _CHINESE_FONT_24X22_HALF_;
//					break;
//				case _FONTSIZE_24X33_ :
//					*usFontsize = _CHINESE_FONT_24X33_HALF_;
//					break;
//				case _FONTSIZE_32X22_ :
//					*usFontsize = _CHINESE_FONT_32X22_HALF_;
//					break;
//				default :
//					return (VS_ERROR);
//			}
//		}
//		else if (inLanguage == _DISP_ENGLISH_)
//		{
//			switch (inFontSize)
//			{
//				case _FONTSIZE_8X16_ :
//					*usFontsize = _ENGLISH_FONT_8X16_HALF_;
//					break;
//				case _FONTSIZE_8X22_ :
//					*usFontsize = _ENGLISH_FONT_8X22_HALF_;
//					break;
//				case _FONTSIZE_8X33_ :
//					*usFontsize = _ENGLISH_FONT_8X33_HALF_;
//					break;
//                                case _FONTSIZE_8X34_ :
//                                        *usFontsize = _ENGLISH_FONT_8X34_HALF_;
//                                        break;
//				case _FONTSIZE_8X44_ :
//					*usFontsize = _ENGLISH_FONT_8X44_HALF_;
//					break;
//				case _FONTSIZE_12X19_ :
//					*usFontsize = _ENGLISH_FONT_12X19_HALF_;
//					break;
//				case _FONTSIZE_16X16_ :
//					*usFontsize = _ENGLISH_FONT_16X16_HALF_;
//					break;
//				case _FONTSIZE_16X22_ :
//					*usFontsize = _ENGLISH_FONT_16X22_HALF_;
//					break;
//				case _FONTSIZE_16X33_ :
//					*usFontsize = _ENGLISH_FONT_16X33_HALF_;
//					break;
//				case _FONTSIZE_16X44_ :
//					*usFontsize = _ENGLISH_FONT_16X44_HALF_;
//					break;
//				case _FONTSIZE_24X22_ :
//					*usFontsize = _ENGLISH_FONT_24X22_HALF_;
//					break;
//				case _FONTSIZE_24X33_ :
//					*usFontsize = _ENGLISH_FONT_24X33_HALF_;
//					break;
//				case _FONTSIZE_32X22_ :
//					*usFontsize = _ENGLISH_FONT_32X22_HALF_;
//					break;
//				default :
//					return (VS_ERROR);
//			}
//		}
//		else
//		{
//			switch (inFontSize)
//			{
//				case _FONTSIZE_8X16_ :
//					*usFontsize = _CHINESE_FONT_8X16_HALF_;
//					break;
//				case _FONTSIZE_8X22_ :
//					*usFontsize = _CHINESE_FONT_8X22_HALF_;
//					break;
//				case _FONTSIZE_8X33_ :
//					*usFontsize = _CHINESE_FONT_8X33_HALF_;
//					break;
//                                case _FONTSIZE_8X34_ :
//                                        *usFontsize = _CHINESE_FONT_8X34_HALF_;
//                                        break;
//				case _FONTSIZE_8X44_ :
//					*usFontsize = _CHINESE_FONT_8X44_HALF_;
//					break;
//				case _FONTSIZE_12X19_ :
//					*usFontsize = _CHINESE_FONT_12X19_HALF_;
//					break;
//				case _FONTSIZE_16X16_ :
//					*usFontsize = _CHINESE_FONT_16X16_HALF_;
//					break;
//				case _FONTSIZE_16X22_ :
//					*usFontsize = _CHINESE_FONT_16X22_HALF_;
//					break;
//				case _FONTSIZE_16X33_ :
//					*usFontsize = _CHINESE_FONT_16X33_HALF_;
//					break;
//				case _FONTSIZE_16X44_ :
//					*usFontsize = _CHINESE_FONT_16X44_HALF_;
//					break;
//				case _FONTSIZE_24X22_ :
//					*usFontsize = _CHINESE_FONT_24X22_HALF_;
//					break;
//				case _FONTSIZE_24X33_ :
//					*usFontsize = _CHINESE_FONT_24X33_HALF_;
//					break;
//				case _FONTSIZE_32X22_ :
//					*usFontsize = _CHINESE_FONT_32X22_HALF_;
//					break;
//				default :
//					return (VS_ERROR);
//			}
//		}
//
//	}
//	else
//	{
		/* FULL螢幕用原本define */
		if (inLanguage == _DISP_CHINESE_)
		{
			switch (inFontSize)
			{
				case _FONTSIZE_8X16_ :
					*usFontsize = _CHINESE_FONT_8X16_;
					break;
				case _FONTSIZE_8X22_ :
					*usFontsize = _CHINESE_FONT_8X22_;
					break;
				case _FONTSIZE_8X33_ :
					*usFontsize = _CHINESE_FONT_8X33_;
					break;
                                case _FONTSIZE_8X34_ :
                                        *usFontsize = _CHINESE_FONT_8X34_;
                                        break;
				case _FONTSIZE_8X44_ :
					*usFontsize = _CHINESE_FONT_8X44_;
					break;
				case _FONTSIZE_12X19_ :
					*usFontsize = _CHINESE_FONT_12X19_;
					break;
				case _FONTSIZE_16X16_ :
					*usFontsize = _CHINESE_FONT_16X16_;
					break;
				case _FONTSIZE_16X22_ :
					*usFontsize = _CHINESE_FONT_16X22_;
					break;
				case _FONTSIZE_16X33_ :
					*usFontsize = _CHINESE_FONT_16X33_;
					break;
				case _FONTSIZE_16X44_ :
					*usFontsize = _CHINESE_FONT_16X44_;
					break;
				case _FONTSIZE_24X22_ :
					*usFontsize = _CHINESE_FONT_24X22_;
					break;
				case _FONTSIZE_24X33_ :
					*usFontsize = _CHINESE_FONT_24X33_;
					break;
				case _FONTSIZE_32X22_ :
					*usFontsize = _CHINESE_FONT_32X22_;
					break;
				default :
					return (VS_ERROR);
			}
		}
		else if (inLanguage == _DISP_ENGLISH_)
		{
			switch (inFontSize)
			{
				case _FONTSIZE_8X16_ :
					*usFontsize = _ENGLISH_FONT_8X16_;
					break;
				case _FONTSIZE_8X22_ :
					*usFontsize = _ENGLISH_FONT_8X22_;
					break;
				case _FONTSIZE_8X33_ :
					*usFontsize = _ENGLISH_FONT_8X33_;
					break;
                                case _FONTSIZE_8X34_ :
                                        *usFontsize = _ENGLISH_FONT_8X34_;
                                        break;
				case _FONTSIZE_8X44_ :
					*usFontsize = _ENGLISH_FONT_8X44_;
					break;
				case _FONTSIZE_12X19_ :
					*usFontsize = _ENGLISH_FONT_12X19_;
					break;
				case _FONTSIZE_16X16_ :
					*usFontsize = _ENGLISH_FONT_16X16_;
					break;
				case _FONTSIZE_16X22_ :
					*usFontsize = _ENGLISH_FONT_16X22_;
					break;
				case _FONTSIZE_16X33_ :
					*usFontsize = _ENGLISH_FONT_16X33_;
					break;
				case _FONTSIZE_16X44_ :
					*usFontsize = _ENGLISH_FONT_16X44_;
					break;
				case _FONTSIZE_24X22_ :
					*usFontsize = _ENGLISH_FONT_24X22_;
					break;
				case _FONTSIZE_32X22_ :
					*usFontsize = _ENGLISH_FONT_32X22_;
					break;
				default :
					return (VS_ERROR);
			}
		}
		else
		{
			switch (inFontSize)
			{
				case _FONTSIZE_8X16_ :
					*usFontsize = _CHINESE_FONT_8X16_;
					break;
				case _FONTSIZE_8X22_ :
					*usFontsize = _CHINESE_FONT_8X22_;
					break;
				case _FONTSIZE_8X33_ :
					*usFontsize = _CHINESE_FONT_8X33_;
					break;
                                case _FONTSIZE_8X34_ :
                                        *usFontsize = _CHINESE_FONT_8X34_;
                                        break;
				case _FONTSIZE_8X44_ :
					*usFontsize = _CHINESE_FONT_8X44_;
					break;
				case _FONTSIZE_12X19_ :
					*usFontsize = _CHINESE_FONT_12X19_;
					break;
				case _FONTSIZE_16X16_ :
					*usFontsize = _CHINESE_FONT_16X16_;
					break;
				case _FONTSIZE_16X22_ :
					*usFontsize = _CHINESE_FONT_16X22_;
					break;
				case _FONTSIZE_16X33_ :
					*usFontsize = _CHINESE_FONT_16X33_;
					break;
				case _FONTSIZE_16X44_ :
					*usFontsize = _CHINESE_FONT_16X44_;
					break;
				case _FONTSIZE_24X22_ :
					*usFontsize = _CHINESE_FONT_24X22_;
					break;
				case _FONTSIZE_32X22_ :
					*usFontsize = _CHINESE_FONT_32X22_;
					break;
				default :
					return (VS_ERROR);
			}		
		}
//	}	
	
	return (VS_SUCCESS);
}
/*
Function	:inDISP_TTF_SET
Date&Time	:2015/6/8 下午 2:50
Describe	:設定TTF字型及Style
*/
int inDISP_TTF_SetFont(int inLanguage, int inFontStyle)
{	
        if (inLanguage == _DISP_CHINESE_)
        {
		CTOS_FontTTFSelectFontFile(d_FONT_DEVICE_LCD_0, (unsigned char*)_CHINESE_FONE_1_, 0);
		CTOS_FontTTFSelectStyle(d_FONT_DEVICE_LCD_0, inFontStyle);
                ginSetFont = _DISP_CHINESE_;                                    /* 儲存目前的Font */
        }
        else if (inLanguage == _DISP_ENGLISH_)
        {
		CTOS_FontTTFSelectFontFile(d_FONT_DEVICE_LCD_0, (unsigned char*)_ENGLISH_FONE_1_, 0);
		CTOS_FontTTFSelectStyle(d_FONT_DEVICE_LCD_0, inFontStyle);
                ginSetFont = _DISP_ENGLISH_;                                    /* 儲存目前的Font */
        }

        return (VS_SUCCESS);
}

/*
Function        :inDISP_EnglishFont
Date&Time       :2015/8/28 下午 9:45
Describe        :顯示LCD彩色英數字
*/
int inDISP_EnglishFont_Color(char *szMessage, int inFontSize, int inLINE, int inColor, int inAlign)
{
	int	inRetVal = VS_SUCCESS;

        if (ginSetFont != _DISP_ENGLISH_ )
        {
                /* 判斷是否已經SetFont過_DISP_CHINESE_，如果沒有就要Set */
                inDISP_TTF_SetFont(_DISP_ENGLISH_, _FONT_DISPLAY_REGULAR_);     /* 微軟正黑體 */
        }
        /* 將字體顏色換色 */
        CTOS_LCDForeGndColor(inColor);

        inRetVal = inDISP_Select_FontSize(inFontSize, _DISP_ENGLISH_);
        if (inRetVal != VS_SUCCESS)
	{
		return (VS_ERROR);
	}

        CTOS_LCDTPrintAligned(inLINE, (unsigned char*)szMessage, inAlign);
        /* 將字體顏色換回黑色 */
        CTOS_LCDForeGndColor(0x00000000);

        return (VS_SUCCESS);
}
/*
Function	:inDISP_Enter8x16_GetAmount
Date&Time	:2016/9/29 上午 9:53
Describe	:輸入金額
*/
/*
 srDispObj->szOutput 為輸入的按鍵金額，
 透過inFunc_Amount_Comma 顯示幣別 + 金額+Comma +靠右/靠左
 */
int inDISP_Enter8x16_GetAmount(DISPLAY_OBJECT  *srDispObj)
{
	int		inChoice = 0;
	int		inColor;
	int		inFinalTimeOut;
	char		szTemplate[32 + 1];
        unsigned char   uszkey;

        if (srDispObj->inMenuKeyIn > 0)
        {
                /* 將輸入第一碼先存起來 */         
                sprintf(&srDispObj->szOutput[srDispObj->inOutputLen], "%c", srDispObj->inMenuKeyIn);
                srDispObj->inOutputLen ++;
        }
	
	/* 若TIMEOUT時間大於0時用傳進來的TimeOut，否則用EDC.dat的 */
//        if (srDispObj->inTimeout > 0)
//	{
//		inFinalTimeOut = srDispObj->inTimeout;
//	}
//	else
//	{
//		inFinalTimeOut = _EDC_TIMEOUT_;
//	}
	
	inColor = srDispObj->inColor;

	inDISP_Timer_Start(_TIMER_NEXSYS_1_, 30);
	while (1)
	{
                uszkey = -1;

                if (srDispObj->inMenuKeyIn > 0)
                {
                /* 如果為MenuKeyIn，第一個數字要顯示 */
                        uszkey = _MENUKEYIN_EVENT_;
                }
                else
                {
//			inChoice = inDisTouch_TouchSensor_Click_Slide(srDispObj->inTouchSensorFunc);
			uszkey = uszKBD_Key();
                }
		
//		if (inChoice == _Touch_OX_LINE8_8_ENTER_BUTTON_)
//		{
//			uszkey = _KEY_ENTER_;
//		}
//		else if (inChoice == _Touch_OX_LINE8_8_CANCEL_BUTTON_)
//		{
//			uszkey = _KEY_CANCEL_;
//		}
		
		/* Timeout */
		if (inTimerGet(_TIMER_NEXSYS_1_) == VS_SUCCESS)
		{
			uszkey = _KEY_TIMEOUT_;
		}
                
                switch (uszkey)
                {
                        case _KEY_CANCEL_ :
                                srDispObj->inOutputLen = 0;
                                memset(srDispObj->szOutput, 0x00, sizeof(srDispObj->szOutput));
				/* 清空Touch資料 */
//				inDisTouch_Flush_TouchFile();	
                                return (VS_USER_CANCEL);
                        case _KEY_TIMEOUT_ :
                                srDispObj->inOutputLen = 0;
                                memset(srDispObj->szOutput, 0x00, sizeof(srDispObj->szOutput));
				/* 清空Touch資料 */
//				inDisTouch_Flush_TouchFile();
                                return(VS_TIMEOUT);
                        case _KEY_ENTER_ :
                                /* 先確認該部份是否可輸入0 和 ByPass */
				if (srDispObj->inCanNotZero != VS_TRUE && srDispObj->inCanNotBypass != VS_TRUE)
				{
					/* 清空Touch資料 */
//					inDisTouch_Flush_TouchFile();			
					return (srDispObj->inOutputLen);
				}
				/* 不能ByPass但可以輸入0 */
				else if (srDispObj->inCanNotZero != VS_TRUE && srDispObj->inCanNotBypass == VS_TRUE)
				{
					if (srDispObj->inOutputLen == 0)
					{
//						inDISP_BEEP(1, 0);
						continue;
					}
					else
					{
						/* 清空Touch資料 */
//						inDisTouch_Flush_TouchFile();					
						return (srDispObj->inOutputLen);
					}
				}
				/* 不能輸入0但可以ByPass */
				else if (srDispObj->inCanNotZero == VS_TRUE && srDispObj->inCanNotBypass != VS_TRUE)
				{
					/* 判斷輸入為零 提示聲音+重新輸入 */
					if (atol(srDispObj->szOutput) == 0L)
					{
//						inDISP_BEEP(1, 0);
						continue;
					}
					else
					{
						/* 清空Touch資料 */
//						inDisTouch_Flush_TouchFile();						
						return (srDispObj->inOutputLen);
					}
					
				}
				/* 不能輸入0也不能ByPass */
				else
				{
					/* 判斷輸入為零 提示聲音+重新輸入 */
					if (atol(srDispObj->szOutput) == 0L || srDispObj->inOutputLen == 0)
					{
//						inDISP_BEEP(1, 0);
						continue;
					}
					else
					{
						/* 清空Touch資料 */
//						inDisTouch_Flush_TouchFile();					
						return (srDispObj->inOutputLen);
					}
					
				}
                        case _KEY_CLEAR_ :
                                if (srDispObj->inOutputLen > 0)
                                {
                                        srDispObj->szOutput[srDispObj->inOutputLen - 1] = 0x00;
                                        srDispObj->inOutputLen --;
                                        break;
                                }
                                else
                                        continue;
                        case _KEY_0_ :
                        case _KEY_1_ :
                        case _KEY_2_ :
                        case _KEY_3_ :
                        case _KEY_4_ :
                        case _KEY_5_ :
                        case _KEY_6_ :
                        case _KEY_7_ :
                        case _KEY_8_ :
                        case _KEY_9_ :
				/* 金額第一位數不能為0 */
                                if (srDispObj->inOutputLen == 0 && uszkey - 48 == 0)
                                {
                                        continue;
                                }
				/* 若超過最大長度時長嗶一聲 */
				else if (srDispObj->inOutputLen >= srDispObj->inMaxLen)
				{
//					inDISP_BEEP(1, 0);
					continue;
				}

                                sprintf(&srDispObj->szOutput[srDispObj->inOutputLen], "%c", uszkey);
                                srDispObj->inOutputLen ++;
                                break;
                        case _MENUKEYIN_EVENT_:
                                /* 將inMenuKeyIn初始化 */
                                srDispObj->inMenuKeyIn = -1;
                                break;
                        default :
                                continue;
                }
                
                /* 設定螢幕字型大小 */
		/* _ENGLISH_FONT_8X16_ */
//		if (ginHalfLCD == VS_TRUE)
//			srDispObj->inFoneSize = _ENGLISH_FONT_8X16_HALF_;
//		else
			srDispObj->inFoneSize = _ENGLISH_FONT_8X16_;

                /* 一律先把畫面清掉後再顯示輸入訊息 */
                if (srDispObj->inX != 0)
                        //不懂
			inDISP_Clear_Area(srDispObj->inX, srDispObj->inY, 16, srDispObj->inY, srDispObj->inFoneSize);
		else
			inDISP_Clear_Area(1, srDispObj->inY, 16, srDispObj->inY, srDispObj->inFoneSize);

		memset(szTemplate, 0x00, sizeof(szTemplate));
		strcpy(szTemplate, srDispObj->szOutput);
		/* 如果砍到沒金額，顯示0元而不是空白 */
		if (strlen(szTemplate) == 0)
		{
			strcat(szTemplate, "0");
		}
		
                if (srDispObj->inR_L == _DISP_LEFT_)
                {
			inFunc_Amount_Comma(szTemplate, srDispObj->szPromptMsg, 0x00, _SIGNED_NONE_, 16, _PADDING_RIGHT_);
                        inDISP_EnglishFont_Color(szTemplate, _FONTSIZE_8X16_, srDispObj->inY, inColor, _DISP_LEFT_);
                }
                else if (srDispObj->inR_L == _DISP_RIGHT_)
                {       
			inFunc_Amount_Comma(szTemplate, srDispObj->szPromptMsg, 0x00, _SIGNED_NONE_, 16, _PADDING_RIGHT_);                   
                        inDISP_EnglishFont_Color(szTemplate, _FONTSIZE_8X16_, srDispObj->inY, inColor, _DISP_RIGHT_);
                }
		
    	}
}
int inDISP_Enter8x16_GetSetting(DISPLAY_OBJECT  *srDispObj)
{
	int		inChoice = 0;
	int		inColor;
	int		inFinalTimeOut;
	char		szTemplate[32 + 1];
        unsigned char   uszkey;

//        if (srDispObj->inMenuKeyIn > 0)
//        {
//                /* 將輸入第一碼先存起來 */         
//                sprintf(&srDispObj->szOutput[srDispObj->inOutputLen], "%c", srDispObj->inMenuKeyIn);
//                srDispObj->inOutputLen ++;
//        }
	
	/* 若TIMEOUT時間大於0時用傳進來的TimeOut，否則用EDC.dat的 */
//        if (srDispObj->inTimeout > 0)
//	{
//		inFinalTimeOut = srDispObj->inTimeout;
//	}
//	else
//	{
//		inFinalTimeOut = _EDC_TIMEOUT_;
//	}
//	inGetHostIPPrimary(srDispObj->szPromptMsg);
	inColor = srDispObj->inColor;
     
	inDISP_Timer_Start(_TIMER_NEXSYS_1_, 30);
	while (1)
	{
                uszkey = -1;
                uszkey = uszKBD_Key();
//                if (strlen(srDispObj->szPromptMsg) > 0)
//                {
//                /* 如果為MenuKeyIn，第一個數字要顯示 */
//                        uszkey = _MENUKEYIN_EVENT_;
//                }
//                else
//                {
////			inChoice = inDisTouch_TouchSensor_Click_Slide(srDispObj->inTouchSensorFunc);
//			uszkey = uszKBD_Key();
//                }
		
//		if (inChoice == _Touch_OX_LINE8_8_ENTER_BUTTON_)
//		{
//			uszkey = _KEY_ENTER_;
//		}
//		else if (inChoice == _Touch_OX_LINE8_8_CANCEL_BUTTON_)
//		{
//			uszkey = _KEY_CANCEL_;
//		}
		
		/* Timeout */
		if (inTimerGet(_TIMER_NEXSYS_1_) == VS_SUCCESS)
		{
			uszkey = _KEY_TIMEOUT_;
		}
                
                switch (uszkey)
                {
                        case _KEY_CANCEL_ :
                                srDispObj->inOutputLen = 0;
                                memset(srDispObj->szOutput, 0x00, sizeof(srDispObj->szOutput));
				/* 清空Touch資料 */
//				inDisTouch_Flush_TouchFile();	
                                return (VS_USER_CANCEL);
                        case _KEY_TIMEOUT_ :
                                srDispObj->inOutputLen = 0;
                                memset(srDispObj->szOutput, 0x00, sizeof(srDispObj->szOutput));
				/* 清空Touch資料 */
//				inDisTouch_Flush_TouchFile();
                                return(VS_TIMEOUT);
                        case _KEY_ENTER_ :
                                /* 先確認該部份是否可輸入0 和 ByPass */
				if (srDispObj->inCanNotZero != VS_TRUE && srDispObj->inCanNotBypass != VS_TRUE)
				{
					/* 清空Touch資料 */
//					inDisTouch_Flush_TouchFile();			
					return (srDispObj->inOutputLen);
				}
				/* 不能ByPass但可以輸入0 */
				else if (srDispObj->inCanNotZero != VS_TRUE && srDispObj->inCanNotBypass == VS_TRUE)
				{
					if (srDispObj->inOutputLen == 0)
					{
//						inDISP_BEEP(1, 0);
						continue;
					}
					else
					{
						/* 清空Touch資料 */
//						inDisTouch_Flush_TouchFile();					
						return (srDispObj->inOutputLen);
					}
				}
				/* 不能輸入0但可以ByPass */
				else if (srDispObj->inCanNotZero == VS_TRUE && srDispObj->inCanNotBypass != VS_TRUE)
				{
					/* 判斷輸入為零 提示聲音+重新輸入 */
					if (atol(srDispObj->szOutput) == 0L)
					{
//						inDISP_BEEP(1, 0);
						continue;
					}
					else
					{
						/* 清空Touch資料 */
//						inDisTouch_Flush_TouchFile();						
						return (srDispObj->inOutputLen);
					}
					
				}
				/* 不能輸入0也不能ByPass */
				else
				{
					/* 判斷輸入為零 提示聲音+重新輸入 */
					if (atol(srDispObj->szOutput) == 0L || srDispObj->inOutputLen == 0)
					{
//						inDISP_BEEP(1, 0);
						continue;
					}
					else
					{
						/* 清空Touch資料 */
//						inDisTouch_Flush_TouchFile();					
						return (srDispObj->inOutputLen);
					}
					
				}
                        case _KEY_CLEAR_ :
                                if (srDispObj->inOutputLen > 0)
                                {
                                        srDispObj->szOutput[srDispObj->inOutputLen - 1] = 0x00;
                                        srDispObj->inOutputLen --;
                                        break;
                                }
                                else
                                        continue;
                        case _KEY_0_ :
                        case _KEY_1_ :
                        case _KEY_2_ :
                        case _KEY_3_ :
                        case _KEY_4_ :
                        case _KEY_5_ :
                        case _KEY_6_ :
                        case _KEY_7_ :
                        case _KEY_8_ :
                        case _KEY_9_ :
                        case _KEY_ALPHA_:
				/* 金額第一位數不能為0 */
//                                if (srDispObj->inOutputLen == 0 && uszkey - 48 == 0)
//                                {
//                                        continue;
//                                }
				/* 若超過最大長度時長嗶一聲 */
				if (srDispObj->inOutputLen >= srDispObj->inMaxLen)
				{
//					inDISP_BEEP(1, 0);
					continue;
				}
                                uszkey = (uszkey ==_KEY_ALPHA_) ? '.' : uszkey;    
                                sprintf(&srDispObj->szOutput[srDispObj->inOutputLen], "%c", uszkey);
                                srDispObj->inOutputLen ++;
                                break;
                        case _MENUKEYIN_EVENT_:
                                /* 將inMenuKeyIn初始化 */
                                srDispObj->inMenuKeyIn = -1;
                                break;
                        default :
                                continue;
                }
                
                /* 設定螢幕字型大小 */
		/* _ENGLISH_FONT_8X16_ */
//		if (ginHalfLCD == VS_TRUE)
//			srDispObj->inFoneSize = _ENGLISH_FONT_8X16_HALF_;
//		else
			srDispObj->inFoneSize = _ENGLISH_FONT_8X16_;

                /* 一律先把畫面清掉後再顯示輸入訊息 */
                if (srDispObj->inX != 0)
			inDISP_Clear_Area(srDispObj->inX, srDispObj->inY, 16, srDispObj->inY, srDispObj->inFoneSize);
		else
			inDISP_Clear_Area(1, srDispObj->inY, 16, srDispObj->inY, srDispObj->inFoneSize);
                
		memset(szTemplate, 0x00, sizeof(szTemplate));
		strcpy(szTemplate, srDispObj->szOutput);
		/* 如果砍到沒金額，顯示0元而不是空白 */
		if (strlen(szTemplate) == 0)
		{
			strcat(szTemplate, "0");
		}
		
                if (srDispObj->inR_L == _DISP_LEFT_)
                {
//			inFunc_Amount_Comma(szTemplate, srDispObj->szPromptMsg, 0x00, _SIGNED_NONE_, 16, _PADDING_RIGHT_);
                        inDISP_EnglishFont_Color(szTemplate, _FONTSIZE_8X16_, srDispObj->inY, inColor, _DISP_LEFT_);
                }
                else if (srDispObj->inR_L == _DISP_RIGHT_)
                {       

//			inFunc_Amount_Comma(szTemplate, srDispObj->szPromptMsg, 0x00, _SIGNED_NONE_, 16, _PADDING_RIGHT_);                   
                        inDISP_EnglishFont_Color(szTemplate, _FONTSIZE_8X16_, srDispObj->inY, inColor, _DISP_RIGHT_);
                }
		
    	}
}

/*
Function	:inDISP_Clear_Line
Date&Time	:2015/6/7 下午 5:09
Describe	:清除局部矩形螢幕
*/
/*
 * 將游標移到特定位置(CTOS_LCDTGotoXY )，並清除從游標位置到行尾顯示的所有字元(CTOS_LCDTClear2EOL)。
 */
int inDISP_Clear_Line(int inLineT, int inLineB)
{
	int i;

//	if (ginHalfLCD == VS_TRUE)
//	{
//		CTOS_LCDTSelectFontSize(_ENGLISH_FONT_8X16_HALF_);
//	}
//	else
//	{
		CTOS_LCDTSelectFontSize(_ENGLISH_FONT_8X16_);
//	}

	for (i = inLineT; i <= inLineB; i++)
	{
                /*多這個if判斷的用意為何，不懂*/
                if ( i == inLineT)
                        CTOS_LCDTGotoXY(1, i);
                else
                        CTOS_LCDTGotoXY(1, i);
                        
		CTOS_LCDTClear2EOL();
	}

        return (VS_SUCCESS);
}


/*
Function	:inPutGraphic
Date&Time	:2015/6/7 下午 5:09
Describe	:顯示LCD BMP圖檔
 */
int inDISP_PutGraphic(char *szFileName, int inX, int inY) {
    int inY_Modify;
    char szDebugMsg[100 + 1];
    unsigned short usRetVal = 0;
    //	if (ginHalfLCD == VS_TRUE)
    //		inY_Modify = inY / 2;
    //	else
    inY_Modify = inY;

    usRetVal = CTOS_LCDGShowBMPPic(inX, inY_Modify, (BYTE *) szFileName);
    
    if (usRetVal == d_OK) {
        printf(" CTOS_LCDGShowBMPPic == d_OK\n");
    } else {
        printf("VS_ERROR,FileName is %s\n",szFileName);
        return (VS_ERROR);
    }
    return (VS_SUCCESS);
}
/*
Function        :inTimerStart_MicroSecond
Date&Time       :2018/2/9 下午 1:55
Describe        :設定計時器開啟，inTimerNbr是第幾號計時器，lnDelay是多久Timeout
 *		 lnDelay傳入單位為 10MicroSecond泛用度更高
*/
int inTimerStart_MicroSecond(int inTimerNbr, long lnDelay)
{
        CTOS_TimeOutSet(inTimerNbr, lnDelay);

        return (VS_SUCCESS);
}

/*
Function        :inDISP_Timer_Start_MicroSecond
Date&Time       :2018/2/9 下午 1:57
Describe        :當lnDelay設為-1，使用EDC.dat內的TimeOut，時間單位更細的API
*/
int inDISP_Timer_Start_MicroSecond(int inTimerNumber, long lnDelay)
{
	int	inEnterTimeout = 0;
	char	szEnterTimeout[3 + 1];
	
//	if (lnDelay == _EDC_TIMEOUT_)
//	{
//		memset(szEnterTimeout, 0x00, sizeof(szEnterTimeout));
//		inGetEnterTimeout(szEnterTimeout);
//		inEnterTimeout = atoi(szEnterTimeout);
//		if (inEnterTimeout != 0)
//		{
//			inTimerStart_MicroSecond(inTimerNumber, (long)inEnterTimeout);
//		}
//		else
//		{
//			return (VS_ERROR);
//		}
//	}
//	else
//	{
                //暫時不跑EDC.dat內的TimeOut
		inTimerStart_MicroSecond(inTimerNumber, lnDelay);
//	}
	
	return (VS_SUCCESS);
}
/*
Function	:inPRINT_Buffer_Get_Height
Date&Time	:2016/3/16 下午 2:15
Describe	:用來預先獲取圖檔的高度，以便利用
 */
int inPRINT_Buffer_GetHeight(unsigned char* uszFileName, int* inHeight) {
    int i;
    int inCalulateHeight = 0;
    char szAscii[8 + 1];
    char szDebugMsg[100 + 1];
    unsigned char uszHex[4 + 1];
    unsigned char uszHeight[4 + 1];
    unsigned long ulHandle;

    memset(&ulHandle, 0x00, sizeof (ulHandle));
    memset(uszHeight, 0x00, sizeof (uszHeight));
    memset(uszHex, 0x00, sizeof (uszHex));
    memset(szAscii, 0x00, sizeof (szAscii));
    /* 初始為0 若因錯誤而跳出則高度為0 */
    *inHeight = 0;

    if (inFILE_OpenReadOnly(&ulHandle, uszFileName) != VS_SUCCESS) {
        return (VS_ERROR);
    }
    /* 16 in hex = 22 in decimal 圖檔放高度資訊的位置 */
    //參數三傳入0  >>  inSeekMode == _SEEK_BEGIN_
    if (inFILE_Seek(ulHandle, 22, 0) != VS_SUCCESS) {
        printf("inFILE_Seek\n");
        return (VS_ERROR);
    }

    if (inFILE_Read(&ulHandle, uszHeight, 4) != VS_SUCCESS) {
        return (VS_ERROR);
    }

    /* height資料只有四個bytes，因為是Little-Endian，所以要先反過來 */
    for (i = 0; i < 4; i++) {
        memcpy(&uszHex[i], &uszHeight[3 - i], 1);
    }
    /* 16進制轉10進制，以byte為單位，從高位乘上256加到低位 */
    for (i = 0; i < 4; i++) { // x*256 = x <<8(256 == 2^8) 
        //uszHex[i] - 0 是為了把字元強制轉整數?
        inCalulateHeight *= 256;
        inCalulateHeight += uszHex[i] - 0;
    }

    /* 加到變數上 */
    *inHeight = inCalulateHeight;
    /* 關閉檔案 */
    inFILE_Close(&ulHandle);
    return (VS_SUCCESS);
}
int inDISP_Timer_Start(int inTimerNumber, long lnDelay) {
//    int inEnterTimeout = 0;
//    char szEnterTimeout[3 + 1];

//    if (lnDelay == _EDC_TIMEOUT_) {
//        memset(szEnterTimeout, 0x00, sizeof (szEnterTimeout));
//        //inGetEnterTimeout(szEnterTimeout); //to fix
//        inEnterTimeout = atoi(szEnterTimeout);
//        if (inEnterTimeout != 0) {
//            inTimerStart(inTimerNumber, (long) inEnterTimeout);
//        } else {
//            return (VS_ERROR);
//        }
//    } 
//    else {
        inTimerStart(inTimerNumber, lnDelay);
//    }

    return (VS_SUCCESS);
}
/*
Function        :inTimerGet
Date&Time       :2016/6/21 下午 1:54
Describe        :確認計時器是否TimeOut，若timeout會回傳VS_SUCCESS
*/
int inTimerGet(int inTimerNbr)
{
         if (CTOS_TimeOutCheck(inTimerNbr) == d_YES)
                return (VS_SUCCESS);
         else
                return (VS_ERROR);
}

int inTimerStart(int inTimerNbr, long lnDelay) {
    CTOS_TimeOutSet(inTimerNbr, lnDelay * 100);

    return (VS_SUCCESS);
}

/*
Function        :inDISP_ChineseFont_Point_Color_By_Graphic_Mode
Date&Time       :2018/4/11 上午 9:20
Describe        :如果用Text Mode會有中文字和英文字對不齊的問題，這時候可以改用Graphic_Mode
*/
//("1", _FONTSIZE_16X22_, _COLOR_BLACK_, _COLOR_WHITE_, _Distouch_KEY_IN_MENU_LINE_3_TO_8_3X4_Touch_KEY_1_BUTTON_Xm_, _Distouch_KEY_IN_MENU_LINE_3_TO_8_3X4_Touch_KEY_1_BUTTON_Y1_, VS_FALSE);
int inDISP_ChineseFont_Point_Color_By_Graphic_Mode(char *szMessage, int inFontSize, int inForeColor, int inBackColor, int inX, int inY, unsigned char uszReverse)
{
	int		inY_Modify;
	int		inRetVal = VS_SUCCESS;
	unsigned short	usFontSize = 0;
	
        if (ginSetFont != _DISP_CHINESE_ )
        {
                /* 判斷是否已經SetFont過_DISP_CHINESE_，如果沒有就要Set */
                inDISP_TTF_SetFont(_DISP_CHINESE_, _FONT_DISPLAY_REGULAR_);     /* 微軟正黑體 */
        }
	
	/* 將字體顏色換色 */
        CTOS_LCDForeGndColor(inForeColor);
	/* 將背景顏色換色 */
	CTOS_LCDBackGndColor(inBackColor);
	
	inDISP_Decide_FontSize(inFontSize, _DISP_CHINESE_, &usFontSize);

//	if (ginHalfLCD == VS_TRUE)
//		inY_Modify = inY / 2;
//	else
		inY_Modify = inY;
	
        inRetVal = CTOS_LCDGTextOut(inX, inY_Modify, (unsigned char*)szMessage, usFontSize, uszReverse);
	if (inRetVal != d_OK)
	{
            printf("0x%04X\n", inRetVal);
	}

        /* 將字體顏色換回黑色 */
        CTOS_LCDForeGndColor(0x00000000);
	/* 將背景顏色換回白色 */
	CTOS_LCDBackGndColor(0x00FFFFFF);

        return (VS_SUCCESS);
}