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

namespace {
	TEST(dxtionary_bind_test, editdist3_active) {
		const char* client = "unittest";
		const char* filename = ":memory:";
		const char* query = "SELECT editdist3('Phantom', 'Fantom') AS editdist3;";
		ostringstream err;
		int rc = executeSqlQuery(client, filename, query, callback, err);
		ASSERT_EQ(rc, 0);
		string collectedResult = output.str();
		string expectedResult = "editdist3 = 250\n\n";
		ASSERT_EQ(collectedResult, expectedResult);
		string errorText = err.str();
		ASSERT_EQ(errorText, ""); // no error
	}

	TEST(dxtionary_bind_test, soundex_active) {
		const char* client = "unittest";
		const char* filename = ":memory:";
		const char* query = "SELECT soundex('Phantom') AS soundex;";
		ostringstream err;
		int rc = executeSqlQuery(client, filename, query, callback, err);
		ASSERT_EQ(rc, 0);
		string collectedResult = output.str();
		string expectedResult = "soundex = P535\n\n";
		ASSERT_EQ(collectedResult, expectedResult);
		string errorText = err.str();
		ASSERT_EQ(errorText, ""); // no error
	}
}