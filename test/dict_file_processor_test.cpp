#include <string>
#include <vector>
#include <cstdio>
#include <tuple>
#include "gtest/gtest.h"
#include "dict_file_processor.hpp"

using namespace std;

namespace _TEST_ {
	vector<string> columnNames;
	vector<vector<string>> dictionaryContent;
	string sqlCreateTable ;
	string sqlInsertInto ;
	class DummyDxtionaryDb : public Dxtionary {
	public:
		DummyDxtionaryDb(const char *dbPath, const string &tableName)
			: Dxtionary(dbPath, tableName)
		{
			columnNames.clear();
			dictionaryContent.clear();
		}

		void createTextTable(const vector<string> &columnNames_) override {
			for (const string &name : columnNames_) {
				columnNames.push_back(name);
			}
		}

		void insertText(const vector<string> &textRow) override {
			dictionaryContent.push_back(textRow);
		}
	};

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
		DbTestFixture( ) {
			// initialization code here
		}

		void SetUp( ) {
			// code here will execute just before the test ensues
			int rc = std::remove(dbPath);
			if(rc != 0)
			{
				cout << "Info: remove returned " << rc;
			}
		}

		void TearDown( ) {
			// code here will be called just after the test completes
			// ok to through exceptions from here if need be
		}

		~DbTestFixture( )  {
			// cleanup any pending stuff, but no exceptions allowed
		}


	};
}

namespace _TEST_ {
	TEST(dict_file_processor, trim)
	{
		string text = " blabla   ";
		trim(text);
		ASSERT_EQ(text, "blabla");
	}

	TEST(dict_file_processor, trim_empty)
	{
		string text = "    ";
		trim(text);
		ASSERT_EQ(text, "");
		ASSERT_TRUE(text.empty());
	}

	TEST(dict_file_processor, parseTextToVector)
	{
		string text = "wortart::wordtrennung::bebeutung";
		vector<string> expectedColumns = {
			"wortart", "wordtrennung", "bebeutung"
		};
		vector<string> parsedColumns = parseTextToVector(text, "::");
		ASSERT_EQ(parsedColumns, expectedColumns);
	}

	TEST(dict_file_processor, dictFileNameToSqlTableName_relative)
	{
		string fileName = "../big-file/dewiktionary.csv.gz";
		string tableName = dictFileNameToSqlTableName(fileName);
		string expected = "dewiktionary";
		ASSERT_EQ(tableName, expected);
	}

	TEST(dict_file_processor, dictFileNameToSqlTableName_absolute)
	{
		string fileName = "/home/user/big-file/dewiktionary.csv.gz";
		string tableName = dictFileNameToSqlTableName(fileName);
		string expected = "dewiktionary";
		ASSERT_EQ(tableName, expected);
	}

	TEST(dict_file_processor, dictFileNameToSqlTableName_no_extension)
	{
		string fileName = "/home/user/big-file/dewiktionary";
		string tableName = dictFileNameToSqlTableName(fileName);
		string expected = "dewiktionary";
		ASSERT_EQ(tableName, expected);
	}

	TEST(dict_file_processor, dictFileNameToSqlTableName_replaceSpaces)
	{
		string fileName = "/home/user/big-file/de wiktionary";
		string tableName = dictFileNameToSqlTableName(fileName);
		string expected = "de_wiktionary";
		ASSERT_EQ(tableName, expected);
	}

	TEST(dict_file_processor, importEntryField)
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
		DictFileProcessor p("=>") ;
		p.importEntryField(inputStream, db);
		vector<string> expected = {"wortart","worttrennung", "bedeutung"};
		ASSERT_EQ(columnNames, expected);
	}

	TEST(dict_file_processor, importDictionaryContent)
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
		DictFileProcessor p(";") ;
		p.importEntryField(inputStream, db);
		p.importDictionaryContent(inputStream, db);
		vector<vector<string>> expected = {
			{"verb","test-verb","nix"},
			{"adj","test-adj","nix2"},
			{"bla","bla","bla"}
		};
		ASSERT_EQ(dictionaryContent, expected);
	}

	TEST(dict_file_processor_Dxtionary, buildCreateTableStatement)
	{
		TextualDxtionaryDb db ("nix", "nix");
		vector<string> columns = {
			"col_1", "col_2", "col_3"
		};
		db.createTextTable(columns);
		string expectedCreateTableCmd = "CREATE TABLE nix(col_1 TEXT, col_2 TEXT, col_3 TEXT);";
		ASSERT_EQ(sqlCreateTable, expectedCreateTableCmd);
		string expectedSqlInsert = "INSERT INTO nix(col_1, col_2, col_3) VALUES (?, ?, ?);";
		ASSERT_EQ(sqlInsertInto, expectedSqlInsert);
	}

	TEST_F(DbTestFixture, dict_file_processor_Dxtionary_createTextTable)
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


	TEST_F(DbTestFixture, dict_file_processor_Dxtionary_insertText)
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
}





























