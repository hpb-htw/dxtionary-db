#include <stdexcept>
#include <string>

/*
 * wrapper over <filesystem> until C++17 compilers are installed
 * on common systems.
 * */
bool checkFileExist(const char* path) ;

/**
 * gunzip file {@param dictPath} to import its conten to database path
 * {@param databasePath}
 * may throw {@class BadDictFileException} if dictionary is not readable.
 * */
void processDictFile(const char* dictPath, const char* databasePath);

class SqliteConnector
{

};

class BadDictFileException: public std::exception
{
public:
	explicit BadDictFileException(const char* path_);
	const char * what() const  throw ();
private:
	const char* path;
	std::string msg;
};