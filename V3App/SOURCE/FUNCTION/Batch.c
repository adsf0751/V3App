#include "Batch.h"
extern  char		gszTranDBPath[100 + 1];
/*
Function        :inBATCH_Get_Batch_ByCnt_Enormous_Flow
Date&Time       :2017/3/21 下午 1:18
Describe        :查詢大量紀錄使用，最後一定要call free
*/
int inBATCH_Get_Batch_ByCnt_Enormous_Flow(TRANSACTION_OBJECT *pobTran,char* szTableName,int inState,SQLITE_ALL_TABLE* srAll )
{
	int	inRetVal;
	char	szSql[300 + 1] = {0};
	char	szDebugMsg[100 + 1] = {0};
	
	if (inState == _BYCNT_ENORMMOUS_READ_)
	{
            memset(srAll, 0x00, sizeof(SQLITE_ALL_TABLE));
            inRetVal = inSqlite_Table_Link_BRec(pobTran, srAll, _LS_READ_);
            if (inRetVal != VS_SUCCESS)
            {
                    memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
                    sprintf(szDebugMsg, "Table Link 失敗");
                    printf("%s\n", szDebugMsg);       
                    return (VS_ERROR);
            }
            /* 替換資料前先清空srBRec */
            memset(&pobTran->srBRec, 0x00, sizeof(pobTran->srBRec));
	}
	if (inState == _BYCNT_ENORMMOUS_SEARCH_)
	{
            memset(szSql, 0x00, sizeof(szSql));
            //目前沒分流，直接組sql語句
            sprintf(szSql, "SELECT * FROM %s ORDER BY inTableID ASC", szTableName);
            inRetVal = inSqlite_Get_Data_Enormous_Search(gszTranDBPath,szSql);
	}
	else if (inState == _BYCNT_ENORMMOUS_READ_)
	{
            inRetVal = inSqlite_Get_Data_Enormous_Get(srAll);
            
	}
	else if (inState == _BYCNT_ENORMMOUS_FREE_)
	{
		inRetVal = inSqlite_Get_Data_Enormous_Free();
	}
	else
	{
		return (VS_ERROR);
	}
	
	return (inRetVal);
}

