/** 
 * blabla
 *
 * */
#include <iostream>
#include <sqlite3.h>
#include <sqlite3ext.h>

#include "dxtionary_version.h"

using namespace std;


int main(int argc, const char* argv[]) 
{
	if (argc < 3) {
		// report version
        cout << argv[0] << " Version "
				  << dxtionary_VERSION_MAJOR << "."
				  << dxtionary_VERSION_MINOR << "."
				  << dxtionary_VERSION_PATCH
				  << std::endl;
        cout << "Usage: " << argv[0] << " <database file> " << " <query>" << std::endl;
		return 1;
	}
	const char* db_path = argv[1];
    cout << "Database path:" << db_path << endl;
    const char* query = argv[2];
    cout << "Query:" << query << endl;
	
	return 0;
}



