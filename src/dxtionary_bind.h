#include <iostream>


using sqlite3CallbackFn = int (*)(void*,int,char**,char**);
int executeSqlQuery(const char* clientname, const char* filename, const char* sql, sqlite3CallbackFn cb, std::ostream& err);