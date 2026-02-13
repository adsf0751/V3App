#include<sqlite3.h>
/* Link State */
#define _LS_INSERT_	0
#define _LS_READ_	1
#define _LS_UPDATE_	2

/* inSqlite_Get_Batch_ByCnt_Enormous_Flow 使用 */
#define _BYCNT_ENORMMOUS_SEARCH_	0
#define _BYCNT_ENORMMOUS_READ_		1
#define _BYCNT_ENORMMOUS_FREE_		2

#define _SQLITE_DEFAULT_FLAGS_		SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE


int inSqlite_Initial(void);
int inSqlite_Initial_Setting(void);
int inSqlite_DB_Open_Or_Create(char* szDBPath, sqlite3** srDBConnection, int inFlags, char* szVfs);
int inSqlite_DB_Close(sqlite3** srDBConnection);
int inSqlite_SQL_Finalize(sqlite3_stmt **srSQLStat);