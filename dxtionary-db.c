/** 
 * blabla
 *
 * */
#include <stdio.h>
#include <sqlite3.h>
/* #include <sqlite3ext.h> */

#include "dxtionary_version.h"




int sqlite3_spellfix_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
);
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, const char* argv[]) 
{
    if (argc != 3) {
		// report version
        fprintf(stdout, "%s Version %d.%d.%d\n", argv[0],
                  dxtionary_VERSION_MAJOR,
                  dxtionary_VERSION_MINOR,
                  dxtionary_VERSION_PATCH );
        fprintf(stdout, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
		return 1;
	}

    sqlite3_auto_extension((void*)sqlite3_spellfix_init);
    sqlite3 *db;
    int openDbOk = sqlite3_open(argv[1], &db);
    if (openDbOk != 0)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    char *zErrMsg = NULL;
    int excuteSttmOK = sqlite3_exec(db, argv[2], callback, NULL, &zErrMsg);
    if (excuteSttmOK != 0) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
	
	return 0;
}



