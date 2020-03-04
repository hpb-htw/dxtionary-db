#include <iostream>
#include "gtest/gtest.h"
#include "dxtionary_bind.h"

using namespace std;

ostringstream output;
int callback(void* NotUse, int argc, char** argv, char** azColName)
{
	cout << argc << ' ';
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
			output.str("");
			output.clear();
		}

		// code here will be called just after the test completes
		// ok to through exceptions from here if need be
		void TearDown( ) override {

		}

		~DbTestFixture( ) override  = default;
	};
}

namespace TEST_DXTIONARY_BIND {
	TEST_F(DbTestFixture, dxtionary_bind_test_editdist3_active) {
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

	TEST_F(DbTestFixture, dxtionary_bind_test_soundex_active) {
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
			string("CREATE VIRTUAL TABLE email USING fts5 (sender, title, body);"),
			string("INSERT INTO email(sender, title, body) VALUES ('test-sender', 'todo', 'test body'), ('test-sender 2', 'todo task', 'test body');"),
			string("SELECT sender FROM email;")
		};
		//";
		vector<string>  expected = {
			"", "", "sender = test-sender\n\nsender = test-sender 2\n\n"
		};
		for(size_t i = 0; i < sql.size(); ++i)
		{
			size_t cmdLength = sql[i].length() + 1;
			char* query = new char[cmdLength];
			size_t copy = (sql[i]).copy(query, cmdLength);
			query[copy] = '\0';
			ostringstream err;
			int rc = executeSqlQuery(dbPath, query, callback, err);
			ASSERT_EQ(rc, 0);
			string collectedResult = output.str();
			string expectedResult = expected[i];
			ASSERT_EQ(collectedResult, expectedResult);
			string errorText = err.str();
			ASSERT_EQ(errorText, ""); // no error at all
			delete [] query;
		}
	}
	//   = test not happy execution-path =
	//  == test error code when database does not exist ==
	TEST_F(DbTestFixture, dxtionary_bind_test_return_error_code_when_db_file_not_accesabel) {
		const char* filename = "/tmp/not/existing/db/file.sqlite";
		const char* query = "SELECT soundex('Phantom') AS soundex;";
		ostringstream err;
		int rc = executeSqlQuery(filename, query, callback, err);
		ASSERT_EQ(rc, dxtionary::BAD_DATABASE_FILE);
	}
	//  == test error code when syntax of querey not correct
	TEST_F(DbTestFixture, dxtionary_bind_test_return_error_code_when_syntax_not_correct) {
		const char* filename = ":memory:";
		const char* query = "SELECT not_a_function('Phantom') AS soundex;";
		ostringstream err;
		int rc = executeSqlQuery(filename, query, callback, err);
		ASSERT_EQ(rc, dxtionary::BAD_QUERY);
	}
}