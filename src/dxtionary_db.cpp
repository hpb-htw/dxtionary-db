/** 
 * provide CLI to query/insert data to dictionary database
 *
 * */
#include <iostream>
#include <sqlite3.h>
#include "dxtionary_bind.h"
#include "../dxtionary_version.h"


using namespace std;

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

	const char* clientname = argv[0];
	const char* filename = argv[1];
	const char* sql = argv[2];
	return executeSqlQuery(filename, sql, callback, cerr);
}



