#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "dict_file_processor.hpp"

using namespace std;

namespace _TEST_ {
	vector<string> columnNames;
	vector<vector<string>> dictionaryContent;

	class DummyDxtionaryDb : public Dxtionary {
	public:
		DummyDxtionaryDb(const char *dbPath, const string &tableName)
			: Dxtionary(dbPath, tableName)
		{
			columnNames.clear();
			dictionaryContent.clear();
		}

		void createTextTable(const vector<string> &columnNames_) const override {
			for (const string &name : columnNames_) {
				columnNames.push_back(name);
			}
		}

		void insertText(const vector<string> &textRow) override {
			dictionaryContent.push_back(textRow);
		}
	};
}

namespace _TEST_ {
	TEST(dict_file_processor, parseTextToVector)
	{
		string text = "wortart::wordtrennung::bebeutung";
		vector<string> expectedColumns = {
			"wortart", "wordtrennung", "bebeutung"
		};
		vector<string> parsedColumns = parseTextToVector(text, "::");
		ASSERT_EQ(parsedColumns, expectedColumns);
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
}