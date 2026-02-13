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
char gszTranDBPath[100 +1] = "./fs_data/myDB.db";

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