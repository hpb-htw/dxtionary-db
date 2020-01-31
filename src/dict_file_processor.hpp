#include <stdexcept>
#include <string>
#include <vector>

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
	explicit Dxtionary(const char* dbPath_, const string& tableName_)
		:dbPath(dbPath_)
		,tableName(tableName_)
	{ /*Nothing to write*/ }
	/**
	 * create a table with given name and given column names. All columns have data-type TEXT.
	 * */
	virtual void createTextTable(const vector<string>& columnNames) const;
	/**
	 * insert text to a table, client must ensure that each text field must be a single line. Newlines
	 * must be escaped before, sothat he can parse result from member function (so und so) of this
	 * class. Text-rows are not inserted directly to database, but they are kept in a cache. The are
	 * inserted to database when the cache is full or the member function flush() is called.
	 * */
	virtual void insertText(const vector<string>& textRow) ;

	/** insert text in cache immediately to database. Cache is then empty. Client of this class
	 * *must* call this method to ensure that all rows are inserted into database. */
	virtual void flush() const;

private:
	string dbPath;
	string tableName;
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
	void processDictFile(const char *dictPath, Dxtionary &dict);

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
	void importEntryField(istream &rawDictionaryDataStream, Dxtionary &dxtionary);

	/**
	 * read {@param decompressedStream} line-by-line, each line is an entry to be
	 * imported to {@param dxtionary}
	 *
	 * */
	void importDictionaryContent(istream &decompressedStream, Dxtionary &dxtionary);

private:
	string delimiter;
};

vector<string> parseTextToVector(const string& text, const string& delimiter);

/** represents a bad raw dictionary file */
class BadDictFileException: public exception
{
public:
	explicit BadDictFileException(const char* path_);
	const char * what() const  throw ();
private:
	const char* path;
	string msg;
};