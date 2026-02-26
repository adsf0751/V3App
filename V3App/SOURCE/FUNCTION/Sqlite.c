#include "../../Trans.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctosapi.h>
#include <sqlite3.h>
#include <unistd.h>
#include <errno.h>
#include "Sqlite.h"
#include "../INCLUDE/Define_1.h"
char		gszTranDBPath[100 + 1] = {_FS_DATA_PATH_ _DATA_BASE_NAME_NEXSYS_TRANSACTION_};
sqlite3		*gsrDBConnection;	/* 建立到資料庫的connection */
sqlite3_stmt	*gsrSQLStat;
/*
Function        :inSqlite_Initial
Date&Time       :2017/3/14 上午 10:50
Describe        :
*/
int inSqlite_Initial()
{

	/* 設定*/
	inSqlite_Initial_Setting();

	return (VS_SUCCESS);
}
/*
Function        :inSqlite_Initial_Setting
Date&Time       :2017/3/14 上午 10:33
Describe        :有一些設定要改
*/
int inSqlite_Initial_Setting()
{
	int		inRetVal;
	int		inSwitch;
	int		inState;
	char		szDebugMsg[100 + 1];
	sqlite3		*srDBConnection;		/* 建立到資料庫的connection */
	
	/* 開啟DataBase檔 */
	inRetVal = inSqlite_DB_Open_Or_Create(gszTranDBPath, &srDBConnection, _SQLITE_DEFAULT_FLAGS_, NULL);
	if (inRetVal != VS_SUCCESS)
	{	
		return (VS_ERROR);
	}
	else
	{
		
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Open Database File OK");
            printf("%s\n",szDebugMsg);
		
	}
	
	/* SQLlite 外鍵預設為關閉，改成開啟(不然用外鍵就沒意義了) (1代表開啟 0代表關閉)*/
	inSwitch = 1;
	inRetVal = sqlite3_db_config(srDBConnection, SQLITE_DBCONFIG_ENABLE_FKEY, inSwitch, &inState);
	if (inRetVal != SQLITE_OK)
	{
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "SQLITE_DBCONFIG_ENABLE_FKEY Switch :%d ERROR Num%d", inSwitch, inRetVal);
            printf("%s\n",szDebugMsg);

            inSqlite_DB_Close(&srDBConnection);
            return (VS_ERROR);
	}
	else
	{
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "SQLITE_DBCONFIG_ENABLE_FKEY Switch :%d OK, State: %d", inSwitch, inState);
            printf("%s\n",szDebugMsg);
		
	}
	
	/* 關閉 database, close null pointer 是NOP(No Operation) */
	inRetVal = inSqlite_DB_Close(&srDBConnection);
	if (inRetVal != VS_SUCCESS)
	{
		
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "sqlite3_close Fail ErrNum :%d", inRetVal);
            printf("%s\n",szDebugMsg);
            /* 如果資料還在更新就close會因為SQLITE_BUSY而失敗，而且正在更新的事務也會roll back（回復上一動）*/
            return (VS_ERROR);
	}
	
	return (VS_SUCCESS);
}

/*
Function        :inSqlite_DB_Open_Or_Create
Date&Time       :2019/6/3 下午 6:26
Describe        :開啟DB，」若無則創建
*/
int inSqlite_DB_Open_Or_Create(char* szDBPath, sqlite3** srDBConnection, int inFlags, char* szVfs)
{
	int	inRetVal = SQLITE_OK;
	char	szDebugMsg[84 + 1] = {0};
	int version = sqlite3_libversion_number();
        
	inRetVal = sqlite3_open_v2(szDBPath, srDBConnection, inFlags, szVfs);
	if (inRetVal != SQLITE_OK)
	{

                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Open Database File Failed");
                printf("%s\n",szDebugMsg);

                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "ErrorNum:%d", inRetVal);
                printf("%s\n",szDebugMsg);
		
		return (VS_ERROR);
	}
	else
	{	
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Open Database File OK");
                printf("%s\n",szDebugMsg);		
//		inFile_Open_File_Cnt_Increase();
		
		return (VS_SUCCESS);
	}
}
/*
Function        :inSqlite_DB_Close
Date&Time       :2019/9/6 上午 10:00
Describe        :關閉DB
*/
int inSqlite_DB_Close(sqlite3** srDBConnection)
{
	int             inRetVal = SQLITE_OK;
	char            szDebugMsg[84 + 1] = {0};
        sqlite3_stmt    *pStmt = NULL;
	
        if (srDBConnection == NULL || *srDBConnection == NULL)
        {
//                vdUtility_SYSFIN_LogMessage(AT, "inSqlite_DB_Close *srDBConnection == NULL");
                inRetVal = SQLITE_ERROR;
        }
        else
        {
                while (sqlite3_close(*srDBConnection) == SQLITE_BUSY)
                {
                        /* 強制 finalize 所有未釋放的 statement */
                        pStmt = sqlite3_next_stmt(*srDBConnection, NULL);
                        if (pStmt != NULL)
                        {
                            printf("stmt not released\n");
                        }
                        
                        while (pStmt)
                        {
                                if (pStmt != NULL)
                                {
                                    printf("%s", sqlite3_sql(pStmt));
                                }
                                inSqlite_SQL_Finalize(&pStmt);
                                pStmt = sqlite3_next_stmt(*srDBConnection, NULL);
                        }
                }

                *srDBConnection = NULL;
        }
        
	if (inRetVal != SQLITE_OK)
	{

            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Close Database File Failed");
            printf("%s\n",szDebugMsg);

            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "ErrorNum:%d", inRetVal);
            printf("%s\n",szDebugMsg);

            return (VS_ERROR);
	}
	else
	{
		
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Close Database File OK");
            printf("%s\n",szDebugMsg);
		
//		inFile_Open_File_Cnt_Decrease();
		
            return (VS_SUCCESS);
	}
	
}
/*
Function        :inSqlite_SQL_Finalize
Date&Time       :2019/10/24 下午 5:55
Describe        :
*/
int inSqlite_SQL_Finalize(sqlite3_stmt **srSQLStat)
{
	int	inRetVal = SQLITE_OK;
	char	szDebugMsg[84 + 1] = {0};
	
        if (srSQLStat != NULL && *srSQLStat != NULL)
        {
            sqlite3_finalize(*srSQLStat);
            *srSQLStat = NULL;
        }
        else
        {
            printf("inSqlite_SQL_Finalize *srSQLStat == NULL\n");
            inRetVal = SQLITE_ERROR;
        }
	
	if (inRetVal != SQLITE_OK)
	{
		
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "sqlite3_finalize Failed");
                printf("%s\n",szDebugMsg);

                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "ErrorNum:%d", inRetVal);
                printf("%s\n",szDebugMsg);
			
		return (VS_ERROR);
	}
	else
	{
		
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "sqlite3_finalize OK");
            printf("%s\n",szDebugMsg);


            return (VS_SUCCESS);
	}
}

/*
Function        :inSqlite_CreateBatchTable
Date&Time       :2016/3/28 上午 10:47
Describe        :Open Database檔 建立table 這function只負責建batch的table
*/
int inSqlite_Create_Table(char* szDBName, char* szTableName, SQLITE_TAG_TABLE* pobSQLTag)
{
	int		i;
	int		inRetVal;
	int		inSqlLength = 0;
	char		szSqlPrefix[100 + 2];		/* CREATE TABLE	szTableName( */
	char		szSqlSuffix[10 + 2];		/* ); */
	char		szDebugMsg[100 + 1];
	char		*szCreateSql;			/* 因為會很長且隨table浮動，所以用pointer */
	char		*szErrorMessage = NULL;
	sqlite3		*srDBConnection;		/* 建立到資料庫的connection */
		

	/* 開啟DataBase檔 */
	inRetVal = inSqlite_DB_Open_Or_Create(szDBName, &srDBConnection, _SQLITE_DEFAULT_FLAGS_, NULL);
	
	if (inRetVal != VS_SUCCESS)
	{
		return (VS_ERROR);
	}
	else
	{	
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Open Database File OK");
                printf("%s\n", szDebugMsg);
		
	}

	/* 塞入SQL語句 */
	/* 為了使table name可變動，所以拉出來組 */
	/* 加入了if not exists字串，若已建立不會重複建立 */
	/* 前綴 */
	memset(szSqlPrefix, 0x00, sizeof(szSqlPrefix));
	sprintf(szSqlPrefix, "CREATE TABLE if not exists %s(", szTableName);
	/*  "CREATE TABLE	szTableName("的長度 */
	inSqlLength += strlen(szSqlPrefix);
	
	
	/* 計算要分配的記憶體長度 */
	for (i = 0;; i ++)
	{
		/* 碰到Table底部 */
		if (strlen((char*)&pobSQLTag[i].szTag) == 0)
		{
			break;
		}
		
		/* 第一行前面不加逗號，其他都要 */
		if (i > 0)
		{
			inSqlLength += strlen(",");
		}
		
		/* Tag Name */
		inSqlLength += strlen((char*)&pobSQLTag[i].szTag);
		/* Tag 型別 */
		if (strlen((char*)&pobSQLTag[i].szType) > 0)
		{
			inSqlLength += strlen(" ");
			inSqlLength += strlen((char*)&pobSQLTag[i].szType);
		}
		/* Tag 屬性1 */
		if (strlen((char*)&pobSQLTag[i].szAttribute1) > 0)
		{
			inSqlLength += strlen(" ");
			inSqlLength += strlen((char*)&pobSQLTag[i].szAttribute1);
		}
		/* Tag 屬性2 */
		if (strlen((char*)&pobSQLTag[i].szAttribute2) > 0)
		{
			inSqlLength += strlen(" ");
			inSqlLength += strlen((char*)&pobSQLTag[i].szAttribute2);
		}
	}
	
	/* 後綴 */
	memset(szSqlSuffix, 0x00, sizeof(szSqlSuffix));
	sprintf(szSqlSuffix, ");");
	inSqlLength += strlen(szSqlSuffix);
        
	/* inSqlLength: */
        memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
        sprintf(szDebugMsg, "inSqlLength: %d", inSqlLength);
        printf("%s\n", szDebugMsg);
	
	/* 配置記憶體(一定要+1，超級重要，不然會overflow) */
	szCreateSql = malloc(inSqlLength + 1);
	memset(szCreateSql, 0x00, inSqlLength);
	
	/* 先丟前綴Table Name */
	strcat(szCreateSql, szSqlPrefix);
	
	/* table要哪些tag */
	for (i = 0 ;; i ++)
        {
		/* 碰到Table底部 */
		if (strlen((char*)&pobSQLTag[i].szTag) == 0)
		{
			break;
		}
		
		/* 第一行前面不加逗號，其他都要 */
		if (i > 0)
		{
			strcat(szCreateSql , ",");
		}
		
		/* Tag Name */
		strcat(szCreateSql, (char*)&pobSQLTag[i].szTag);
		/* Tag 型別 */
		if (strlen((char*)&pobSQLTag[i].szType) > 0)
		{
			strcat(szCreateSql, " ");
			strcat(szCreateSql, (char*)&pobSQLTag[i].szType);
		}
		/* Tag 屬性1 */
		if (strlen((char*)&pobSQLTag[i].szAttribute1) > 0)
		{
			strcat(szCreateSql, " ");
			strcat(szCreateSql, (char*)&pobSQLTag[i].szAttribute1);
		}
		/* Tag 屬性2 */
		if (strlen((char*)&pobSQLTag[i].szAttribute2) > 0)
		{
			strcat(szCreateSql, " ");
			strcat(szCreateSql, (char*)&pobSQLTag[i].szAttribute2);
		}
        }
	
	/* 後綴 */
	strcat(szCreateSql, szSqlSuffix);
	
	/* 建立 Table */
	inRetVal = sqlite3_exec(srDBConnection, szCreateSql, 0, 0, &szErrorMessage);
	if (inRetVal != SQLITE_OK)
	{
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Create Table ERROR Num:%d", inRetVal);
                printf("%s\n", szDebugMsg);

                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Reason:%s", szErrorMessage);
                printf("%s\n", szDebugMsg);

		
		free(szCreateSql);
		if (szErrorMessage != NULL)
		{
			sqlite3_free(szErrorMessage);
		}
		
		/* 關閉 database, close null pointer 是NOP(No Operation) */
		if (inSqlite_DB_Close(&srDBConnection) != VS_SUCCESS)
		{
			/* 如果資料還在更新就close會因為SQLITE_BUSY而失敗，而且正在更新的事務也會roll back（回復上一動）*/
			return (VS_ERROR);
		}
		else
		{
			
                        memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                        sprintf(szDebugMsg, "Close Database OK");
                        printf("%s\n", szDebugMsg);
			
		}
		
		return (VS_ERROR);
	}
	else
	{
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Create Table OK");
                printf("%s\n", szDebugMsg);

		
		/* 釋放記憶體 */
		free(szCreateSql);
		
		/* 關閉 database, close null pointer 是NOP(No Operation) */
		if (inSqlite_DB_Close(&srDBConnection) != VS_SUCCESS)
		{
			/* 如果資料還在更新就close會因為SQLITE_BUSY而失敗，而且正在更新的事務也會roll back（回復上一動）*/
			return (VS_ERROR);
		}
		else
		{
			
                        memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                        sprintf(szDebugMsg, "Close Database OK");
                        printf("%s\n", szDebugMsg);
			
		}

	}
	
	return (VS_SUCCESS);
}

/*
Function        :inSqlite_Table_Link_BRec
Date&Time       :2017/3/13 上午 10:59
Describe        :將pobTran變數pointer位置放到Table中(用以解決每一個function都要放一個table的問題)
 *		 這邊直接把pobTran的pointer直接指到srAll(之後可能要考慮給動態記憶體)，TagName因為是寫在這個Function內的Table，所以要給實體位置儲存
*/
int inSqlite_Table_Link_BRec(TRANSACTION_OBJECT *pobTran, SQLITE_ALL_TABLE *srAll, int inLinkState)
{
	SQLITE_INT32T_TABLE TABLE_BATCH_INT[] = 
	{
		{0	,"inTableID"			,&pobTran->inTableID				},	/* inTableID */
		{0	,"inCode"			,&pobTran->srBRec.inCode			},	/* Trans Code */
		{0	,"inOrgCode"			,&pobTran->srBRec.inOrgCode			},	/* Original Trans Code  */
		{0	,"inPrintOption" 		,&pobTran->srBRec.inPrintOption			},	/* Print Option Flag */
		{0	,"inHDTIndex"			,&pobTran->srBRec.inHDTIndex			},	/* 紀錄HDTindex */
		{0	,"inCDTIndex"			,&pobTran->srBRec.inCDTIndex			},	/* 紀錄CDTindex */
		{0	,"inCPTIndex"			,&pobTran->srBRec.inCPTIndex			},	/* 紀錄CPTindex */
		{0	,"inTxnResult"			,&pobTran->srBRec.inTxnResult			},	/* 紀錄交易結果 */
		{0	,"inChipStatus"			,&pobTran->srBRec.inChipStatus			},	/* 0 NOT_USING_CHIP, 1 EMV_CARD, 2 EMV_EASY_ENTRY_CARD */
		{0	,"inFiscIssuerIDLength"		,&pobTran->srBRec.inFiscIssuerIDLength		},	/* 金融卡發卡單位代號長度 */
		{0	,"inFiscCardCommentLength"	,&pobTran->srBRec.inFiscCardCommentLength	},	/* 金融卡備註欄長度 */
		{0	,"inFiscAccountLength"		,&pobTran->srBRec.inFiscAccountLength		},	/* 金融卡帳號長度 */
		{0	,"inFiscSTANLength"		,&pobTran->srBRec.inFiscSTANLength		},	/* 金融卡交易序號長度 */
		{0	,"inESCTransactionCode"		,&pobTran->srBRec.inESCTransactionCode		},	/* ESC組ISO使用 重新上傳使用 Transaction Code沒辦法存在Batch */
		{0	,"inESCUploadMode"		,&pobTran->srBRec.inESCUploadMode		},	/* 標示支不支援ESC */
		{0	,"inESCUploadStatus"		,&pobTran->srBRec.inESCUploadStatus		},	/* 標示ESC上傳狀態 */
		{0	,"inSignStatus"			,&pobTran->srBRec.inSignStatus			},	/* 簽名檔狀態(有 免簽 或 Bypass) ESC電文使用 */
		{0	,"inHGCreditHostIndex"		,&pobTran->srBRec.inHGCreditHostIndex		},	/* 聯合_HAPPY_GO_信用卡主機 */
		{0	,"inHGCode"			,&pobTran->srBRec.inHGCode			},	/* 聯合_HAPPY_GO_交易碼 */
		
		{0	,"lnTxnAmount"			,&pobTran->srBRec.lnTxnAmount			},	/* The transaction amount, such as a SALE */
		{0	,"lnOrgTxnAmount"		,&pobTran->srBRec.lnOrgTxnAmount		},	/* The ORG transaction amount, such as a SALE */
		{0	,"lnTipTxnAmount"		,&pobTran->srBRec.lnTipTxnAmount		},	/* The transaction amount, such as a TIP */
		{0	,"lnAdjustTxnAmount"		,&pobTran->srBRec.lnAdjustTxnAmount		},	/* The transaction amount, such as a ADJUST */
		{0	,"lnTotalTxnAmount"		,&pobTran->srBRec.lnTotalTxnAmount		},	/* The transaction amount, such as a TOTAL */
		{0	,"lnInvNum"			,&pobTran->srBRec.lnInvNum			},	/* 調閱編號  */
		{0	,"lnOrgInvNum"			,&pobTran->srBRec.lnOrgInvNum			},	/* Original 調閱編號  */
		{0	,"lnBatchNum"			,&pobTran->srBRec.lnBatchNum			},	/* Batch Number */
		{0	,"lnOrgBatchNum" 		,&pobTran->srBRec.lnOrgBatchNum			},	/* Original Batch Number */
		{0	,"lnSTANNum"			,&pobTran->srBRec.lnSTANNum			},	/* Stan Number */
		{0	,"lnOrgSTANNum"			,&pobTran->srBRec.lnOrgSTANNum			},	/* Original Stan Number */
		{0	,"lnInstallmentPeriod"		,&pobTran->srBRec.lnInstallmentPeriod		},	/* 分期付款_期數 */
		{0	,"lnInstallmentDownPayment"	,&pobTran->srBRec.lnInstallmentDownPayment	},	/* 分期付款_頭期款 */
		{0	,"lnInstallmentPayment"		,&pobTran->srBRec.lnInstallmentPayment		},	/* 分期付款_每期款 */
		{0	,"lnInstallmentFormalityFee"	,&pobTran->srBRec.lnInstallmentFormalityFee	},	/* 分期付款_手續費 */
		{0	,"lnRedemptionPoints"		,&pobTran->srBRec.lnRedemptionPoints		},	/* 紅利扣抵_扣抵紅利點數 */
		{0	,"lnRedemptionPointsBalance"	,&pobTran->srBRec.lnRedemptionPointsBalance	},	/* 紅利扣抵_剩餘紅利點數 */
		{0	,"lnRedemptionPaidCreditAmount"	,&pobTran->srBRec.lnRedemptionPaidCreditAmount	},	/* 紅利扣抵_支付金額 */
		{0	,"lnHGTransactionType"		,&pobTran->srBRec.lnHGTransactionType		},	/* 聯合_HAPPY GO_交易類別 */
		{0	,"lnHGPaymentType"		,&pobTran->srBRec.lnHGPaymentType		},	/* 聯合_HAPPY_GO_支付工具 */
		{0	,"lnHGPaymentTeam"		,&pobTran->srBRec.lnHGPaymentTeam		},	/* 聯合_HAPPY_GO_支付工具_主機回_*/
		{0	,"lnHGBalancePoint"		,&pobTran->srBRec.lnHGBalancePoint		},	/* 聯合_HAPPY_GO_剩餘點數 */
		{0	,"lnHGTransactionPoint"		,&pobTran->srBRec.lnHGTransactionPoint		},	/* 聯合_HAPPY_GO_交易點數  合計 */
		{0	,"lnHGAmount"			,&pobTran->srBRec.lnHGAmount			},	/* 聯合_HAPPY_GO_扣抵後金額  (商品金額 = lnHGAmount + lnHGRedeemAmt) */
		{0	,"lnHGRedeemAmount"		,&pobTran->srBRec.lnHGRedeemAmount		},	/* 聯合_HAPPY_GO_扣抵金額 */
		{0	,"lnHGRefundLackPoint"		,&pobTran->srBRec.lnHGRefundLackPoint		},	/* 聯合_HAPPY_GO_不足點數 */
		{0	,"lnHGBatchIndex"		,&pobTran->srBRec.lnHGBatchIndex		},	/* 聯合_HAPPY_GO_主機當下批次號碼 */
		{0	,"lnHG_SPDH_OrgInvNum"		,&pobTran->srBRec.lnHG_SPDH_OrgInvNum		},	/* HAPPY_GO取消用INV */
		{0	,"lnHGSTAN"			,&pobTran->srBRec.lnHGSTAN			},	/* HAPPY_GO STAN */
		{0	,"lnCUPUPlanDiscountedAmount"	,&pobTran->srBRec.lnCUPUPlanDiscountedAmount	},	/* 銀聯優計畫折價後金額 */
		{0	,"lnCUPUPlanPreferentialAmount"	,&pobTran->srBRec.lnCUPUPlanPreferentialAmount	},	/* 銀聯優計畫優惠金額 */
		
		{0	,""				,NULL						}	/* 這行用Null用來知道尾端在哪 */
		
	};
	
	SQLITE_INT32T_TABLE TABLE_BATCH_INT64T[] = 
	{
		{0	,""				,NULL						}	/* 這行用Null用來知道尾端在哪 */
	};
	
	SQLITE_CHAR_TABLE TABLE_BATCH_CHAR[] =
	{
		{0	,"szAuthCode"                   ,pobTran->srBRec.szAuthCode			,strlen(pobTran->srBRec.szAuthCode)		},	/* Auth Code */
		{0	,"szMPASAuthCode"               ,pobTran->srBRec.szMPASAuthCode			,strlen(pobTran->srBRec.szMPASAuthCode)		},	/* MPAS Auth Code */
		{0	,"szRespCode"                   ,pobTran->srBRec.szRespCode			,strlen(pobTran->srBRec.szRespCode)		},	/* Response Code */
		{0	,"szStoreID"                    ,pobTran->srBRec.szStoreID			,strlen(pobTran->srBRec.szStoreID)		},	/* StoreID */
		{0	,"szCardLabel"                  ,pobTran->srBRec.szCardLabel			,strlen(pobTran->srBRec.szCardLabel)		},	/* 卡別  */
		{0	,"szPAN"                        ,pobTran->srBRec.szPAN				,strlen(pobTran->srBRec.szPAN)			},	/* 卡號  */
		{0	,"szDate"                       ,pobTran->srBRec.szDate				,strlen(pobTran->srBRec.szDate)			},	/* YYYYMMDD */
		{0	,"szOrgDate"                    ,pobTran->srBRec.szOrgDate			,strlen(pobTran->srBRec.szOrgDate)		},	/* YYYYMMDD */
		{0	,"szTime"                       ,pobTran->srBRec.szTime				,strlen(pobTran->srBRec.szTime)			},	/* HHMMSS */
		{0	,"szOrgTime"                    ,pobTran->srBRec.szOrgTime			,strlen(pobTran->srBRec.szOrgTime)		},	/* HHMMSS */
		{0	,"szCardTime"                   ,pobTran->srBRec.szCardTime			,strlen(pobTran->srBRec.szCardTime)		},	/* 晶片卡讀卡時間 , YYYYMMDDHHMMSS */
		{0	,"szRefNo"                      ,pobTran->srBRec.szRefNo			,strlen(pobTran->srBRec.szRefNo)		},	/* 序號  */
		{0	,"szExpDate"                    ,pobTran->srBRec.szExpDate			,strlen(pobTran->srBRec.szExpDate)		},	/* Expiration date */
		{0	,"szServiceCode"                ,pobTran->srBRec.szServiceCode			,strlen(pobTran->srBRec.szServiceCode)		},	/* Service code from track */
		{0	,"szCardHolder"                 ,pobTran->srBRec.szCardHolder			,strlen(pobTran->srBRec.szCardHolder)		},	/* 持卡人名字 */
		{0	,"szAMEX4DBC"                   ,pobTran->srBRec.szAMEX4DBC			,strlen(pobTran->srBRec.szAMEX4DBC)		},		
		{0	,"szFiscIssuerID"               ,pobTran->srBRec.szFiscIssuerID			,strlen(pobTran->srBRec.szFiscIssuerID)		},	/* 發卡單位代號 */
		{0	,"szFiscCardComment"            ,pobTran->srBRec.szFiscCardComment		,strlen(pobTran->srBRec.szFiscCardComment)	},	/* 金融卡備註欄 */
		{0	,"szFiscAccount"                ,pobTran->srBRec.szFiscAccount			,strlen(pobTran->srBRec.szFiscAccount)		},	/* 金融卡帳號 */
		{0	,"szFiscOutAccount"             ,pobTran->srBRec.szFiscOutAccount		,strlen(pobTran->srBRec.szFiscOutAccount)	},	/* 金融卡轉出帳號 */
		{0	,"szFiscSTAN"                   ,pobTran->srBRec.szFiscSTAN			,strlen(pobTran->srBRec.szFiscSTAN)		},	/* 金融卡交易序號 */
		{0	,"szFiscTacLength"              ,pobTran->srBRec.szFiscTacLength		,strlen(pobTran->srBRec.szFiscTacLength)	},	/* 金融卡Tac長度 */
		{0	,"szFiscTac"                    ,pobTran->srBRec.szFiscTac			,strlen(pobTran->srBRec.szFiscTac)		},	/* 金融卡Tac */
		{0	,"szFiscTCC"                    ,pobTran->srBRec.szFiscTCC			,strlen(pobTran->srBRec.szFiscTCC)		},	/* 端末設備查核碼 */
		{0	,"szFiscMCC"                    ,pobTran->srBRec.szFiscMCC			,strlen(pobTran->srBRec.szFiscMCC)		},	/* 金融卡MCC */
		{0	,"szFiscRRN"                    ,pobTran->srBRec.szFiscRRN			,strlen(pobTran->srBRec.szFiscRRN)		},	/* 金融卡調單編號 */
		{0	,"szFiscRefundDate"             ,pobTran->srBRec.szFiscRefundDate		,strlen(pobTran->srBRec.szFiscRefundDate)	},	/* 金融卡退貨原始交易日期(YYYYMMDD) */
		{0	,"szFiscDateTime"               ,pobTran->srBRec.szFiscDateTime			,strlen(pobTran->srBRec.szFiscDateTime)		},	/* 計算TAC(S2)的交易日期時間 */
		{0	,"szFiscPayDevice"              ,pobTran->srBRec.szFiscPayDevice		,strlen(pobTran->srBRec.szFiscPayDevice)	},	/* 金融卡付款裝置 1 = 手機 2 = 卡片 */
		{0	,"szFiscMobileDevice"           ,pobTran->srBRec.szFiscMobileDevice		,strlen(pobTran->srBRec.szFiscMobileDevice)	},	/* SE 類型，0x05：雲端卡片(Cloud-Based) */
		{0	,"szFiscMobileNFType"           ,pobTran->srBRec.szFiscMobileNFType		,strlen(pobTran->srBRec.szFiscMobileNFType)	},	/* 行動金融卡是否需輸入密碼 00不需要 01視情況 02一定要 */
		{0	,"szFiscMobileNFSetting"        ,pobTran->srBRec.szFiscMobileNFSetting		,strlen(pobTran->srBRec.szFiscMobileNFSetting)	},	/* 近端交易類型設定 0x00：Single Issuer Wallet 0x01：國內Third-Party Wallet 0x02~9：保留 0x0A：其他 */
		{0	,"szInstallmentIndicator"       ,pobTran->srBRec.szInstallmentIndicator		,strlen(pobTran->srBRec.szInstallmentIndicator)	},
		{0	,"szRedeemIndicator"            ,pobTran->srBRec.szRedeemIndicator		,strlen(pobTran->srBRec.szRedeemIndicator)	},
		{0	,"szRedeemSignOfBalance"        ,pobTran->srBRec.szRedeemSignOfBalance		,strlen(pobTran->srBRec.szRedeemSignOfBalance)	},
		{0	,"szHGCardLabel"                ,pobTran->srBRec.szHGCardLabel			,strlen(pobTran->srBRec.szHGCardLabel)		},	/* HAPPY_GO 卡別 */
		{0	,"szHGPAN"                      ,pobTran->srBRec.szHGPAN			,strlen(pobTran->srBRec.szHGPAN)		},	/* HAPPY_GO Account number */
		{0	,"szHGAuthCode"                 ,pobTran->srBRec.szHGAuthCode			,strlen(pobTran->srBRec.szHGAuthCode)		},	/* HAPPY_GO 授權碼 */
		{0	,"szHGRefNo"                    ,pobTran->srBRec.szHGRefNo			,strlen(pobTran->srBRec.szHGRefNo)		},	/* HAPPY_GO Reference Number */
		{0	,"szHGRespCode"                 ,pobTran->srBRec.szHGRespCode			,strlen(pobTran->srBRec.szHGRespCode)		},	/* HG Response Code */
		{0	,"szCUP_TN"                     ,pobTran->srBRec.szCUP_TN			,strlen(pobTran->srBRec.szCUP_TN)		},	/* CUP Trace Number (TN) */
		{0	,"szCUP_TD"                     ,pobTran->srBRec.szCUP_TD			,strlen(pobTran->srBRec.szCUP_TD)		},	/* CUP Transaction Date (MMDD) */
		{0	,"szCUP_TT"                     ,pobTran->srBRec.szCUP_TT			,strlen(pobTran->srBRec.szCUP_TT)		},	/* CUP Transaction Time (hhmmss) */
		{0	,"szCUP_RRN"                    ,pobTran->srBRec.szCUP_RRN			,strlen(pobTran->srBRec.szCUP_RRN)		},	/* CUP Retrieve Reference Number (CRRN) */
		{0	,"szCUP_STD"                    ,pobTran->srBRec.szCUP_STD			,strlen(pobTran->srBRec.szCUP_STD)		},	/* CUP Settlement Date(MMDD) Of Host Response */
		{0	,"szCUP_EMVAID"                 ,pobTran->srBRec.szCUP_EMVAID			,strlen(pobTran->srBRec.szCUP_EMVAID)		},	/* CUP晶片交易存AID帳單列印使用 */
		{0	,"szTranAbbrev"                 ,pobTran->srBRec.szTranAbbrev			,strlen(pobTran->srBRec.szTranAbbrev)		},	/* Tran abbrev for reports */
		{0	,"szIssueNumber"                ,pobTran->srBRec.szIssueNumber			,strlen(pobTran->srBRec.szIssueNumber)		},
		{0	,"szStore_DREAM_MALL"           ,pobTran->srBRec.szStore_DREAM_MALL		,strlen(pobTran->srBRec.szStore_DREAM_MALL	)	},	/* 存Dream_Mall Account Number And Member ID*/
		{0	,"szDCC_FCNFR"                  ,pobTran->srBRec.szDCC_FCNFR			,strlen(pobTran->srBRec.szDCC_FCNFR)		},	/* Foreign Currency No. For Rate */
		{0	,"szDCC_AC"                     ,pobTran->srBRec.szDCC_AC			,strlen(pobTran->srBRec.szDCC_AC)		},	/* Action Code */
		{0	,"szDCC_FCN"                    ,pobTran->srBRec.szDCC_FCN			,strlen(pobTran->srBRec.szDCC_FCN)		},	/* Foreign Currency Number */
		{0	,"szDCC_FCA"                    ,pobTran->srBRec.szDCC_FCA			,strlen(pobTran->srBRec.szDCC_FCA)		},	/* Foreign Currency Amount */
		{0	,"szDCC_FCMU"                   ,pobTran->srBRec.szDCC_FCMU			,strlen(pobTran->srBRec.szDCC_FCMU)		},	/* Foreign Currency Minor Unit */
		{0	,"szDCC_FCAC"                   ,pobTran->srBRec.szDCC_FCAC			,strlen(pobTran->srBRec.szDCC_FCAC)		},	/* Foreign currcncy Alphabetic Code */
		{0	,"szDCC_ERMU"                   ,pobTran->srBRec.szDCC_ERMU			,strlen(pobTran->srBRec.szDCC_ERMU)		},	/* Exchange Rate Minor Unit */
		{0	,"szDCC_ERV"                    ,pobTran->srBRec.szDCC_ERV			,strlen(pobTran->srBRec.szDCC_ERV)		},	/* Exchange Rate Value */
		{0	,"szDCC_IRMU"                   ,pobTran->srBRec.szDCC_IRMU			,strlen(pobTran->srBRec.szDCC_IRMU)		},	/* Inverted Rate Minor Unit */
		{0	,"szDCC_IRV"                    ,pobTran->srBRec.szDCC_IRV			,strlen(pobTran->srBRec.szDCC_IRV)		},	/* Inverted Rate Value */
		{0	,"szDCC_IRDU"                   ,pobTran->srBRec.szDCC_IRDU			,strlen(pobTran->srBRec.szDCC_IRDU)		},	/* Inverted Rate Display Unit */
		{0	,"szDCC_MPV"                    ,pobTran->srBRec.szDCC_MPV			,strlen(pobTran->srBRec.szDCC_MPV)		},	/* Markup Percentage Value */
		{0	,"szDCC_MPDP"                   ,pobTran->srBRec.szDCC_MPDP			,strlen(pobTran->srBRec.szDCC_MPDP)		},	/* Markup Percentage Decimal Point */
		{0	,"szDCC_CVCN"                   ,pobTran->srBRec.szDCC_CVCN			,strlen(pobTran->srBRec.szDCC_CVCN)		},	/* Commissino Value Currency Number */
		{0	,"szDCC_CVCA"                   ,pobTran->srBRec.szDCC_CVCA			,strlen(pobTran->srBRec.szDCC_CVCA)		},	/* Commission Value Currency Amount */
		{0	,"szDCC_CVCMU"                  ,pobTran->srBRec.szDCC_CVCMU			,strlen(pobTran->srBRec.szDCC_CVCMU)		},	/* Commission Value Currency Minor Unit */
		{0	,"szDCC_TIPFCA"                 ,pobTran->srBRec.szDCC_TIPFCA			,strlen(pobTran->srBRec.szDCC_TIPFCA)		},	/* Tip Foreign Currency Amount */
		{0	,"szDCC_OTD"			,pobTran->srBRec.szDCC_OTD			,strlen(pobTran->srBRec.szDCC_OTD)		},	/* Original Transaction Date & Time (MMDD) */
		{0	,"szDCC_OTA"			,pobTran->srBRec.szDCC_OTA			,strlen(pobTran->srBRec.szDCC_OTA)		},	/* Original Transaction Amount */
		{0	,"szProductCode"                ,pobTran->srBRec.szProductCode			,strlen(pobTran->srBRec.szProductCode)		},	/* 產品代碼 */
		{0	,"szAwardNum"			,pobTran->srBRec.szAwardNum			,strlen(pobTran->srBRec.szAwardNum)		},	/* 優惠個數 */
		{0	,"szAwardSN"			,pobTran->srBRec.szAwardSN			,strlen(pobTran->srBRec.szAwardSN)		},	/* 優惠序號(Award S/N) TID(8Bytes)+YYYYMMDDhhmmss(16 Bytes)，共22Bytes */
		{0	,"szTxnNo"			,pobTran->srBRec.szTxnNo			,strlen(pobTran->srBRec.szTxnNo)		},	/* 交易編號 */
		{0	,"szMCP_BANKID"			,pobTran->srBRec.szMCP_BANKID			,strlen(pobTran->srBRec.szMCP_BANKID)		},	/* 行動支付標記 金融機構代碼 */
		{0	,"szPayItemCode"                ,pobTran->srBRec.szPayItemCode			,strlen(pobTran->srBRec.szPayItemCode)		},	/* 繳費項目代碼 */
		{0	,"szTableTD_Data"		,pobTran->srBRec.szTableTD_Data			,strlen(pobTran->srBRec.szTableTD_Data)		},	/* Table TD的資料， */
		{0	,"szDFSTraceNum"                ,pobTran->srBRec.szDFSTraceNum			,strlen(pobTran->srBRec.szDFSTraceNum)		},	/* DFS交易系統追蹤號 */
		{0	,"szCheckNO"			,pobTran->srBRec.szCheckNO			,strlen(pobTran->srBRec.szCheckNO)		},	/* 檢查碼，目前FOR SDK使用 */
		{0	,"szEIVI_BANKID"                ,pobTran->srBRec.szEIVI_BANKID			,strlen(pobTran->srBRec.szEIVI_BANKID)		},	/* E Invoice vehicle I ssue Bank ID 支援電子發票載具之發卡行代碼 */
		{0	,"szBarCodeData"                ,pobTran->srBRec.szBarCodeData			,strlen(pobTran->srBRec.szBarCodeData)		},	/* 掃碼交易資料內容 */
		{0	,"szUnyTransCode"		,pobTran->srBRec.szUnyTransCode			,strlen(pobTran->srBRec.szUnyTransCode)		},	/* Uny交易碼 */
		{0	,"szUnyMaskedCardNo"		,pobTran->srBRec.szUnyMaskedCardNo		,strlen(pobTran->srBRec.szUnyMaskedCardNo)	},	/* 遮掩交易卡號資料(非完整卡號，有遮掩，故另設欄位) */
		{0	,"szEInvoiceHASH"		,pobTran->srBRec.szEInvoiceHASH			,strlen(pobTran->srBRec.szEInvoiceHASH)		},	/* 電子發票HASH 值（發卡銀行若無參加電子發票載具，則此欄位回覆值 為空白 SPACE 。） */
		{0	,"szUnyCardLabelLen"		,pobTran->srBRec.szUnyCardLabelLen		,strlen(pobTran->srBRec.szUnyCardLabelLen)	},	/* 卡別名稱長度，szMCP_BANKID為Twin卡專用，故另設欄位 */
		{0	,"szUnyCardLabel"		,pobTran->srBRec.szUnyCardLabel			,strlen(pobTran->srBRec.szUnyCardLabel)		},	/* 卡別名稱，szMCP_BANKID為Twin卡專用，故另設欄位 */		
		{0	,"szFPG_FTC_Invoice"		,pobTran->srBRec.szFPG_FTC_Invoice		,strlen(pobTran->srBRec.szFPG_FTC_Invoice)	},	/* 台塑生醫生技客製化專屬【005】　台亞福懋加油站客製化專屬【006】發票號碼 */
                {0	,"szUPlan_Coupon"               ,pobTran->srBRec.szUPlan_Coupon                 ,strlen(pobTran->srBRec.szUPlan_Coupon)         },	/* 銀聯優計畫的優惠訊息 */
                {0	,"szUPlan_RemarksInformation"   ,pobTran->srBRec.szUPlan_RemarksInformation     ,strlen(pobTran->srBRec.szUPlan_RemarksInformation)	},	/* 銀聯優計畫-備註訊息 */
                {0	,"szFlightTicketTransBit"       ,pobTran->srBRec.szFlightTicketTransBit         ,strlen(pobTran->srBRec.szFlightTicketTransBit)	},	/* 判斷是否是機票交易 不設定為空值 機票交易為1 非機票交易為2 */
                {0	,"szFlightTicketPDS0523"        ,pobTran->srBRec.szFlightTicketPDS0523          ,strlen(pobTran->srBRec.szFlightTicketPDS0523)	},	/* 出發地機場代碼（PDS 0523）左靠右補空白 */
                {0	,"szFlightTicketPDS0524"        ,pobTran->srBRec.szFlightTicketPDS0524          ,strlen(pobTran->srBRec.szFlightTicketPDS0524)	},	/* 目的地機場代碼（PDS 0524）左靠右補空白 */
                {0	,"szFlightTicketPDS0530"        ,pobTran->srBRec.szFlightTicketPDS0530          ,strlen(pobTran->srBRec.szFlightTicketPDS0530)	},	/* 航班號碼（PDS 0530） */
		
		{0	,"uszWAVESchemeID"		,&pobTran->srBRec.uszWAVESchemeID		,1						},	/* WAVE 使用用於組電文 Field_22 */
		{0	,"uszVOIDBit"			,&pobTran->srBRec.uszVOIDBit			,1						},	/* 負向交易 */
	        {0	,"uszUpload1Bit" 		,&pobTran->srBRec.uszUpload1Bit			,1						},	/* Offline交易使用 (原交易advice是否已上傳)*/
	        {0	,"uszUpload2Bit" 		,&pobTran->srBRec.uszUpload2Bit			,1						},	/* Offline交易使用 (當前交易是否為advice)*/
	        {0	,"uszUpload3Bit" 		,&pobTran->srBRec.uszUpload3Bit			,1						},	/* Offline交易使用 */
	        {0	,"uszReferralBit"		,&pobTran->srBRec.uszReferralBit		,1						},	/* ISO Response Code 【01】【02】使用 */
	        {0	,"uszOfflineBit" 		,&pobTran->srBRec.uszOfflineBit			,1						},	/* 離線交易 */
	        {0	,"uszManualBit"			,&pobTran->srBRec.uszManualBit			,1						},	/* Manual Keyin */
	        {0	,"uszNoSignatureBit"		,&pobTran->srBRec.uszNoSignatureBit		,1						},	/* 免簽名使用 (免簽名則為true)*/
		{0	,"uszCUPTransBit"		,&pobTran->srBRec.uszCUPTransBit		,1						},	/* 是否為CUP */
		{0	,"uszFiscTransBit"		,&pobTran->srBRec.uszFiscTransBit		,1						},	/* SmartPay交易，是否為金融卡 */
	        {0	,"uszContactlessBit"		,&pobTran->srBRec.uszContactlessBit		,1						},	/* 是否為非接觸式 */
	        {0	,"uszEMVFallBackBit"		,&pobTran->srBRec.uszEMVFallBackBit		,1						},	/* 是否要啟動fallback */
		{0	,"uszInstallmentbit"		,&pobTran->srBRec.uszInstallmentBit		,1						},	/* Installment */
		{0	,"uszRedeemBit"			,&pobTran->srBRec.uszRedeemBit			,1						},	/* Redemption */
	        {0	,"uszForceOnlineBit"		,&pobTran->srBRec.uszForceOnlineBit		,1						},	/* 組電文使用 Field_25 Point of Service Condition Code */
	        {0	,"uszMail_OrderBit"		,&pobTran->srBRec.uszMail_OrderBit		,1						},	/* 組電文使用 Field_25 Point of Service Condition Code */
		{0	,"uszDCCTransBit"		,&pobTran->srBRec.uszDCCTransBit		,1						},	/* 是否為DCC交易 */
		{0	,"uszNCCCDCCRateBit"		,&pobTran->srBRec.uszNCCCDCCRateBit		,1						},
		{0	,"uszCVV2Bit"			,&pobTran->srBRec.uszCVV2Bit			,1						},
		{0	,"uszRewardSuspendBit"		,&pobTran->srBRec.uszRewardSuspendBit		,1						},
		{0	,"uszRewardL1Bit"		,&pobTran->srBRec.uszRewardL1Bit		,1						},
		{0	,"uszRewardL2Bit"		,&pobTran->srBRec.uszRewardL2Bit		,1						},
		{0	,"uszRewardL5Bit"		,&pobTran->srBRec.uszRewardL5Bit		,1						},	/* 要印L5 */
		{0	,"uszField24NPSBit"		,&pobTran->srBRec.uszField24NPSBit		,1						},
		{0	,"uszVEPS_SignatureBit"		,&pobTran->srBRec.uszVEPS_SignatureBit		,1						},	/* VEPS 免簽名是否成立 */
		{0	,"uszTCUploadBit"		,&pobTran->srBRec.uszTCUploadBit		,1						},	/* TCUpload是否已上傳 */
		{0	,"uszFiscConfirmBit"		,&pobTran->srBRec.uszFiscConfirmBit		,1						},	/* SmartPay 0220 是否已上傳 */
		{0	,"uszFiscVoidConfirmBit"		,&pobTran->srBRec.uszFiscVoidConfirmBit		,1						},	/* SmartPay Void 0220 是否已上傳 */
		{0	,"uszPinEnterBit"		,&pobTran->srBRec.uszPinEnterBit		,1						},	/* 此筆交易是否有鍵入密碼(只能確認原交易，若該筆之後的調整或取消不會將此Bit Off) */
		{0	,"uszL2PrintADBit"		,&pobTran->srBRec.uszL2PrintADBit		,1						},	/* L2是否印AD，因L2原交易取消要判斷，只好增加 */
		{0	,"uszInstallment"		,&pobTran->srBRec.uszInstallment		,1						},	/* HappyGo分期交易 */
		{0	,"uszRedemption"   		,&pobTran->srBRec.uszRedemption			,1						},	/* HappyGo點數兌換 */ 
		{0	,"uszHappyGoSingle"		,&pobTran->srBRec.uszHappyGoSingle		,1						},	/* HappyGo交易 */ 
		{0	,"uszHappyGoMulti"		,&pobTran->srBRec.uszHappyGoMulti		,1						},	/* HappyGo混合交易 */
		{0	,"uszCLSBatchBit"		,&pobTran->srBRec.uszCLSBatchBit		,1						},	/* 是否已接續上傳 */
		{0	,"uszTxNoCheckBit"		,&pobTran->srBRec.uszTxNoCheckBit		,1						},	/* 商店自存聯卡號遮掩開關 */
		{0	,"uszSpecial00Bit"		,&pobTran->srBRec.uszSpecial00Bit		,1						},	/* 特殊卡別參數檔，活動代碼00表示免簽(只紀錄，主要看uszNoSignatureBit) */
		{0	,"uszSpecial01Bit"		,&pobTran->srBRec.uszSpecial01Bit		,1						},	/* 特殊卡別參數檔，活動代碼01表示ECR回傳明碼(先決條件ECR卡號遮掩有開才做判斷) */
		{0	,"uszRefundCTLSBit"		,&pobTran->srBRec.uszRefundCTLSBit		,1						},	/* 用在簽單印(W) 因為送電文contactless bit已OFF轉Manual Key in */
		{0	,"uszMPASTransBit"		,&pobTran->srBRec.uszMPASTransBit		,1						},	/* 標示為小額交易 */
		{0	,"uszMPASReprintBit"		,&pobTran->srBRec.uszMPASReprintBit		,1						},	/* 標示該小額交易是否可重印 */
		{0	,"uszMobilePayBit"		,&pobTran->srBRec.uszMobilePayBit		,1						},	/* 判斷是不是行動支付 Table NC */
		{0	,"uszUCARDTransBit"		,&pobTran->srBRec.uszUCARDTransBit		,1						},	/* 判斷是否是自由品牌卡 */
		{0	,"uszUnyTransBit"		,&pobTran->srBRec.uszUnyTransBit		,1						},	/* 是否是Uny交易 */
		{0	,"uszESCOrgUploadBit"		,&pobTran->srBRec.uszESCOrgUploadBit		,1						},	/* 用於標明原交易的ESC上傳狀態 */
		{0	,"uszESCTipUploadBit"		,&pobTran->srBRec.uszESCTipUploadBit		,1						},	/* 用於標明Tip的ESC上傳狀態 */
		{0	,"uszTakaTransBit"		,&pobTran->srBRec.uszTakaTransBit		,1						},	/* 客製化026、021 Taka社員卡交易 */
                {0	,"uszUPlanECRBit"		,&pobTran->srBRec.uszUPlanECRBit		,1						},	/* 判斷是否為優計畫ECR規格(用以決定是否回傳hash value) */
                {0	,"uszCUPEMVQRCodeBit"		,&pobTran->srBRec.uszCUPEMVQRCodeBit		,1						},	/* 判斷是否使用銀聯QRCode支付 */
                {0	,"uszUPlanTransBit"		,&pobTran->srBRec.uszUPlanTransBit		,1						},	/* 判斷是否為優計畫交易(使用銀聯優惠券) */	
		{0	,""				,NULL						,0						}	/* 這行用Null用來知道尾端在哪 */
	};

	
        printf("%s\n", "inSqlite_Table_Link_BRec()_START");
        
	SQLITE_LINK_TABLE	srLink;
	memset(&srLink, 0x00, sizeof(SQLITE_LINK_TABLE));
	srLink.psrInt = TABLE_BATCH_INT;
	srLink.psrInt64t = TABLE_BATCH_INT64T;
	srLink.psrChar = TABLE_BATCH_CHAR;
	srLink.psrText = NULL;
	//依據srLink的sqlite Table mapping到srAll上面
	inSqlite_Table_Link(srAll, inLinkState, &srLink);
	
        printf("%s\n", "inSqlite_Table_Link_BRec()_END");

	
	return (VS_SUCCESS);
}
/*
Function        :inSqlite_Table_Link
Date&Time       :2022/5/13 下午 5:17
Describe        :
*/
int inSqlite_Table_Link(SQLITE_ALL_TABLE *srAll, int inLinkState, SQLITE_LINK_TABLE* srLink)
{
	int		i = 0;
	char		szDebugMsg[100 + 1] = {0};
	unsigned char	uszFailBit = VS_FALSE;
	

        printf("%s\n", "inSqlite_Table_Link()_START");
        
	
	do
	{
		for (i = 0;; i++)
		{
			if (srLink->psrInt == NULL)
			{
				break;
			}
			
			/* 碰到Table底部，設定Tag數並跳出 */
			if (strlen(srLink->psrInt[i].szTag) == 0)
			{
				break;
			}

			/* pointer為空，則跳過 */
			if (srLink->psrInt[i].pTagValue == NULL)
				continue;

			/* 變數多過於原來設定的Tag數 */
			if (srAll->inIntNum == _TAG_INT_MAX_NUM_)
			{

                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                                sprintf(szDebugMsg, "Int變數過多");
                                printf("%s\n", szDebugMsg);
				
				return (VS_ERROR);
			}

			/* Tag 名稱過長 */
			if (strlen(srLink->psrInt[i].szTag) > _TAG_MAX_LENGRH_)
			{
			
                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                                sprintf(szDebugMsg, "Tag 名稱過長");
                                printf("%s\n", szDebugMsg);
				
				return (VS_ERROR);
			}

			/* Insert時不用塞TableID */
			if (inLinkState == _LS_INSERT_)
			{
				/* 判斷長度是因為避免相同字首比對錯誤 */
				if ((memcmp(srLink->psrInt[i].szTag, "inTableID", strlen("inTableID")) == 0) && (strlen(srLink->psrInt[i].szTag) == strlen("inTableID")))
				{
					continue;
				}
			}

			strcat(srAll->srInt[srAll->inIntNum].szTag, srLink->psrInt[i].szTag);
			srAll->srInt[srAll->inIntNum].pTagValue = srLink->psrInt[i].pTagValue;
			srAll->inIntNum++;
		}
		
		/* 有誤中止 */
		if (uszFailBit == VS_TRUE)
		{
			break;
		}

		for (i = 0;; i++)
		{
			if (srLink->psrInt64t == NULL)
			{
				break;
			}
			
			/* 碰到Table底部，設定Tag數並跳出 */
			if (strlen(srLink->psrInt64t[i].szTag) == 0)
			{
				break;
			}

			/* pointer為空，則跳過 */
			if (srLink->psrInt64t[i].pTagValue == NULL)
				continue;

			/* 變數多過於原來設定的Tag數 */
			if (srAll->inInt64tNum == _TAG_INT64T_MAX_NUM_)
			{
				
                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                                sprintf(szDebugMsg, "Int64t變數過多");
                                printf("%s\n", szDebugMsg);
				
				return (VS_ERROR);
			}

			/* Tag 名稱過長 */
			if (strlen(srLink->psrInt64t[i].szTag) > _TAG_MAX_LENGRH_)
			{
				
                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                                sprintf(szDebugMsg, "Tag 名稱過長");
                                printf("%s\n", szDebugMsg);
				
				return (VS_ERROR);
			}

			strcat(srAll->srInt64t[srAll->inInt64tNum].szTag, srLink->psrInt64t[i].szTag);
			srAll->srInt64t[srAll->inInt64tNum].pTagValue = srLink->psrInt64t[i].pTagValue;
			srAll->inInt64tNum++;
		}

		/* 有誤中止 */
		if (uszFailBit == VS_TRUE)
		{
			break;
		}
		
		for (i = 0;; i++)
		{
			if (srLink->psrChar == NULL)
			{
				break;
			}
			
			/* 碰到Table底部，設定Tag數並跳出 */
			if (strlen(srLink->psrChar[i].szTag) == 0)
			{
				break;
			}

			/* pointer為空，則跳過 */
			if (srLink->psrChar[i].pCharVariable == NULL)
				continue;

			/* 變數多過於原來設定的Tag數 */
			if (srAll->inCharNum == _TAG_CHAR_MAX_NUM_)
			{
                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                                sprintf(szDebugMsg, "Char變數過多");
                                printf("%s\n", szDebugMsg);
				
				return (VS_ERROR);
			}

			/* Tag 名稱過長 */
			if (strlen(srLink->psrChar[i].szTag) > _TAG_MAX_LENGRH_)
			{
				
                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                                sprintf(szDebugMsg, "Tag 名稱過長");
                                printf("%s\n", szDebugMsg);
				
				return (VS_ERROR);
			}

			strcat(srAll->srChar[srAll->inCharNum].szTag, srLink->psrChar[i].szTag);
			srAll->srChar[srAll->inCharNum].pCharVariable = srLink->psrChar[i].pCharVariable;
			srAll->srChar[srAll->inCharNum].inTagValueLen = srLink->psrChar[i].inTagValueLen;
			srAll->inCharNum++;
		}
		
		/* 有誤中止 */
		if (uszFailBit == VS_TRUE)
		{
			break;
		}
		
		for (i = 0;; i++)
		{
			if (srLink->psrText == NULL)
			{
				break;
			}
			
			/* 碰到Table底部，設定Tag數並跳出 */
			if (strlen(srLink->psrText[i].szTag) == 0)
			{
				break;
			}

			/* pointer為空，則跳過 */
			if (srLink->psrText[i].pCharVariable == NULL)
				continue;

			/* 變數多過於原來設定的Tag數 */
			if (srAll->inTextNum == _TAG_TEXT_MAX_NUM_)
			{
				
                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                                sprintf(szDebugMsg, "TEXT變數過多");
                                printf("%s\n", szDebugMsg);
				
				return (VS_ERROR);
			}

			/* Tag 名稱過長 */
			if (strlen(srLink->psrText[i].szTag) > _TAG_MAX_LENGRH_)
			{
				
                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                                sprintf(szDebugMsg, "Tag 名稱過長");
                                printf("%s\n", szDebugMsg);
				
				return (VS_ERROR);
			}

			strcat(srAll->srText[srAll->inTextNum].szTag, srLink->psrText[i].szTag);
			srAll->srText[srAll->inTextNum].pCharVariable = srLink->psrText[i].pCharVariable;
			srAll->srText[srAll->inTextNum].inTagValueLen = srLink->psrText[i].inTagValueLen;
			srAll->inTextNum++;
		}
		
		/* 有誤中止 */
		if (uszFailBit == VS_TRUE)
		{
			break;
		}
		break;
	}while(1);
	
        memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
        sprintf(szDebugMsg, "Table Link 成功");
        printf("%s\n", szDebugMsg);

	return (VS_SUCCESS);
}

/*
Function        :inSqlite_Insert_Record
Date&Time       :2022/5/10 上午 11:08
Describe        :Insert通用
*/
int inSqlite_Insert_Record(char* szDBPath, char* szTableName, SQLITE_ALL_TABLE* srAll)
{
	int			inRetVal;
	int			inSqlLength = 0;	/* 算組SQL語句的長度 */
	char			szDebugMsg[84 + 1] = {0};
	char			szSqlPrefix[100 + 1];	/* INSERT INTO	szTableName( */
	char			szSqlSuffix[20 + 1];	/* VALUES ( */
	char			szSqlSuffix2[10 + 1];	/* ); */
	char			*szInsertSql;
	sqlite3			*srDBConnection;	/* 建立到資料庫的connection */
	sqlite3_stmt		*srSQLStat;
	SQLITE_SQL_CALCULATE_TABLE	srSQLCal;
	
        printf("%s\n", "inSqlite_Insert_All_Batch()_START");

	
	inRetVal = inSqlite_DB_Open_Or_Create(szDBPath, &srDBConnection, _SQLITE_DEFAULT_FLAGS_, NULL);
	if (inRetVal != VS_SUCCESS)
	{	
		return (VS_ERROR);
	}
	
	/* 塞入SQL語句 */
	
	/* 為了使table name可變動，所以拉出來組 */
	memset(szSqlPrefix, 0x00, sizeof(szSqlPrefix));
	sprintf(szSqlPrefix, "INSERT INTO %s (", szTableName);
	
	memset(szSqlSuffix, 0x00, sizeof(szSqlSuffix));
	sprintf(szSqlSuffix, ")VALUES (");
	
	memset(szSqlSuffix2, 0x00, sizeof(szSqlSuffix2));
	sprintf(szSqlSuffix2, ");");
	
	/* 算要配置多少記憶體 */
	memset(&srSQLCal, 0x00, sizeof(SQLITE_SQL_CALCULATE_TABLE));
	srSQLCal.pSqlPrefix = szSqlPrefix;
	srSQLCal.pSqlSuffix = szSqlSuffix;
	srSQLCal.pSqlSuffix2 = szSqlSuffix2;
	inSqlite_Calculate_Insert_SQLLength(&srSQLCal, srAll, &inSqlLength);
	
	/* 配置記憶體 */
	szInsertSql = malloc(inSqlLength + 100);
	memset(szInsertSql, 0x00, inSqlLength + 100);
	
	inSqlite_Gernerate_InsertSQL(&srSQLCal, srAll, szInsertSql);
	
	/* prepare語句 */
	inRetVal = sqlite3_prepare_v2(srDBConnection, szInsertSql, -1, &srSQLStat, NULL);
	if (inRetVal != SQLITE_OK)
	{
		
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Prepare Fail: %d", inRetVal);
            printf("%s\n", szDebugMsg);

            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Reason: %s", sqlite3_errmsg(gsrDBConnection));
            printf("%s\n", szDebugMsg);
			
	}
	
	/* Binding變數 */
	inSqlite_Binding_Values(srAll, &srSQLStat);
	
	do
	{
		/* Insert */
		inRetVal = sqlite3_step(srSQLStat);
		if (inRetVal == SQLITE_ROW	||
		    inRetVal == SQLITE_DONE)
		{
			
                    memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                    sprintf(szDebugMsg, "Insert OK");
                    printf("%s\n", szDebugMsg);
			
		}
		else
		{

                    memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                    sprintf(szDebugMsg, "Insert ERROR Num:%d", inRetVal);
                    printf("%s\n", szDebugMsg);

                    memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                    sprintf(szDebugMsg, "Reason: %s", sqlite3_errmsg(srDBConnection));
                    printf("%s\n", szDebugMsg);

                    printf("%s\n", szInsertSql);
			

		}
		
	}while (inRetVal == SQLITE_ROW);
	
	/* 釋放事務，若要重用則用sqlite3_reset */
	inSqlite_SQL_Finalize(&srSQLStat);
	
	free(szInsertSql);
	
	/* 關閉 database, close null pointer 是NOP(No Operation) */
	if (inSqlite_DB_Close(&srDBConnection) != VS_SUCCESS)
	{
		/* 如果資料還在更新就close會因為SQLITE_BUSY而失敗，而且正在更新的事務也會roll back（回復上一動）*/
		return (VS_ERROR);
	}
	else
	{
		
            memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
            sprintf(szDebugMsg, "Close Database OK");
            printf("%s\n", szDebugMsg);
		
	}
	
	if (inRetVal == SQLITE_ERROR)
	{
		return (VS_ERROR);
	}
	
        printf("%s\n", "inSqlite_Insert_All_Batch()_END");
	
	return (VS_SUCCESS);
}
/*
Function        :inSqlite_Calculate_Insert_SQLLength
Date&Time       :2022/5/19 下午 1:58
Describe        :
*/
int inSqlite_Calculate_Insert_SQLLength(SQLITE_SQL_CALCULATE_TABLE* srSQLCal, SQLITE_ALL_TABLE* srAll, int* inSqlLength)
{
	int	i = 0;
	char	szDebugMsg[84 + 1] = {0};
	
	/* 算要配置多少記憶體 */
	if (srSQLCal->pSqlPrefix != NULL)
	{
		*inSqlLength += strlen(srSQLCal->pSqlPrefix);
	}
        printf("%s\n", szDebugMsg);

        memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
        sprintf(szDebugMsg, "IntTag: %d", srAll->inIntNum);
        printf("%s\n", szDebugMsg);

        memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
        sprintf(szDebugMsg, "Int64tTag: %d", srAll->inInt64tNum);
        printf("%s\n", szDebugMsg);

        memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
        sprintf(szDebugMsg, "CharTag: %d", srAll->inCharNum);
        printf("%s\n", szDebugMsg);

        memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
        sprintf(szDebugMsg, "TextTag: %d", srAll->inTextNum);
        printf("%s\n", szDebugMsg);
	
	
	for (i = 0; i < srAll->inIntNum; i++)
	{
		*inSqlLength += strlen(srAll->srInt[i].szTag);
		/* Comma */
		*inSqlLength += 2;
	}
	
	for (i = 0; i < srAll->inInt64tNum; i++)
	{
		*inSqlLength += strlen(srAll->srInt64t[i].szTag);
		/* Comma */
		*inSqlLength += 2;
	}
	
	for (i = 0; i < srAll->inCharNum; i++)
	{
		*inSqlLength += strlen(srAll->srChar[i].szTag);
		/* Comma */
		*inSqlLength += 2;
	}
	
	for (i = 0; i < srAll->inTextNum; i++)
	{
		*inSqlLength += strlen(srAll->srText[i].szTag);
		/* Comma */
		*inSqlLength += 2;
	}
	
	if (srSQLCal->pSqlSuffix != NULL)
	{
		/* 第一行最後面的) */
		(*inSqlLength) ++;
		/* 第二行"VALUES ("的長度 */
		*inSqlLength += strlen(srSQLCal->pSqlSuffix);
	}
	
	for (i = 0; i < srAll->inIntNum; i++)
	{
		/* 用問號 */
		*inSqlLength += 1;
		/* Comma & Space */
		*inSqlLength += 2;
	}
	
	for (i = 0; i < srAll->inInt64tNum; i++)
	{
		/* 用問號 */
		*inSqlLength += 1;
		/* Comma & Space */
		*inSqlLength += 2;
	}
	
	for (i = 0; i < srAll->inCharNum; i++)
	{
		/* 用問號 */
		*inSqlLength += 1;
		/* Comma & Space & 兩個單引號 */
		*inSqlLength += 2;
	}
	
	for (i = 0; i < srAll->inTextNum; i++)
	{
		/* 用問號 */
		*inSqlLength += 1;
		/* Comma & Space & 兩個單引號 */
		*inSqlLength += 2;
	}
	
	if (srSQLCal->pSqlSuffix2 != NULL)
	{
		/* ); */
		*inSqlLength += strlen(srSQLCal->pSqlSuffix2);
	}
	
	
        memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
        sprintf(szDebugMsg, "inSqlLength: %d", *inSqlLength);
        printf("%s\n", szDebugMsg);
	
	
	return (VS_SUCCESS);
}

/*
Function        :inSqlite_Gernerate_InsertSQL
Date&Time       :2022/5/19 下午 1:58
Describe        :
*/
int inSqlite_Gernerate_InsertSQL(SQLITE_SQL_CALCULATE_TABLE* srSQLCal, SQLITE_ALL_TABLE* srAll, char* szSQL)
{
	int	i = 0;
	char	szTemplate[100 + 1];
	char	szDebugMsg[84 + 1] = {0};
	
	if (srSQLCal->pSqlPrefix != NULL)
	{
		/* 先丟Table Name */
		strcat(szSQL, srSQLCal->pSqlPrefix);
	}
	
	if (srAll != NULL)
	{
		/* 看上一個table是不是空的，有東西的話，第一項前面要加comma，但這已經是第一個table，所以放0 */
		if (0 > 0)
		{
			strcat(szSQL, ", ");
		}

		for (i = 0; i < srAll->inIntNum; i++)
		{
			if (i > 0)
			{
				strcat(szSQL, ", ");
			}
			strcat(szSQL, srAll->srInt[i].szTag);
		}

		/* 看上一個table是不是空的，有東西的話，第一項前面要加comma */
		if (srAll->inIntNum > 0)
		{
			strcat(szSQL, ", ");
		}

		for (i = 0; i < srAll->inInt64tNum; i++)
		{
			if (i > 0)
			{
				strcat(szSQL, ", ");
			}
			strcat(szSQL, srAll->srInt64t[i].szTag);
		}

		/* 看上一個table是不是空的，有東西的話，第一項前面要加comma */
		if (srAll->inInt64tNum > 0)
		{
			strcat(szSQL, ", ");
		}

		for (i = 0; i < srAll->inCharNum; i++)
		{

			if (i > 0)
			{
				strcat(szSQL, ", ");
			}
			strcat(szSQL, srAll->srChar[i].szTag);
		}
		
		/* 看上一個table是不是空的，有東西的話，第一項前面要加comma */
		if (srAll->inCharNum > 0)
		{
			strcat(szSQL, ", ");
		}

		for (i = 0; i < srAll->inTextNum; i++)
		{

			if (i > 0)
			{
				strcat(szSQL, ", ");
			}
			strcat(szSQL, srAll->srText[i].szTag);
		}

		/* 為了避免有空的table導致多塞, */
		if (memcmp((szSQL + strlen(szSQL) - 2), ", ", 2) == 0)
		{
			memset(szSQL + strlen(szSQL) - 2, 0x00, 2);
		}
	}
	
	if (srSQLCal->pSqlSuffix != NULL)
	{
		/* ")VALUES (" */
		strcat(szSQL, srSQLCal->pSqlSuffix);
		memset(szTemplate, 0x00, sizeof(szTemplate));
	}
	
	if (srAll != NULL)
	{
		for (i = 0; i < srAll->inIntNum; i++)
		{
			if (i == 0)
			{
				sprintf(szTemplate, "?");
			}
			else
			{
				sprintf(szTemplate, ", ?");
			}
			strcat(szSQL, szTemplate);
			/* 只清自己用過得長度 comma(1) + space(1) + ?(1) */
			memset(szTemplate, 0x00, 3);
		}

		/* 代表上一個table有東西，要加comma */
		if (srAll->inIntNum > 0)
		{
			strcat(szSQL, ", ");
		}

		
                memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                sprintf(szDebugMsg, "Int Insert OK");
                printf("%s\n", szDebugMsg);
		

		for (i = 0; i < srAll->inInt64tNum; i++)
		{
			if (i == 0)
			{
				sprintf(szTemplate, "?");
			}
			else
			{
				sprintf(szTemplate, ", ?");
			}
			strcat(szSQL, szTemplate);
			/* 只清自己用過得長度 comma(1) + space(1) + ?(1) */
			memset(szTemplate, 0x00, 3);
		}

		/* 代表上一個table有東西，要加comma */
		if (srAll->inInt64tNum > 0)
		{
			strcat(szSQL, ", ");
		}

                memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                sprintf(szDebugMsg, "Int64t Insert OK");
                printf("%s\n", szDebugMsg);
		

		for (i = 0; i < srAll->inCharNum; i++)
		{
			if (i == 0)
			{
				sprintf(szTemplate, "?");
			}
			else
			{
				sprintf(szTemplate, ", ?");
			}
			strcat(szSQL, szTemplate);
			/* 只清自己用過得長度 comma(1) + space(1) + ?(1) */
			memset(szTemplate, 0x00, 3);
		}

		/* 代表上一個table有東西，要加comma */
		if (srAll->inCharNum > 0)
		{
			strcat(szSQL, ", ");
		}

		
                memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                sprintf(szDebugMsg, "Char Insert OK");
                printf("%s\n", szDebugMsg);
		

		for (i = 0; i < srAll->inTextNum; i++)
		{
			if (i == 0)
			{
				sprintf(szTemplate, "?");
			}
			else
			{
				sprintf(szTemplate, ", ?");
			}
			strcat(szSQL, szTemplate);
			/* 只清自己用過得長度 comma(1) + space(1) + ?(1) */
			memset(szTemplate, 0x00, 3);
		}

		/* 代表上一個table有東西，要加comma */
		if (srAll->inTextNum > 0)
		{
			strcat(szSQL, ", ");
		}

		
                memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                sprintf(szDebugMsg, "Text Insert OK");
                printf("%s\n", szDebugMsg);
		

		/* 為了避免有空的table導致多塞, */
		if (memcmp((szSQL + strlen(szSQL) - 2), ", ", 2) == 0)
		{
			memset(szSQL + strlen(szSQL) - 2, 0x00, 2);
		}
	}
	
	if (srSQLCal->pSqlSuffix2 != NULL)
	{
		/* 最後面的); */
		strcat(szSQL, srSQLCal->pSqlSuffix2);
	}
	
	return (VS_SUCCESS);
}
/*
Function        :inSqlite_Binding_Values
Date&Time       :2022/5/19 下午 2:36
Describe        :
*/
int inSqlite_Binding_Values(SQLITE_ALL_TABLE* srAll, sqlite3_stmt** srSQLStat)
{
	int	i = 0;
	int	inBindingIndex = 1;	/* binding的index從1開始 */
	int	inRetVal = VS_SUCCESS;
	char	szDebugMsg[84 + 1] = {0};
	
	/* Binding變數 */
	for (i = 0; i < srAll->inIntNum; i++)
	{
		inRetVal = sqlite3_bind_int(*srSQLStat, inBindingIndex, *(int32_t*)srAll->srInt[i].pTagValue);
		if (inRetVal != SQLITE_OK)
		{
			
                    memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                    sprintf(szDebugMsg, "Binging Int Fail: %d", inRetVal);
                    printf("%s\n", szDebugMsg);
			
		}
		else
		{
			inBindingIndex++;
		}
		
	}
	
	for (i = 0; i < srAll->inInt64tNum; i++)
	{
		inRetVal = sqlite3_bind_int64(*srSQLStat, inBindingIndex, *(int64_t*)srAll->srInt64t[i].pTagValue);
		if (inRetVal != SQLITE_OK)
		{
			
                        memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                        sprintf(szDebugMsg, "Binging Int64t Fail: %d", inRetVal);
                        printf("%s\n", szDebugMsg);
			
		}
		else
		{
			inBindingIndex++;
		}
	}
	
	for (i = 0; i < srAll->inCharNum; i++)
	{
		inRetVal = sqlite3_bind_blob(*srSQLStat, inBindingIndex, srAll->srChar[i].pCharVariable, srAll->srChar[i].inTagValueLen, NULL);
		if (inRetVal != SQLITE_OK)
		{
			
                        memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                        sprintf(szDebugMsg, "Binging Char Fail: %d", inRetVal);
                        printf("%s\n", szDebugMsg);
			
		}
		else
		{
			inBindingIndex++;
		}
	}
	
	for (i = 0; i < srAll->inTextNum; i++)
	{
		inRetVal = sqlite3_bind_text(*srSQLStat, inBindingIndex, srAll->srText[i].pCharVariable, srAll->srText[i].inTagValueLen, NULL);
		if (inRetVal != SQLITE_OK)
		{
			
                        memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                        sprintf(szDebugMsg, "Binging Text Fail: %d", inRetVal);
                        printf("%s\n", szDebugMsg);
			
		}
		else
		{
			inBindingIndex++;
		}
	}
	
	return (VS_SUCCESS);
}

/*
Function        :inSqlite_Get_Max_TableID
Date&Time       :2017/3/15 下午 1:29
Describe        :抓最大的Primary Key值
*/
int inSqlite_Get_Max_TableID(char* szDBPath, char* szTableName, char *szTagValue)
{
	int		j = 0;
	int		inDataLen = 0;
	int		inRetVal = VS_ERROR;
	char		szDebugMsg[84 + 1];
	char		szQuerySql[_SQL_SENTENCE_MAX_ + 1];	/* INSERT INTO	szTableName( */
	sqlite3		*srDBConnection;	/* 建立到資料庫的connection */
	sqlite3_stmt	*srSQLStat;
	
	
        printf("%s\n", "inSqlite_Get_Max_TableID()_START");
        
	
	inRetVal = inSqlite_DB_Open_Or_Create(szDBPath, &srDBConnection, _SQLITE_DEFAULT_FLAGS_, NULL);
	
	if (inRetVal != SQLITE_OK)
	{
		
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "%d", inRetVal);
                printf("%s\n", szDebugMsg);
		
		
		return (VS_ERROR);
	}
	else
	{
		
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Open Database File OK");
                printf("%s\n", szDebugMsg);
		
	}
	
	/* 塞入SQL語句 */
	
	/* 為了使table name可變動，所以拉出來組 */
	memset(szQuerySql, 0x00, sizeof(szQuerySql));
	sprintf(szQuerySql, "SELECT MAX(%s) FROM %s", "inTableID", szTableName);
	
	/* prepare語句 */
	inRetVal = sqlite3_prepare_v2(srDBConnection, szQuerySql, -1, &srSQLStat, NULL);
	if (inRetVal != SQLITE_OK)
	{
		
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Prepare Fail: %d", inRetVal);
            printf("%s\n", szDebugMsg);

            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Reason: %s", sqlite3_errmsg(gsrDBConnection));
            printf("%s\n", szDebugMsg);
		
	}
	
	/* 取得 database 裡所有的資料 */
	/* Qerry */
	inRetVal = sqlite3_step(srSQLStat);
	if (inRetVal == SQLITE_ROW)
	{
		
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Get Table OK");
            printf("%s\n", szDebugMsg);
		
	}
	else if (inRetVal == SQLITE_DONE)
	{
		
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "NO DATA");
                printf("%s\n", szDebugMsg);
		
		
		/* 釋放事務 */
		inSqlite_SQL_Finalize(&srSQLStat);
		inSqlite_DB_Close(&srDBConnection);
		
		return (VS_NO_RECORD);
	}
	else
	{
		
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Get Table ERROR Num:%d", inRetVal);
                printf("%s\n", szDebugMsg);

                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Reason: %s", sqlite3_errmsg(srDBConnection));
                printf("%s\n", szDebugMsg);

                printf("%s\n", szQuerySql);
		

		/* 釋放事務 */
		inSqlite_SQL_Finalize(&srSQLStat);
		
		/* 關閉 database, close null pointer 是NOP(No Operation) */
		if (inSqlite_DB_Close(&srDBConnection) != VS_SUCCESS)
		{
			/* 如果資料還在更新就close會因為SQLITE_BUSY而失敗，而且正在更新的事務也會roll back（回復上一動）*/
			return (VS_ERROR);
		}
		else
		{
			
                        memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                        sprintf(szDebugMsg, "Close Database OK");
                        printf("%s\n", szDebugMsg);
			
		}

		return (VS_ERROR);
	}
	
	/* 若是成功，將值丟到輸入的位置。 */
	j = 0;
	inDataLen = sqlite3_column_bytes(srSQLStat, j);
	memcpy(szTagValue, sqlite3_column_blob(srSQLStat, j), inDataLen);
	
	/* 釋放事務 */
	inSqlite_SQL_Finalize(&srSQLStat);
	
	/* 關閉 database, close null pointer 是NOP(No Operation) */
	if (inSqlite_DB_Close(&srDBConnection) != VS_SUCCESS)
	{
		/* 如果資料還在更新就close會因為SQLITE_BUSY而失敗，而且正在更新的事務也會roll back（回復上一動）*/
		return (VS_ERROR);
	}
	else
	{
		
                memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                sprintf(szDebugMsg, "Close Database OK");
                printf("%s\n", szDebugMsg);
		
	}
	
        printf("%s\n", "inSqlite_Get_Max_TableID()_END");

	return (VS_SUCCESS);
}

/*
Function        :inSqlite_Get_Data_By_External_SQL
Date&Time       :2019/5/28 下午 2:22
Describe        :嘗試不將SQL寫死達到重複使用的效果
*/
int inSqlite_Get_Data_By_External_SQL(char* szDBName, SQLITE_ALL_TABLE* srAll, char* szSQLStatement)
{
	int			inRetVal = VS_SUCCESS;
	char			szDebugMsg[128 + 1];
	char			szQuerySql[_SQL_SENTENCE_MAX_ + 1];	/* INSERT INTO	szTableName( */
	char			szErrorMessage[100 + 1];
	sqlite3			*srDBConnection;	/* 建立到資料庫的connection */
	sqlite3_stmt		*srSQLStat;

        printf("%s\n", "inSqlite_Get_Table_ByRecordID_All()_START");
        
	
	inRetVal = inSqlite_DB_Open_Or_Create(szDBName, &srDBConnection, _SQLITE_DEFAULT_FLAGS_, NULL);
	
	if (inRetVal != SQLITE_OK)
	{
		
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "%d", inRetVal);
                printf("%s\n", szDebugMsg);
		
		return (VS_ERROR);
	}
	else
	{
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Open Database File OK");
                printf("%s\n", szDebugMsg);
		
	}
	
	/* 塞入SQL語句 */
	memset(szQuerySql, 0x00, sizeof(szQuerySql));
	strcpy(szQuerySql, szSQLStatement);

	/* prepare語句 */
	inRetVal = sqlite3_prepare_v2(srDBConnection, szQuerySql, -1, &srSQLStat, NULL);
	if (inRetVal != SQLITE_OK)
	{
		
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Prepare Fail: %d", inRetVal);
                printf("%s\n", szDebugMsg);

                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Reason: %s", sqlite3_errmsg(gsrDBConnection));
                printf("%s\n", szDebugMsg);
		
	}
	
	/* 取得 database 裡所有的資料 */
	/* Qerry */
	inRetVal = sqlite3_step(srSQLStat);
	if (inRetVal == SQLITE_ROW)
	{
		
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Get Table OK");
                printf("%s\n", szDebugMsg);
		
	}
	else if (inRetVal == SQLITE_DONE)
	{
		
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "SQLITE_DONE");
                printf("%s\n", szDebugMsg);
		
		/* 釋放事務 */
		inSqlite_SQL_Finalize(&srSQLStat);
		inSqlite_DB_Close(&srDBConnection);
		
		return (VS_NO_RECORD);
	}
	else
	{
		memset(szErrorMessage, 0x00, sizeof(szErrorMessage));
		strcpy(szErrorMessage, sqlite3_errmsg(srDBConnection));
			
		
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Get Table ERROR Num:%d", inRetVal);
                printf("%s\n", szDebugMsg);

                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Reason: %s", szErrorMessage);
                printf("%s\n", szDebugMsg);

                printf("%s\n", szQuerySql);
		

		/* 釋放事務 */
		inSqlite_SQL_Finalize(&srSQLStat);
		
		/* 關閉 database, close null pointer 是NOP(No Operation) */
		if (inSqlite_DB_Close(&srDBConnection) != VS_SUCCESS)
		{
			/* 如果資料還在更新就close會因為SQLITE_BUSY而失敗，而且正在更新的事務也會roll back（回復上一動）*/
			return (VS_ERROR);
		}
		else
		{
			
                        memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                        sprintf(szDebugMsg, "Close Database OK");
                        printf("%s\n", szDebugMsg);
			
		}

		/* 因為直接get 不存在的table回傳值是-1，只有在Error Msg才能得知錯誤原因 */
		if (memcmp(szErrorMessage, "no such table", strlen("no such table")) == 0)
		{
			return (VS_NO_RECORD);
		}
		else
		{
			return (VS_ERROR);
		}
	}
	
	/* binding 取得值 */
	inSqlite_Get_Binding_Value(&srSQLStat, srAll);
	
	/* 釋放事務 */
	sqlite3_finalize(srSQLStat);
	
	/* 關閉 database, close null pointer 是NOP(No Operation) */
	if (inSqlite_DB_Close(&srDBConnection) != VS_SUCCESS)
	{
		/* 如果資料還在更新就close會因為SQLITE_BUSY而失敗，而且正在更新的事務也會roll back（回復上一動）*/
		return (VS_ERROR);
	}
	else
	{
		
            memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
            sprintf(szDebugMsg, "Close Database OK");
            printf("%s\n", szDebugMsg);
		
	}
        printf("%s\n", "inSqlite_Get_Table_ByRecordID_All()_END");
               
	return (VS_SUCCESS);
}
/*
Function        :inSqlite_Get_Binding_Value
Date&Time       :2019/6/3 下午 5:19
Describe        :取得查詢後的結果
*/
int inSqlite_Get_Binding_Value(sqlite3_stmt** srSQLStat, SQLITE_ALL_TABLE* srAll)
{
	int	j = 0;
	int	inCols = 0;
	int	inFind = VS_FALSE;
	int	inIntIndex = 0, inInt64tIndex = 0, inCharIndex = 0, inTextIndex = 0;
	int	inDataLen = 0;
	char	szTagName[_TAG_WIDTH_ + 1] = {0};
	char	szDebugMsg[128 + 1] = {0};
	
	inCols = sqlite3_column_count(*srSQLStat);
	
	/* binding 取得值 */
	/*看起來是取得Query結果後，For Loop比對每個Table的TagName塞值*/
        for (j = 0; j < inCols; j++)
	{
		inFind = VS_FALSE;
		memset(szTagName, 0x00, sizeof(szTagName));
		strcat(szTagName, sqlite3_column_name(*srSQLStat, j));
		
		
		for (inIntIndex = 0; inIntIndex < srAll->inIntNum; inIntIndex++)
		{
			if (srAll->srInt[inIntIndex].uszIsFind == VS_TRUE)
			{
				continue;
			}
			
			
			/* 比對Tag Name */
			if (memcmp(szTagName, srAll->srInt[inIntIndex].szTag, strlen(srAll->srInt[inIntIndex].szTag)) == 0	&&
			    strlen(szTagName) == strlen(srAll->srInt[inIntIndex].szTag))
			{
				*(int32_t*)srAll->srInt[inIntIndex].pTagValue = sqlite3_column_int(*srSQLStat, j);
				srAll->srInt[inIntIndex].uszIsFind = VS_TRUE;
				inFind = VS_TRUE;

				break;
			}

		}
		
		/* inFind == VS_TRUE表示找到了，跳下一回 */
		if (inFind == VS_TRUE)
		{
			continue;
		}

		for (inInt64tIndex = 0; inInt64tIndex < srAll->inInt64tNum; inInt64tIndex++)
		{
			if (srAll->srInt64t[inInt64tIndex].uszIsFind == VS_TRUE)
			{
				continue;
			}

			/* 比對Tag Name 所以列恆為0 */
			if (memcmp(szTagName, srAll->srInt64t[inInt64tIndex].szTag, strlen(srAll->srInt64t[inInt64tIndex].szTag)) == 0	&&
			    strlen(szTagName) == strlen(srAll->srInt64t[inInt64tIndex].szTag))
			{
				*(int64_t*)srAll->srInt64t[inInt64tIndex].pTagValue = sqlite3_column_int64(*srSQLStat, j);
				srAll->srInt64t[inInt64tIndex].uszIsFind = VS_TRUE;
				inFind = VS_TRUE;

				break;
			}
			
		}

		/* inFind == VS_TRUE表示找到了，跳下一回 */
		if (inFind == VS_TRUE)
		{
			continue;
		}
		
		for (inCharIndex = 0; inCharIndex < srAll->inCharNum; inCharIndex++)
		{
			if (srAll->srChar[inCharIndex].uszIsFind == VS_TRUE)
			{
				continue;
			}
			
			/* 比對Tag Name 所以列恆為0 */
			if (memcmp(szTagName, srAll->srChar[inCharIndex].szTag, strlen(srAll->srChar[inCharIndex].szTag)) == 0	&&
			    strlen(szTagName) == strlen(srAll->srChar[inCharIndex].szTag))
			{
				inDataLen = sqlite3_column_bytes(*srSQLStat, j);
				/* 放之前先清空 */
				memset(srAll->srChar[inCharIndex].pCharVariable, 0x00, inDataLen + 1);
				memcpy(srAll->srChar[inCharIndex].pCharVariable, sqlite3_column_blob(*srSQLStat, j), inDataLen);
				srAll->srChar[inCharIndex].uszIsFind = VS_TRUE;
				inFind = VS_TRUE;

				break;
			}

		}
		
		/* inFind == VS_TRUE表示找到了，跳下一回 */
		if (inFind == VS_TRUE)
		{
			continue;
		}

		for (inTextIndex = 0; inTextIndex < srAll->inTextNum; inTextIndex++)
		{
			if (srAll->srText[inTextIndex].uszIsFind == VS_TRUE)
			{
				continue;
			}
			
			/* 比對Tag Name 所以列恆為0 */
			if (memcmp(szTagName, srAll->srText[inTextIndex].szTag, strlen(srAll->srText[inTextIndex].szTag)) == 0	&&
			    strlen(szTagName) == strlen(srAll->srText[inTextIndex].szTag))
			{
				inDataLen = sqlite3_column_bytes(*srSQLStat, j);
				/* 放之前先清空 */
				memset(srAll->srText[inTextIndex].pCharVariable, 0x00, inDataLen + 1);
				memcpy(srAll->srText[inTextIndex].pCharVariable, sqlite3_column_text(*srSQLStat, j), inDataLen);
				srAll->srText[inTextIndex].uszIsFind = VS_TRUE;
				inFind = VS_TRUE;

				break;
			}

		}
		
		/* inFind == VS_TRUE表示找到了，跳下一回 */
		if (inFind == VS_TRUE)
		{
			continue;
		}
	
                memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                sprintf(szDebugMsg, "Find no variable to insert:Tag: %s", szTagName);
                printf("%s\n", szDebugMsg);

                memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                sprintf(szDebugMsg, "Value: %s", (char*)sqlite3_column_blob(*srSQLStat, j));
                printf("%s\n", szDebugMsg);
			
	}
	return (VS_SUCCESS);
}


/*
Function        :inSqlite_Table_Show
Date&Time       :
Describe        :show出srAll的table欄位 和 欄位值
*/
int inSqlite_Table_Show(SQLITE_ALL_TABLE *srAll)
{
	int		i = 0;
	char		szDebugMsg[100 + 1] = {0};
	unsigned char	uszFailBit = VS_FALSE;
	char header_buf[4096];
        char value_buf[4096];
        memset(header_buf,0x00,BUFF_SIZE);
        memset(value_buf ,0x00,BUFF_SIZE);
        printf("%s\n", "inSqlite_Table_Show()_START");
        int header_len = 0;
        int value_len  = 0;
	int cntln = 1;
	do
	{
		for (i = 0;; i++)
		{
			if (srAll->srInt == NULL)
			{
				break;
			}
			
			/* 碰到Table底部，設定Tag數並跳出 */
			if (strlen(srAll->srInt[i].szTag) == 0)
			{
				break;
			}

			/* pointer為空，則跳過 */
			if (srAll->srInt[i].pTagValue == NULL)
				continue;

			/* 變數多過於原來設定的Tag數 */
//			if (srAll->inIntNum == _TAG_INT_MAX_NUM_)
//			{
//
//                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//                                sprintf(szDebugMsg, "Int變數過多");
//                                printf("%s\n", szDebugMsg);
//				
//				return (VS_ERROR);
//			}

			/* Tag 名稱過長 */
			if (strlen(srAll->srInt[i].szTag) > _TAG_MAX_LENGRH_)
			{
			
                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                                sprintf(szDebugMsg, "Tag 名稱過長");
                                printf("%s\n", szDebugMsg);
				
				return (VS_ERROR);
			}

			/* Insert時不用塞TableID */
//			if (inLinkState == _LS_INSERT_)
//			{
//				/* 判斷長度是因為避免相同字首比對錯誤 */
//				if ((memcmp(srLink->psrInt[i].szTag, "inTableID", strlen("inTableID")) == 0) && (strlen(srLink->psrInt[i].szTag) == strlen("inTableID")))
//				{
//					continue;
//				}
//			}
                    header_len += snprintf(header_buf + header_len,
                               BUFF_SIZE - header_len,
                               "%-30s",
                               srAll->srInt[i].szTag);
                    value_len += snprintf(value_buf + value_len,
                                            BUFF_SIZE - value_len,
                                            "%-30d",
                                            *(int32_t*)srAll->srInt[i].pTagValue);
                    cntln++;

                    if((cntln-1)% 4 == 0)
                    {
                        printf("%s\n",header_buf);
                        printf("%s\n",value_buf);
                        printf("----------------------------------------------------------------------------------------------------------------------\n");
                        memset(header_buf,0x00,BUFF_SIZE);
                        memset(value_buf ,0x00,BUFF_SIZE);
                        header_len = value_len = 0;
                   
                    }
//			strcat(srAll->srInt[srAll->inIntNum].szTag, srLink->psrInt[i].szTag);
//			srAll->srInt[srAll->inIntNum].pTagValue = srLink->psrInt[i].pTagValue;
//			srAll->inIntNum++;
		}
		
		/* 有誤中止 */
		if (uszFailBit == VS_TRUE)
		{
			break;
		}
                cntln = 1;
                if(header_len !=0)
                {
                    printf("%s\n",header_buf);
                    printf("%s\n",value_buf);
                    printf("----------------------------------------------------------------------------------------------------------------------\n");
                    memset(header_buf,0x00,BUFF_SIZE);
                    memset(value_buf ,0x00,BUFF_SIZE);
                    header_len = value_len = 0;
                }
                printf("\n\n\n");
		for (i = 0;; i++)
		{
			if (srAll->srInt64t == NULL)
			{
				break;
			}
			
			/* 碰到Table底部，設定Tag數並跳出 */
			if (strlen(srAll->srInt64t[i].szTag) == 0)
			{
				break;
			}

			/* pointer為空，則跳過 */
			if (srAll->srInt64t[i].pTagValue == NULL)
				continue;

			/* 變數多過於原來設定的Tag數 */
//			if (srAll->inInt64tNum == _TAG_INT64T_MAX_NUM_)
//			{
//				
//                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//                                sprintf(szDebugMsg, "Int64t變數過多");
//                                printf("%s\n", szDebugMsg);
//				
//				return (VS_ERROR);
//			}

			/* Tag 名稱過長 */
			if (strlen(srAll->srInt64t[i].szTag) > _TAG_MAX_LENGRH_)
			{
				
                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                                sprintf(szDebugMsg, "Tag 名稱過長");
                                printf("%s\n", szDebugMsg);
				
				return (VS_ERROR);
			}
                    header_len += snprintf(header_buf + header_len,
                               BUFF_SIZE - header_len,
                               "%-30s",
                               srAll->srInt64t[i].szTag);
                    value_len += snprintf(value_buf + value_len,
                                BUFF_SIZE - value_len,
                                "%-30lld",
                                *(int64_t*)srAll->srInt64t[i].pTagValue);
                    cntln++;

                    if((cntln-1)% 4 == 0)
                    {
                        printf("%s\n",header_buf);
                        printf("%s\n",value_buf);
                        printf("----------------------------------------------------------------------------------------------------------------------\n");
                        memset(header_buf,0x00,BUFF_SIZE);
                        memset(value_buf,0x00,BUFF_SIZE);
                        header_len = value_len = 0;
                   
                    }
//			strcat(srAll->srInt64t[srAll->inInt64tNum].szTag, srLink->psrInt64t[i].szTag);
//			srAll->srInt64t[srAll->inInt64tNum].pTagValue = srLink->psrInt64t[i].pTagValue;
//			srAll->inInt64tNum++;
		}

		/* 有誤中止 */
		if (uszFailBit == VS_TRUE)
		{
			break;
		}
		cntln = 1;
                if(header_len !=0)
                {
                    printf("%s\n",header_buf);
                    printf("%s\n",value_buf);
                    printf("----------------------------------------------------------------------------------------------------------------------\n");
                    memset(header_buf,0x00,BUFF_SIZE);
                    memset(value_buf,0x00,BUFF_SIZE);
                    header_len = value_len = 0;
                }
                printf("\n\n\n");
		for (i = 0;; i++)
		{
			if (srAll->srChar == NULL)
			{
				break;
			}
			
			/* 碰到Table底部，設定Tag數並跳出 */
			if (strlen(srAll->srChar[i].szTag) == 0)
			{
				break;
			}

			/* pointer為空，則跳過 */
			if (srAll->srChar[i].pCharVariable == NULL)
				continue;

			/* 變數多過於原來設定的Tag數 */
//			if (srAll->inCharNum == _TAG_CHAR_MAX_NUM_)
//			{
//                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//                                sprintf(szDebugMsg, "Char變數過多");
//                                printf("%s\n", szDebugMsg);
//				
//				return (VS_ERROR);
//			}

			/* Tag 名稱過長 */
			if (strlen(srAll->srChar[i].szTag) > _TAG_MAX_LENGRH_)
			{
				
                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                                sprintf(szDebugMsg, "Tag 名稱過長");
                                printf("%s\n", szDebugMsg);
				
				return (VS_ERROR);
			}
                        header_len += snprintf(header_buf + header_len,
                                   BUFF_SIZE - header_len,
                                   "%-30s",
                                   srAll->srChar[i].szTag);
                        value_len += snprintf(value_buf + value_len,
                                    BUFF_SIZE - value_len,
                                    "%-30s",
                                    (char *)srAll->srChar[i].pCharVariable);
                        cntln++;

                        if((cntln-1)% 4 == 0)
                        {
                            printf("%s\n",header_buf);
                            printf("%s\n",value_buf);
                            printf("----------------------------------------------------------------------------------------------------------------------\n");
                            memset(header_buf,0x00,BUFF_SIZE);
                            memset(value_buf,0x00,BUFF_SIZE);
                            header_len = value_len = 0;

                        } 
//			strcat(srAll->srChar[srAll->inCharNum].szTag, srLink->psrChar[i].szTag);
//			srAll->srChar[srAll->inCharNum].pCharVariable = srLink->psrChar[i].pCharVariable;
//			srAll->srChar[srAll->inCharNum].inTagValueLen = srLink->psrChar[i].inTagValueLen;
//			srAll->inCharNum++;
		}
		
		/* 有誤中止 */
		if (uszFailBit == VS_TRUE)
		{
			break;
		}
                cntln = 1;
                if(header_len !=0)
                {
                    printf("%s\n",header_buf);
                    printf("%s\n",value_buf);
                    printf("----------------------------------------------------------------------------------------------------------------------\n");
                    memset(header_buf,0x00,BUFF_SIZE);
                    memset(value_buf,0x00,BUFF_SIZE);
                    header_len = value_len = 0;
                }
//		for (i = 0;; i++)
//		{
//			if (srLink->psrText == NULL)
//			{
//				break;
//			}
//			
//			/* 碰到Table底部，設定Tag數並跳出 */
//			if (strlen(srLink->psrText[i].szTag) == 0)
//			{
//				break;
//			}
//
//			/* pointer為空，則跳過 */
//			if (srLink->psrText[i].pCharVariable == NULL)
//				continue;
//
//			/* 變數多過於原來設定的Tag數 */
//			if (srAll->inTextNum == _TAG_TEXT_MAX_NUM_)
//			{
//				
//                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//                                sprintf(szDebugMsg, "TEXT變數過多");
//                                printf("%s\n", szDebugMsg);
//				
//				return (VS_ERROR);
//			}
//
//			/* Tag 名稱過長 */
//			if (strlen(srLink->psrText[i].szTag) > _TAG_MAX_LENGRH_)
//			{
//				
//                                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//                                sprintf(szDebugMsg, "Tag 名稱過長");
//                                printf("%s\n", szDebugMsg);
//				
//				return (VS_ERROR);
//			}
//
//			strcat(srAll->srText[srAll->inTextNum].szTag, srLink->psrText[i].szTag);
//			srAll->srText[srAll->inTextNum].pCharVariable = srLink->psrText[i].pCharVariable;
//			srAll->srText[srAll->inTextNum].inTagValueLen = srLink->psrText[i].inTagValueLen;
//			srAll->inTextNum++;
//		}
//		
//		/* 有誤中止 */
//		if (uszFailBit == VS_TRUE)
//		{
//			break;
//		}
		break;
	}while(1);
        if(header_len !=0)
        {
            printf("%s\n",header_buf);
            printf("%s\n",value_buf);
            printf("----------------------------------------------------------------------------------------------------------------------\n");
            memset(header_buf,0x00,BUFF_SIZE);
            memset(value_buf,0x00,BUFF_SIZE);
            header_len = value_len = 0;
        }
        memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
        sprintf(szDebugMsg, "Table Show 成功");
        printf("%s\n", szDebugMsg);
        
	return (VS_SUCCESS);
}
/*
Function        :inSqlite_Get_Table_Count
Date&Time       :2016/4/29 下午 1:17
Describe        :可以取得table有幾筆資料
*/
int inSqlite_Get_Table_Count(char* szDBPath, char *szTableName, int *inTableCount)
{
	int		j = 0;
	int		inRetVal;
	char		szDebugMsg[128 + 1] = {0};
	char		szQuerySql[_SQL_SENTENCE_MAX_ + 1] = {0};	/* INSERT INTO	szTableName( */
	char		szErrorMessage[100 + 1] = {0};
	sqlite3		*srDBConnection;	/* 建立到資料庫的connection */
	sqlite3_stmt	*srSQLStat;
	
        printf("inSqlite_Get_Table_Count()_START\n");
	
	inRetVal = inSqlite_DB_Open_Or_Create(szDBPath, &srDBConnection, _SQLITE_DEFAULT_FLAGS_, NULL);
	
	if (inRetVal != SQLITE_OK)
	{
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "%d", inRetVal);
            printf("%s\n", szDebugMsg);

            return (VS_ERROR);
	}
	else
	{

            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Open Database File OK");
            printf("%s\n", szDebugMsg);
		
	}
	/* 塞入SQL語句 */
	
	/* 為了使table name可變動，所以拉出來組 */
	memset(szQuerySql, 0x00, sizeof(szQuerySql));

	/* SQLite COUNT 計算資料庫中 table的行數 */
	sprintf(szQuerySql, "SELECT count(*) FROM %s", szTableName);
	
	/* prepare語句 */
	inRetVal = sqlite3_prepare_v2(srDBConnection, szQuerySql, -1, &srSQLStat, NULL);
	if (inRetVal != SQLITE_OK)
	{	
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Prepare Fail: %d", inRetVal);
            printf("%s\n", szDebugMsg);

            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Reason: %s", sqlite3_errmsg(gsrDBConnection));
            printf("%s\n", szDebugMsg);	
	}
	
	/* Qerry */
	inRetVal = sqlite3_step(srSQLStat);
	if (inRetVal == SQLITE_ROW)
	{
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Get Table OK");
            printf("%s\n", szDebugMsg);
	}
	else if (inRetVal == SQLITE_DONE)
	{
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "NO DATA");
            printf("%s\n", szDebugMsg);
            /* 釋放事務 */
            inSqlite_SQL_Finalize(&srSQLStat);
            inSqlite_DB_Close(&srDBConnection);

            return (VS_NO_RECORD);
	}
	else
	{
            memset(szErrorMessage, 0x00, sizeof(szErrorMessage));
            strcpy(szErrorMessage, sqlite3_errmsg(srDBConnection));

            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Get Table ERROR Num:%d", inRetVal);
            printf("%s\n", szDebugMsg);

            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Reason: %s", szErrorMessage);
            printf("%s\n", szDebugMsg);

            printf("%s\n", szQuerySql);


            /* 釋放事務 */
            inSqlite_SQL_Finalize(&srSQLStat);

            /* 關閉 database, close null pointer 是NOP(No Operation) */
            if (inSqlite_DB_Close(&srDBConnection) != VS_SUCCESS)
            {
                /* 如果資料還在更新就close會因為SQLITE_BUSY而失敗，而且正在更新的事務也會roll back（回復上一動）*/
                return (VS_ERROR);
            }
            else
            {
                memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                sprintf(szDebugMsg, "Close Database OK");
                printf("%s\n", szDebugMsg);
            }

            /* 因為直接get 不存在的table回傳值是-1，只有在Error Msg才能得知錯誤原因 */
            if (memcmp(szErrorMessage, "no such table", strlen("no such table")) == 0)
            {
                return (VS_NO_RECORD);
            }
            else
            {
                return (VS_ERROR);
            }
	}
	
	/* 若是成功，將值丟到輸入的位置。 */
	j = 0;
	*inTableCount = sqlite3_column_int(srSQLStat, j);

        memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
        sprintf(szDebugMsg, "Get count(*) OK count:%d", *inTableCount);
        printf("%s\n", szDebugMsg);
	
	/* 釋放事務 */
	inSqlite_SQL_Finalize(&srSQLStat);
	
	/* 關閉 database, close null pointer 是NOP(No Operation) */
	if (inSqlite_DB_Close(&srDBConnection) != VS_SUCCESS)
	{
            /* 如果資料還在更新就close會因為SQLITE_BUSY而失敗，而且正在更新的事務也會roll back（回復上一動）*/
            return (VS_ERROR);
	}
	else
	{
		
            memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
            sprintf(szDebugMsg, "Close Database OK");
            printf("%s\n", szDebugMsg);
		
	}
        printf("inSqlite_Get_Table_Count()_END\n");
             
	return (VS_SUCCESS);
}
/*
Function        :inSqlite_Get_Data_Enormous_Search
Date&Time       :2022/5/16 上午 9:10
Describe        :
*/
int inSqlite_Get_Data_Enormous_Search(char* szDBPath, char* szQuerySql)
{
	int	inRetVal = VS_SUCCESS;
	char	szDebugMsg[1024 + 1] = {0};
        char    szErrorMessage[300 + 1] = {0};
	
        printf("inSqlite_Get_Data_Enormous_Search(%s)_START\n", szDBPath);
        printf("SQL(%s)\n", szQuerySql);
	inRetVal = inSqlite_DB_Open_Or_Create(szDBPath, &gsrDBConnection, _SQLITE_DEFAULT_FLAGS_, NULL);
	
	if (inRetVal != SQLITE_OK)
	{
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "%d", inRetVal);
            printf("%s\n", szDebugMsg);
            return (VS_ERROR);
	}
	else
	{
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Open Database File OK");
            printf("%s\n", szDebugMsg);
	}
	/* 塞入SQL語句 */
	
	/* 為了使table name可變動，所以拉出來組 */
	/* Example:"SELECT * FROM %s WHERE (uszUpdated = 0)ORDER BY lnOrgInvNum ASC" */
	
	/* prepare語句 */
	inRetVal = sqlite3_prepare_v2(gsrDBConnection, szQuerySql, -1, &gsrSQLStat, NULL);
	if (inRetVal != SQLITE_OK)
	{
            memset(szErrorMessage, 0x00, sizeof(szErrorMessage));
            strcpy(szErrorMessage, sqlite3_errmsg(gsrDBConnection));


            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Prepare Fail: %d", inRetVal);
            printf("%s\n", szDebugMsg);

            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Reason: %s", szErrorMessage);
            printf("%s\n", szDebugMsg);


            /* 因為直接get 不存在的table回傳值是-1，只有在Error Msg才能得知錯誤原因 */
            if (memcmp(szErrorMessage, "no such table", strlen("no such table")) == 0)
            {
                    return (VS_NO_RECORD);
            }
            else
            {
                    return (VS_ERROR);
            }
	}
	
	/* 初始化計數 */
//	ginEnormousNowCnt = 0;	
        printf("inSqlite_Get_Data_Enormous_Search()_END\n");
	return (VS_SUCCESS);
}

/*
Function        :inSqlite_Get_Data_Enormous_Get
Date&Time       :2022/5/16 上午 9:25
Describe        :
*/
int inSqlite_Get_Data_Enormous_Get(SQLITE_ALL_TABLE *srAll)
{
	int			inRetVal = 0;
	char			szDebugMsg[100 + 1] = {0};
	
        printf("inSqlite_Get_Data_Enormous_Get() START !\n");

	/* 取得 database 裡所有的資料 */
        do{
            /* Qerry */
            inRetVal = sqlite3_step(gsrSQLStat);
            if (inRetVal == SQLITE_ROW)
            {
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Get Table OK");
                printf("%s\n", szDebugMsg);
                    
            }
            else if (inRetVal == SQLITE_DONE)
            {  
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "NO DATA");
                printf("%s\n", szDebugMsg);
                return (VS_NO_RECORD);
            }
            else
            {         
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Get Table ERROR Num:%d", inRetVal);
                printf("%s\n", szDebugMsg);
                memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                sprintf(szDebugMsg, "Reason: %s", sqlite3_errmsg(gsrDBConnection));
                printf("%s\n", szDebugMsg);
                return (VS_ERROR);
            }
            break;
        }while(1);

	/* binding 取得值 */
	inSqlite_Get_Binding_Value(&gsrSQLStat, srAll);
	
        printf("inSqlite_Get_Data_Enormous_Get() END !\n");	
	return (VS_SUCCESS);
}

/*
Function        :inSqlite_Delete_Record
Date&Time       :
Describe        :刪除inTableID最新一筆資料
*/
int inSqlite_Delete_Record(TRANSACTION_OBJECT *pobTran, char* szTableName)
{
	int		inRetVal;
	char		szDebugMsg[84 + 1];
	char		szQuerySql[_SQL_SENTENCE_MAX_ + 1];	/* INSERT INTO	szTableName( */
	char		*szErrorMessage = NULL;
	sqlite3		*srDBConnection;	/* 建立到資料庫的connection */
	
        printf("inSqlite_Delete_Record()_START\n");   	
	inRetVal = inSqlite_DB_Open_Or_Create(gszTranDBPath, &srDBConnection, _SQLITE_DEFAULT_FLAGS_, NULL);
	
	if (inRetVal != SQLITE_OK)
	{
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "%d", inRetVal);
            printf("%s\n", szDebugMsg);	
            return (VS_ERROR);
	}
	else
	{
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Open Database File OK");
            printf("%s\n", szDebugMsg);
	}
	
	/* 塞入SQL語句 */
	
	/* 為了使table name可變動，所以拉出來組 */
	memset(szQuerySql, 0x00, sizeof(szQuerySql));
	sprintf(szQuerySql, "DELETE FROM %s WHERE inTableID = (SELECT MAX(inTableID) FROM %s)", szTableName, szTableName);
	
	/* 取得 database 裡所有的資料 */
	inRetVal = sqlite3_exec(srDBConnection , szQuerySql, 0 , 0, &szErrorMessage);
	if (inRetVal != SQLITE_OK)
	{	
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Delete Record ERROR Num:%d,", inRetVal);
            printf("%s\n", szDebugMsg);
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Reason:%s", szErrorMessage);
            printf("%s\n", szDebugMsg);
            printf("%s\n", szQuerySql);
	
            if (szErrorMessage != NULL)
            {
                sqlite3_free(szErrorMessage);
            }

            /* 關閉 database, close null pointer 是NOP(No Operation) */
            if (inSqlite_DB_Close(&srDBConnection) != VS_SUCCESS)
            {
                    /* 如果資料還在更新就close會因為SQLITE_BUSY而失敗，而且正在更新的事務也會roll back（回復上一動）*/
                    return (VS_ERROR);
            }
            else
            {             
                memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
                sprintf(szDebugMsg, "Close Database OK");
                printf("%s\n", szDebugMsg);             
            }

            return (VS_ERROR);
	}
	else
	{
            memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
            sprintf(szDebugMsg, "Delete Record OK");
            printf("%s\n", szDebugMsg);
	}
	
	/* 關閉 database, close null pointer 是NOP(No Operation) */
	if (inSqlite_DB_Close(&srDBConnection) != VS_SUCCESS)
	{
		/* 如果資料還在更新就close會因為SQLITE_BUSY而失敗，而且正在更新的事務也會roll back（回復上一動）*/
		return (VS_ERROR);
	}
	else
	{
            memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
            sprintf(szDebugMsg, "Close Database OK");
            printf("%s\n", szDebugMsg);
	}
        printf("inSqlite_ESC_Delete_Record()_END\n");
	return (VS_SUCCESS);
}
/*
Function        :inSqlite_Get_Data_Enormous_Free
Date&Time       :2022/5/16 上午 9:54
Describe        :用完資料Free
*/
int inSqlite_Get_Data_Enormous_Free()
{
	char	szDebugMsg[100 + 1] = {0};
        printf("inSqlite_Get_Data_Enormous_Free() START !\n");
	
	/* 釋放事務 */
	inSqlite_SQL_Finalize(&gsrSQLStat);
	
	/* 計數歸0 */
//	ginEnormousNowCnt = 0;
//	guszEnormousNoNeedResetBit = VS_FALSE;
	
	/* 關閉 database, close null pointer 是NOP(No Operation) */
	if (inSqlite_DB_Close(&gsrDBConnection) != VS_SUCCESS)
	{
		/* 如果資料還在更新就close會因為SQLITE_BUSY而失敗，而且正在更新的事務也會roll back（回復上一動）*/
		return (VS_ERROR);
	}
	else
	{
            memset(szDebugMsg, 0x00, sizeof (szDebugMsg));
            sprintf(szDebugMsg, "Close Database OK");
            printf("%s\n", szDebugMsg);	
	}

        printf("inSqlite_Get_Data_Enormous_Free() END !\n");

	return (VS_SUCCESS);
}