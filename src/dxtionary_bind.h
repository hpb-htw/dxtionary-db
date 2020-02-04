#include <iostream>

//using fn_t = int(int); // its type
using sqlite3CallbackFn = int (*)(void*,int,char**,char**);
int executeSqlQuery(const char* filename, const char* sql, sqlite3CallbackFn cb, std::ostream& err);