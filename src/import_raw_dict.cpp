#include "dict_file_processor.hpp"

#include <iostream>


using namespace std;

int main(int argc, const char** argv) {

	const int BAD_ARGUMENT = 1;
	const int BAD_DICTIONARY_FILE = 2;
	const int BAD_DATABASE_FILE = 3;

	if (argc < 3) {
		cerr << "Error: experted Dictionary file and databse file."
			 << endl;
		cout << argv[0] << " <Database file> <Dictionary file>+"
			 << endl;
		return BAD_ARGUMENT;
	}
	const char* databaseFileName  = argv[1];
	// change this if raw CSV change, or better: re-implement main() to parse argument

	const string delimiter = "<separator>";
	for(int i = 2; i < argc; ++i)
	{
		const char *dicFileName = argv[i];
		try
		{
			if (!checkFileExist(dicFileName))
			{
				throw DictFileNotExist(dicFileName);
			}
			DictFileProcessor p(delimiter);
			Dxtionary d(databaseFileName, "dummy", 1024);
			p.processDictFile(dicFileName, d);
		} catch (const BadDictFileException &ex)
		{
			cerr << ex.what();
			return BAD_DICTIONARY_FILE;
		}
	}
    return 0;
}
