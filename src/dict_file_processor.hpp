#include <stdexcept>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sqlite3.h>

using namespace std;

const char LINE_COMMENT_SIGN = '#';
const string DEFAULT_DELIMITER = ";";

/*
 * wrapper over <filesystem> until C++17 compilers are installed
 * on common systems.
 * */
bool checkFileExist(const char* path) ;

/** use one instance for a table */
class Dxtionary
{
public:
	explicit Dxtionary(const char* dbPath_, const string& tableName_, size_t maxCache=1024)
		:dbPath(dbPath_)
		,tableName(tableName_)
		,cache()
		,maximumCache(maxCache)
	{ /*Nothing to write*/ }
	/**
	 * create a table with given name and given column names. All columns have data-type TEXT.
	 * */
	virtual void createTextTable(const vector<string>& columnNames) ;
	/**
	 * insert text to a table, client must ensure that each text field must be a single line. Newlines
	 * must be escaped before, sothat he can parse result from member function (so und so) of this
	 * class. Text-rows are not inserted directly to database, but they are kept in a cache. The are
	 * inserted to database when the cache is full or the member function flush() is called.
	 * */
	virtual void insertText(const vector<string>& textRow) ;

	/** insert text in cache immediately to database. Cache is then empty. Client of this class
	 * *must* call this method to ensure that all rows are inserted into database. */
	virtual void flush();
	~Dxtionary();

protected:
	tuple<string,string> buildCreateTableStatement(const vector<string>& columnNames) const;

private:
	string dbPath;
	string tableName;
	string insertValueCmd;
	vector<vector<string>> cache;
	size_t maximumCache = 5;
	size_t lineCount = 0;
	static int noop(void *NotUsed, int argc, char **argv, char **azColName) {return 0;}
	inline void executeSqlNoOp(const string& sqlCmd);
};

class DictFileProcessor {
public:
	explicit DictFileProcessor(const string& delimiter_ = DEFAULT_DELIMITER)
		:delimiter(delimiter_)
	{/**/}
	/**
	 * gunzip file {@param dictPath} to import its conten to a Dictionary
	 * {@param dict}
	 * may throw {@class BadDictFileException} if dictionary is not readable.
	 * */
	void processDictFile(const char *dictPath, Dxtionary &dict) const;

	/**
	 * read {@param rawDictionaryDataStream} line-by-line to reach the first non-empty line,
	 * which not begin with a #. This line is considered as column name in table.
	 * A new table with column names in the reached first line is created in
	 * {@param dxtionary}.
	 *
	 * @param rawDictionaryDataStream
	 * @param dxtionary
	 *
	 * */
	void importEntryField(istream &rawDictionaryDataStream, Dxtionary &dxtionary) const;

	/**
	 * read {@param decompressedStream} line-by-line, each line is an entry to be
	 * imported to {@param dxtionary}
	 *
	 * */
	void importDictionaryContent(istream &decompressedStream, Dxtionary &dxtionary) const;

private:
	string delimiter;
};

/**
 * */
vector<string> parseTextToVector(const string& text, const string& delimiter);
string dictFileNameToSqlTableName(const string& fileName);
static inline void printLineCount(size_t lineNumber)
{
	std::cout << lineNumber << endl;
}
// trim from start (in place)
static inline void ltrim(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s)
{
	ltrim(s);
	rtrim(s);
}

/** represents a bad raw dictionary file */
class BadDictFileException: public exception
{
public:
	explicit BadDictFileException(const char* path_, const string& info_="");
	const char * what() const  noexcept ;
private:
	const char* path;
	string info;
	string msg;
};

class DictFileNotExist : public BadDictFileException {
public:
	explicit DictFileNotExist(const char* path_)
		:BadDictFileException(path_, "File not exits")
	{}
};

class DictFileNotReadable : public BadDictFileException {
public:
	explicit DictFileNotReadable(const char* path_)
		:BadDictFileException(path_, "File not readable, or empty")
	{}
};

class DatabaseError: public exception {
public:
	explicit DatabaseError(const char* originError, const char* extraInfo = "")
		:msg(originError)
		,extraInfo(extraInfo)
	{ }
	explicit DatabaseError(const string& originError, const string& extraInfo_ = "")
		:msg(originError)
		,extraInfo(extraInfo_)
	{
		allInfo = msg + string(" ") + extraInfo;
	}

	const char* what() const noexcept
	{
		return allInfo.c_str();
	}
	~DatabaseError()
	{
	}
private:
	string msg;
	string extraInfo;
    string allInfo;
};

static inline void handleSqliteError(sqlite3* db, const char* extraInfo)
{
	const char* errorMsg = sqlite3_errmsg(db);
	sqlite3_close(db); // force close db, don't care about return value
	throw DatabaseError(errorMsg, extraInfo);
}
