#include <string>
#include <vector>
#include <cstdio>
#include <tuple>
#include "gtest/gtest.h"
#include "dict_file_processor.hpp"

#include <iostream>
using namespace std;

namespace DICT_FILE_PROCESSOR
{

	class DummyDxtionaryDb : public Dxtionary
	{
	public:
		DummyDxtionaryDb(const char *dbPath, const string &tableName)
			:Dxtionary(dbPath, tableName)
		{

		}

		void createTextTable(const vector<string> &columnNames_) override
		{
			for (const string &name : columnNames_)
			{
				columnNames.push_back(name);
			}
		}

		void insertText(const vector<string> &textRow) override
		{
			dictionaryContent.push_back(textRow);
		}

		vector<string> getColumnNames()
		{
			return this-> columnNames;
		}

		vector<vector<string>> getDictionaryContent() {
			return this->dictionaryContent;
		}
	private:
		vector<string> columnNames = {};
		vector<vector<string>> dictionaryContent = {};
	};
}

namespace DXTIONARY {
	string sqlCreateTable ;
	string sqlInsertInto ;
	class TextualDxtionaryDb : public Dxtionary {
	public:
		TextualDxtionaryDb(const char *dbPath, const string &tableName)
			: Dxtionary(dbPath, tableName)
		{/*Nothing*/}

		void createTextTable(const vector<string> &columnNames_) override
		{
			tuple<string,string> cmd = buildCreateTableStatement(columnNames_);
			sqlCreateTable = std::get<0>(cmd);
			sqlInsertInto =  std::get<1>(cmd);
		}

		void insertText(const vector<string> &textRow) override
		{

		}
	};
	static const char* dbPath = "dictionray-db.sqlite";
	static const char* dictionaryTableName = "wiktionary";

	class DbTestFixture: public ::testing::Test {
	public:
		DbTestFixture( ) = default;

		void SetUp( ) override {
			// code here will execute just before the test ensues
			int rc = std::remove(dbPath);
			if(rc != 0)
			{
				cout << "Info: remove returned " << rc;
			}
		}

		void TearDown( ) override {
			// code here will be called just after the test completes
			// ok to through exceptions from here if need be
		}

		~DbTestFixture( ) override  = default;
	};
}

// test for in Inline function in HPP-Files
namespace INLINE_FUNCTION
{
	TEST(trim, both_left_and_right)
	{
		string text = " blabla   ";
		trim(text);
		ASSERT_EQ(text, "blabla");
	}

	TEST(trim, empty)
	{
		string text = "    ";
		trim(text);
		ASSERT_EQ(text, "");
		ASSERT_TRUE(text.empty());
	}
}

namespace CPP_FUNCTION
{

	TEST(checkFileExist, shout_return_true_when_file_exists) {
		const char* filePath = "../dxtionary_version.h"; // filename of this test file
		bool fileExists = checkFileExist(filePath);
		ASSERT_TRUE(fileExists);
	}

	TEST(checkFileExist, shout_return_false_when_file_exists) {
		const char* filePath = "../not-exist-file.h"; // filename of this test file
		bool fileExists = checkFileExist(filePath);
		ASSERT_FALSE(fileExists);
	}

	TEST(parseTextToVector, simple)
	{
		string text = "wortart::wordtrennung::bebeutung";
		vector<string> expectedColumns = {
			"wortart", "wordtrennung", "bebeutung"
		};
		vector<string> parsedColumns = parseTextToVector(text, "::");
		ASSERT_EQ(parsedColumns, expectedColumns);
	}

	TEST(dictFileNameToSqlTableName, relative)
	{
		string fileName = "../big-file/dewiktionary.csv.gz";
		string tableName = dictFileNameToSqlTableName(fileName);
		string expected = "dewiktionary";
		ASSERT_EQ(tableName, expected);
	}

	TEST(dictFileNameToSqlTableName, absolute)
	{
		string fileName = "/home/user/big-file/dewiktionary.csv.gz";
		string tableName = dictFileNameToSqlTableName(fileName);
		string expected = "dewiktionary";
		ASSERT_EQ(tableName, expected);
	}

	TEST(dictFileNameToSqlTableName, file_does_not_have_extension)
	{
		string fileName = "/home/user/big-file/dewiktionary";
		string tableName = dictFileNameToSqlTableName(fileName);
		string expected = "dewiktionary";
		ASSERT_EQ(tableName, expected);
	}

	TEST(dictFileNameToSqlTableName, replace_spaces_in_file)
	{
		string fileName = "/home/user/big-file/de wiktionary";
		string tableName = dictFileNameToSqlTableName(fileName);
		string expected = "de_wiktionary";
		ASSERT_EQ(tableName, expected);
	}
}

namespace DICT_FILE_PROCESSOR
{
	TEST(DictFileProcessor, importEntryField)
	{
		string fiels = R"str(
# dieses Wortbuch ist ein Copy von deutsche wiktinary
# Version 123.456. 789
# Original in XML

wortart=>worttrennung=>bedeutung
N;test-wort;nix
		)str";
		DummyDxtionaryDb db("nix", "nix");
		istringstream inputStream(fiels);
		DictFileProcessor p("=>");
		p.importEntryField(inputStream, db);
		vector<string> expected = {"wortart", "worttrennung", "bedeutung"};
		ASSERT_EQ(db.getColumnNames(), expected);
	}

	TEST(DictFileProcessor, importDictionaryContent)
	{
		string dictContent = R"sep(
# dieses Wortbuch ist ein Copy von deutsche wiktinary
# Version 123.456. 789
# Original in XML

wortart;worttrennung;bedeutung
verb;test-verb;nix
adj;test-adj;nix2
# ignore this line

bla;bla;bla
)sep";
		DummyDxtionaryDb db("nix", "nix");
		istringstream inputStream(dictContent);
		DictFileProcessor p(";");
		p.importEntryField(inputStream, db);
		p.importDictionaryContent(inputStream, db);
		vector<vector<string>> expected = {
			{"verb", "test-verb", "nix"},
			{"adj",  "test-adj",  "nix2"},
			{"bla",  "bla",       "bla"}
		};
		ASSERT_EQ(db.getDictionaryContent(), expected);
	}

	TEST(DictFileProcessor, processDictFile)
	{
		const char* gzipDictPath = "./gzipPlaintextDict.csv.gz";
		DummyDxtionaryDb db("nix", "nix");
		DictFileProcessor p("<separator>");
		p.processDictFile(gzipDictPath, db);
		vector<vector<string>> expected = {
			{"apple", "green", "1kg"},
			{ "berry", "viollet", "5kg" },
			{ "banana", "yellow", "2kg" },
			{ "cherry", "red", "1.3kg" }
		};
		ASSERT_EQ( db.getDictionaryContent() , expected);
		vector<string> expectedColumns = {
			"fruit", "color", "weight"
		};

	}
}

namespace DXTIONARY
{
	TEST(Dxtionary, buildCreateTableStatement)
	{
		TextualDxtionaryDb db ("nix", "nix");
		vector<string> columns = {
			"col_1", "col_2", "col_3"
		};
		db.createTextTable(columns);
		string expectedCreateTableCmd = "CREATE VIRTUAL TABLE nix USING fts5(col_1, col_2, col_3);";
		ASSERT_EQ(sqlCreateTable, expectedCreateTableCmd);
		string expectedSqlInsert = "INSERT INTO nix(col_1, col_2, col_3) VALUES (?, ?, ?);";
		ASSERT_EQ(sqlInsertInto, expectedSqlInsert);
	}



	TEST_F(DbTestFixture, Dxtionary_createTextTable)
	{
		Dxtionary db(dbPath, dictionaryTableName);
		vector<string> columns = {
			"col_1", "col_2", "col_3"
		};
		ASSERT_NO_THROW(
			db.createTextTable(columns)
		);
		// now create a new table with the same name => expected an exception
		ASSERT_THROW(
			db.createTextTable(columns),
			DatabaseError
		);
	}


	TEST_F(DbTestFixture, Dxtionary_insertText)
	{
		Dxtionary db(dbPath, dictionaryTableName, 4);
		vector<string> columns = {
			"col_1", "col_2", "col_3"
		};
		ASSERT_NO_THROW(
			db.createTextTable(columns)
		);
		vector<vector<string>> data = {
			{"data 1 1", "data 1 2", "data 1 3"},
			{"data 2 3", "data 2 2", "data 2 3"},
			{"data 3 1", "data 3 2", "data 3 3"},
			{"data 4 1", "data 4 2", "data 4 3"},
			{"data 5 1", "data 5 2", "data 5 3"},
		};
		for(vector<string>& d : data)
		{
			ASSERT_NO_THROW( db.insertText(d) );
		}
	}

	TEST(Dxtionary, bad_sqlite_db)
	{
		Dxtionary db("/tmp/file/not/found.sqlite", dictionaryTableName, 4);
		vector<string> columns = {
			"col_1", "col_2", "col_3"
		};
		ASSERT_THROW(
		{
			db.createTextTable(columns);
		}, DatabaseError);
	}

	TEST(Dxtionary, bad_insert)
	{
		Dxtionary db(":memory:", "some_table", 0);
		vector<string> singleData = {"data1", "data2", "data3"};
		ASSERT_THROW({db.insertText(singleData);}, DatabaseError);
	}
}


namespace EXCEPTIONS
{
	TEST(DictFileNotExist, message_as_expected)
	{
		const char* dbpath = "ein/path/to/db.sqlite";
		const char* expected = "Bad file 'ein/path/to/db.sqlite': File not exits";
		try
		{
			DictFileNotExist error(dbpath);
			throw error;
		} catch (const DictFileNotExist& ex)
		{
			ASSERT_STREQ(ex.what(), expected);
		}
	}

	TEST(DictFileNotReadable, message_as_expected)
	{
		const char* dbpath = "ein/path/to/db.sqlite";
		const char* expected = "Bad file 'ein/path/to/db.sqlite': File not readable, or empty";
		try
		{
			DictFileNotReadable error(dbpath);
			throw error;
		}catch (const DictFileNotReadable& ex)
		{
			ASSERT_STREQ(ex.what() , expected);
		}

	}

	TEST(DatabaseError, message_as_expected)
	{
		const char* originError = "Some error";
		const char* extraInfo = "Bad file 'ein/path/to/db.sqlite': File not readable, or empty";
		const char* expected = "Some error Bad file 'ein/path/to/db.sqlite': File not readable, or empty";
		try
		{
			DatabaseError error(originError, extraInfo);
			throw error;
		}catch (const DatabaseError& ex)
		{
			ASSERT_STREQ(ex.what() , expected);
		}
	}

	TEST(DatabaseError, message_as_expected_2)
	{
		string originError = "Some error";
		string extraInfo = "Bad file 'ein/path/to/db.sqlite': File not readable, or empty";
		const char* expected = "Some error Bad file 'ein/path/to/db.sqlite': File not readable, or empty";
		try
		{
			DatabaseError error(originError, extraInfo);
			throw error;
		}catch (const DatabaseError& ex)
		{
			ASSERT_STREQ(ex.what() , expected);
		}
	}
}































