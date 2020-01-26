/** 
 * provide CLI to query/insert data to dictionary database
 *
 * */
#include <iostream>
#include <sqlite3.h>

#include "../dxtionary_version.h"


using namespace std;

int sqlite3_spellfix_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
);

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{

    for(int i = 0; i < argc; i++){
        cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
    }    
    cout << endl;
    return 0;
}

int main(int argc, const char* argv[]) 
{
    if (argc != 3) {
		// report version        
        cout << argv[0] << " Version "
             << dxtionary_VERSION_MAJOR << "."
             << dxtionary_VERSION_MINOR << "."
             << dxtionary_VERSION_PATCH;
        cout << "Usage: " << argv[0] << " DATABASE SQL-STATEMENT";
		return 1;
	}

    sqlite3 *db;
    int openDbOk = sqlite3_open(argv[1], &db);
    if (openDbOk != 0)
    {

        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    char *zErrMsg = nullptr;
    int excuteSttmOK = sqlite3_exec(db, argv[2], callback, nullptr, &zErrMsg);
    if (excuteSttmOK != 0)
    {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
	
	return 0;
}



