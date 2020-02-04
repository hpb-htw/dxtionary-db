#include <iostream>
#include <sqlite3.h>
#include "dxtionary_bind.h"

//using namespace std;

int executeSqlQuery(const char* clientName, const char* filename, const char* sql, sqlite3CallbackFn cb, std::ostream& err)
{
	sqlite3 *db;
	int openDbOk = sqlite3_open(filename, &db);
	if (openDbOk != 0)
	{
		err << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return 1;
	}

	char *zErrMsg = nullptr;
	int excuteSttmOK = sqlite3_exec(db, sql, cb, nullptr, &zErrMsg);
	if (excuteSttmOK != 0)
	{
		err << clientName << ": " << zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);

	return 0;
}