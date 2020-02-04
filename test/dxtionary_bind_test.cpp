#include <iostream>
#include "gtest/gtest.h"
#include "dxtionary_bind.h"

using namespace std;

static ostringstream output;
static int callback(void* NotUse, int argc, char** argv, char** azColName)
{
	output.str(""); // empty string
	output.clear(); // reset all error flags
	for(int i = 0; i < argc; ++i)
	{
		output << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
	}
	output << endl;
	return 0;
}

namespace TEST_DXTIONARY_BIND {
	static const char* dbPath = "dictionray-db.sqlite";

	class DbTestFixture: public ::testing::Test {
	public:
		DbTestFixture( ) = default;

		// code here will execute just before the test ensues
		void SetUp( ) override {
			int rc = std::remove(dbPath);
			if(rc != 0)
			{
				cout << "Info: remove returned " << rc;
			}
		}

		// code here will be called just after the test completes
		// ok to through exceptions from here if need be
		void TearDown( ) override {

		}

		~DbTestFixture( ) override  = default;
	};
}

namespace TEST_DXTIONARY_BIND {
	TEST(dxtionary_bind_test, editdist3_active) {
		const char* filename = ":memory:";
		const char* query = "SELECT editdist3('Phantom', 'Fantom') AS editdist3;";
		ostringstream err;
		int rc = executeSqlQuery(filename, query, callback, err);
		ASSERT_EQ(rc, 0);
		string collectedResult = output.str();
		string expectedResult = "editdist3 = 250\n\n";
		ASSERT_EQ(collectedResult, expectedResult);
		string errorText = err.str();
		ASSERT_EQ(errorText, ""); // no error
	}

	TEST(dxtionary_bind_test, soundex_active) {
		const char* filename = ":memory:";
		const char* query = "SELECT soundex('Phantom') AS soundex;";
		ostringstream err;
		int rc = executeSqlQuery(filename, query, callback, err);
		ASSERT_EQ(rc, 0);
		string collectedResult = output.str();
		string expectedResult = "soundex = P535\n\n";
		ASSERT_EQ(collectedResult, expectedResult);
		string errorText = err.str();
		ASSERT_EQ(errorText, ""); // no error
	}

	TEST_F(DbTestFixture, dxtionary_bind_test_full_text_search_5_active)
	{
		vector<string> sql = {
			"CREATE VIRTUAL TABLE email USING fts5 (sender, title, body);",
			"INSERT INTO email(sender, title, body) VALUES ('test-sender', 'test title', 'test body'), ('test-sender 2', 'other title', 'test body');",
			"SELECT sender, rank FROM email;"
		};
		//";
		vector<string>  expected = {
			"", "", "TODO"
		};
		for(size_t i = 0; i < sql.size(); ++i)
		{
			const char* query = sql[i].c_str() ;
			ostringstream err;
			int rc = executeSqlQuery(dbPath, query, callback, err);
			ASSERT_EQ(rc, 0);
			string collectedResult = output.str();
			string expectedResult = expected[i];
			ASSERT_EQ(collectedResult, expectedResult);
			string errorText = err.str();
			ASSERT_EQ(errorText, ""); // no error at all
		}
	}
}