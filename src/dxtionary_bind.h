#include <iostream>

namespace dxtionary {
	const int OK = 0;
	const int BAD_DATABASE_FILE = 10;
	const int BAD_QUERY = 20;
}

using sqlite3CallbackFn = int (*)(void*,int,char**,char**);
int executeSqlQuery(const char* filename, const char* sql, sqlite3CallbackFn cb, std::ostream& err);