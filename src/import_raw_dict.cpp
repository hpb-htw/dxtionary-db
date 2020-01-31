#include "dict_file_processor.hpp"

#include <iostream>


using namespace std;

int main(int argc, const char** argv) {

	const int BAD_ARGUMENT = 1;
	const int BAD_DICTIONARY_FILE = 2;
	const int BAD_DATABASE_FILE = 3;

	if (argc != 3) {
		cerr << "Error: experted Dictionary file and databse file."
			 << endl;
		cout << argv[0] << " <Dictionary file> <Database file>"
			 << endl;
		return BAD_ARGUMENT;
	}
	const char* dicFileName = argv[1];
	if( !checkFileExist(dicFileName) )
	{
		return BAD_DICTIONARY_FILE;
	}
	const char* databaseFileName = argv[2];
	if( !checkFileExist(databaseFileName) )
	{
		return BAD_DATABASE_FILE;
	}

	try
	{
		//processDictFile(dicFileName, databaseFileName);
	}catch (const BadDictFileException& ex)
	{
    	cerr << ex.what();
        return BAD_DICTIONARY_FILE;
    }

    return 0;
}
