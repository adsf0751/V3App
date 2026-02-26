#include <sqlite3.h>
#include "Sqlite_Struct.h"
/* Link State */
#define _LS_INSERT_	0
#define _LS_READ_	1
#define _LS_UPDATE_	2

/* inSqlite_Get_Batch_ByCnt_Enormous_Flow 使用 */
#define _BYCNT_ENORMMOUS_SEARCH_	0
#define _BYCNT_ENORMMOUS_READ_		1
#define _BYCNT_ENORMMOUS_FREE_		2

#define _SQLITE_DEFAULT_FLAGS_		SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE
#define	_DATA_BASE_NAME_NEXSYS_TRANSACTION_			"nexsys.db"
#define BUFF_SIZE  4096
int inSqlite_Initial(void);
int inSqlite_Initial_Setting(void);
int inSqlite_DB_Open_Or_Create(char* szDBPath, sqlite3** srDBConnection, int inFlags, char* szVfs);
int inSqlite_DB_Close(sqlite3** srDBConnection);
int inSqlite_SQL_Finalize(sqlite3_stmt **srSQLStat);
int inSqlite_Create_Table(char* szDBName, char* szTableName, SQLITE_TAG_TABLE* pobSQLTag);
int inSqlite_Table_Link_BRec(TRANSACTION_OBJECT *pobTran, SQLITE_ALL_TABLE *srAll, int inLinkState);
int inSqlite_Insert_Record(char* szDBPath, char* szTableName, SQLITE_ALL_TABLE* srAll);
int inSqlite_Calculate_Insert_SQLLength(SQLITE_SQL_CALCULATE_TABLE* srSQLCal, SQLITE_ALL_TABLE* srAll, int* inSqlLength);
int inSqlite_Gernerate_InsertSQL(SQLITE_SQL_CALCULATE_TABLE* srSQLCal, SQLITE_ALL_TABLE* srAll, char* szSQL);
int inSqlite_Binding_Values(SQLITE_ALL_TABLE* srAll, sqlite3_stmt** srSQLStat);
int inSqlite_Get_Max_TableID(char* szDBPath, char* szTableName, char *szTagValue);
int inSqlite_Get_Data_By_External_SQL(char* szDBName, SQLITE_ALL_TABLE* srAll, char* szSQLStatement);
int inSqlite_Get_Binding_Value(sqlite3_stmt** srSQLStat, SQLITE_ALL_TABLE* srAll);
int inSqlite_Table_Show(SQLITE_ALL_TABLE *srAll);
int inSqlite_Table_Link(SQLITE_ALL_TABLE *srAll, int inLinkState, SQLITE_LINK_TABLE* srLink);
int inSqlite_Get_Table_Count(char* szDBPath, char *szTableName, int *inTableCount);
int inSqlite_Get_Data_Enormous_Search(char* szDBPath, char* szQuerySql);
int inSqlite_Get_Data_Enormous_Get(SQLITE_ALL_TABLE *srAll);
int inSqlite_Delete_Record(TRANSACTION_OBJECT *pobTran, char* szTableName);
int inSqlite_Get_Data_Enormous_Free(void);