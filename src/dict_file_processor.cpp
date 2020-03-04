#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <algorithm>


#include <sqlite3.h>

#include "dict_file_processor.hpp"
#include "gz_stream.hpp"

#if defined(_MSC_VER)
	#include <locale>
	#include <codecvt>
#endif

#if (_MSC_VER >= 1920)     // MSVC 2019
	#include <filesystem>
	namespace fs = std::filesystem;
#elif (_MSC_VER >= 1710)  // MSVS 2017
	#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
#elif defined(__APPLE__)
	#include <filesystem>
	namespace fs = std::filesystem;
#else                    // other compiler on Ubuntu 18.04
    #include <filesystem>
	namespace fs = std::filesystem;
	//#include <experimental/filesystem>
	//namespace fs = std::experimental::filesystem;
#endif

using namespace std;

bool checkFileExist(const char* path)
{
	fs::path p = path;
	return fs::exists(path);
}

void Dxtionary::createTextTable(const vector<string> &columnNames)
{
	tuple<string,string> cmd = buildCreateTableStatement(columnNames);
	this->insertValueCmd = std::get<1>(cmd);

	executeSqlNoOp(std::get<0>(cmd));
}

void Dxtionary::insertText(const vector<string>& textRow)
{
	cache.push_back(textRow);
	if (cache.size() == maximumCache)
	{
		flush();
	}
}

Dxtionary::~Dxtionary()
{
	if( !cache.empty() )
	{
		flush();
	}
}

void Dxtionary::flush()
{
	/* optimize parameters */
	// PRAGMA cache_size = 400000;
	// PRAGMA synchronous = OFF;
	// PRAGMA journal_mode = OFF;
	// PRAGMA locking_mode = EXCLUSIVE;
	// PRAGMA count_changes = OFF;
	// PRAGMA temp_store = MEMORY;
	// PRAGMA auto_vacuum = NONE;
	sqlite3* mDb;
	int openDbOk = sqlite3_open(this->dbPath.c_str(), &mDb);
	if (openDbOk != 0)
	{
		handleSqliteError(mDb, "Cannot open database");
	}
	char* errorMessage;
	sqlite3_exec(mDb, "PRAGMA auto_vacuum = FULL;BEGIN TRANSACTION", nullptr, nullptr, &errorMessage);

	const char* zSql = insertValueCmd.c_str();
	const int zSqlSize = insertValueCmd.size() + 1;
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(mDb, zSql, zSqlSize, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		handleSqliteError(mDb, "Cannot prepare statement.");
	}
	const size_t cacheSize = cache.size();
	for (size_t i = 0; i < cacheSize; ++i)
	{
		vector<string> data = cache[i];
		for(size_t arg = 0; arg < data.size(); ++arg)
		{
			string dataValue = data[arg];
			const char* columnValue = dataValue.c_str();
			const int columnSize = dataValue.size() + 1;
			int bindRc = sqlite3_bind_text(stmt, arg+1, columnValue, columnSize, SQLITE_TRANSIENT);
			if(bindRc != SQLITE_OK)
			{
				handleSqliteError(mDb, "Cannot bind value to Sql sttm");
			}
		}

		if (sqlite3_step(stmt) != SQLITE_DONE)
		{
			handleSqliteError(mDb,  "Cannot execute Sql sttm");
		}
		sqlite3_reset(stmt);
	}
	int commitRc = sqlite3_exec(mDb, "COMMIT TRANSACTION", nullptr, nullptr, &errorMessage);
	if (commitRc != SQLITE_OK )
	{
		cerr << errorMessage;
		handleSqliteError(mDb, "Cannot commit transaction");
	}
	sqlite3_finalize(stmt);
	cache.clear();
	lineCount += cacheSize;
	printLineCount(lineCount);
}

void Dxtionary::executeSqlNoOp(const string& sqlCmd)
{
	sqlite3* db;
	int openDbOk = sqlite3_open(this->dbPath.c_str(), &db);
	if (openDbOk != 0)
	{
		const char* sqliteError = sqlite3_errmsg(db);
		sqlite3_close(db);
		throw DatabaseError(sqliteError, "Cannot open database");
	}
	char *zErrMsg = nullptr;
	string errorMsg;
	int excuteSttmOK = sqlite3_exec(db, sqlCmd.c_str(), noop, nullptr, &zErrMsg);
	if (excuteSttmOK != 0)
	{
		errorMsg = zErrMsg;
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
	if(excuteSttmOK != 0)
	{
		throw DatabaseError(errorMsg, sqlCmd);
	}
}

tuple<string,string> Dxtionary::buildCreateTableStatement(const vector<string>& columnNames) const
{
	string createTableCmd = "CREATE VIRTUAL TABLE ";
	createTableCmd += (this->tableName + " USING fts5(") ;
	// prepare for insert
	string insertValueCmd = "INSERT INTO ";
	insertValueCmd += (this->tableName + "(");
	string valueClause = " VALUES (";

	const size_t lastIdx = columnNames.size() - 1;
	for(size_t i = 0; i < columnNames.size(); ++i )
	{
		string col = columnNames[i];
		//TODO: check if col is a valid column name
		//this->columns.push_back(col);
		createTableCmd += col /*) += " TEXT"*/;
		insertValueCmd += col;
		valueClause += "?";
		if (i < lastIdx )
		{
			createTableCmd += ", ";
			insertValueCmd += ", ";
			valueClause += ", ";
		}
	}
	createTableCmd += ");";
	insertValueCmd += (")" + valueClause + ");");
	return std::make_tuple(createTableCmd, insertValueCmd);
}



void DictFileProcessor::processDictFile(const char* gzipDictPath, Dxtionary& dxtionary) const
{

	ifstream ifs(gzipDictPath, ios::in | ios::binary);
	if (!ifs) {
		throw DictFileNotReadable(gzipDictPath);
	}
	GZFileStreamBuffer gzBuffer(&ifs);
	istream decompressedStream(&gzBuffer);
	importEntryField(decompressedStream, dxtionary);
	importDictionaryContent(decompressedStream, dxtionary);
}

void DictFileProcessor::importEntryField(istream& rawDictionaryDataStream,  Dxtionary& dxtionary) const
{
	string columnDefinitionLine; // is the first line in file, which is not empty and not begins with a #-charater
	while (!rawDictionaryDataStream.eof() && !rawDictionaryDataStream.bad())
	{
		getline(rawDictionaryDataStream, columnDefinitionLine);
		trim(columnDefinitionLine);
		if( !columnDefinitionLine.empty() && columnDefinitionLine[0] != LINE_COMMENT_SIGN)
		{
			break;
		}
	}
	vector<string> columnName = parseTextToVector(columnDefinitionLine, delimiter);
	dxtionary.createTextTable(columnName);
}

void DictFileProcessor::importDictionaryContent(istream& decompressedStream, Dxtionary& dxtionary) const
{

	while (!decompressedStream.eof() && !decompressedStream.bad())
	{
		string line;
		getline(decompressedStream, line);
		trim(line);
		if( ! (line.empty() || line[0] == LINE_COMMENT_SIGN) )
		{
			vector<string> content = parseTextToVector(line, delimiter);
			dxtionary.insertText(content);
		}
	}
	dxtionary.flush();
}

// helpers
vector<string> parseTextToVector(const string& s, const string& delimiter)
{
	size_t last = 0;
	size_t next = 0;
	vector<string> tokens;
	while ( (next = s.find(delimiter, last)) != string::npos)
	{
		string token = s.substr(last, next-last);
		tokens.push_back(token);
		last = next + delimiter.size() ;
	}

	tokens.push_back(s.substr(last));
	return tokens;
}

string dictFileNameToSqlTableName(const string& fileName)
{
	fs::path p = fileName;
#if defined(_MSC_VER)
	wstring _fn = p.filename();
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	string fn = converter.to_bytes(_fn);
#else
	string fn = p.filename();
#endif
	size_t firstPointPos = fn.find(".");
	if(firstPointPos != string::npos) // trim last
	{
		size_t lastIdx = fn.size();
		fn.erase(firstPointPos, lastIdx);
	}
	std::transform(fn.begin(), fn.end(), fn.begin(), [](char ch) {
		return isspace(ch) ? '_' : ch;
	});

	return fn;
}

