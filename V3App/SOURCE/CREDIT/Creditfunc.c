#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../Trans.h"
#include "../../PrtMsg.h"
#include "../DISPLAY/Display.h"
#include "../INCLUDE/Define_1.h"

/*
Function	:inCREDIT_Func_Get_OPT_Amount
Date&Time	:2016/12/29 上午 11:20
Describe	:輸入金額
*/
int inCREDIT_Func_Get_OPT_Amount(TRANSACTION_OBJECT *pobTran)
{
	int		inRetVal;
        char		szTemplate[_DISP_MSG_SIZE_ + 1];
        DISPLAY_OBJECT  srDispObj;

	/* idle進入 */
//	if (ginEventCode >= '1' && ginEventCode <= '9')
//        {
		/* 若ECR或idle有輸入金額就跳走，OPT已經輸入過 */
//		if (pobTran->uszECRBit == VS_TRUE)
//		{
//			if (pobTran->srBRec.lnTxnAmount > 0)
//			{
//				return (VS_SUCCESS);
//			}
//                        
//			/* 兩段式收銀機連線 第一段不輸入 這邊只擋會出現在OPT的 */
//			if (pobTran->uszCardInquiryFirstBit == VS_TRUE)
//			{
//				return (VS_SUCCESS);
//			}
//		}

		memset(&srDispObj, 0x00, sizeof(DISPLAY_OBJECT));
		memset(szTemplate, 0x00, sizeof(szTemplate));

		srDispObj.inY = _LINE_8_7_;
		srDispObj.inR_L = _DISP_RIGHT_;
//		srDispObj.inMaxLen = inFunc_Check_Digit();      /* 不可超過9，long變數最多放9位 */
		srDispObj.inMaxLen = 9; 
//              srDispObj.inMenuKeyIn = ginEventCode;
                srDispObj.inMenuKeyIn = pobTran->inMenuKeyin;
		srDispObj.inCanNotBypass = VS_TRUE;
		srDispObj.inCanNotZero = VS_TRUE;
		srDispObj.inColor = _COLOR_RED_;
//		srDispObj.inTouchSensorFunc = _Touch_OX_LINE8_8_;
		strcpy(srDispObj.szPromptMsg, "NT$ ");
 
		inDISP_Clear_Line(_LINE_8_4_, _LINE_8_8_);
		/* 輸入金額 */
		inDISP_PutGraphic(_GET_AMOUNT_, 0, _COORDINATE_Y_LINE_8_4_);
//		inDISP_PutGraphic(_MSG_ENTER_OR_CANCEL_, 0, _COORDINATE_Y_LINE_8_8_);

		memset(srDispObj.szOutput, 0x00, sizeof(srDispObj.szOutput));
		srDispObj.inOutputLen = 0;
                
		inRetVal = inDISP_Enter8x16_GetAmount(&srDispObj);
                    
		if (inRetVal == VS_TIMEOUT || inRetVal == VS_USER_CANCEL)
		{
//			vdUtility_SYSFIN_LogMessage(AT, "inCREDIT_Func_Get_OPT_Amount Timeout_Or_UserCancel(%d) END!", inRetVal);
                        printf("inCREDIT_Func_Get_OPT_Amount Timeout_Or_UserCancel(%d) END!\n",inRetVal);
                        return (inRetVal);
		}

		pobTran->srBRec.lnTxnAmount = atol(srDispObj.szOutput);
		pobTran->srBRec.lnOrgTxnAmount = atol(srDispObj.szOutput);
		pobTran->srBRec.lnTotalTxnAmount = atol(srDispObj.szOutput);
//	}
        

	return (VS_SUCCESS);
}
