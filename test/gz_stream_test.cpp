#include <iostream>
#include <fstream>

#include "gtest/gtest.h"
#include "gz_stream.hpp"

using namespace std;

namespace {
	TEST(GZFileStreamBuffer, ungzip_file)
	{
		const char* testDataPath = "./data.txt.gz";
		ifstream testDataFileStream(testDataPath, ios::in | ios::binary);
		if(!testDataFileStream)
		{
			FAIL() << "Cannot open file '" << testDataPath << "' as stream. Check if file exist";
		}
		GZFileStreamBuffer gzBuffer(&testDataFileStream);
		vector<string> expectedLines = {
			"line ä Ä 1",
			"line ö Ö 2",
			"line ü Ü 3",
			"line ß 4",
			"" // the last line is an empty line!
		};
		istream decompressedStream(&gzBuffer);
		size_t i = 0;
		while(!decompressedStream.eof() && !decompressedStream.bad())
		{
			string line;
			getline(decompressedStream, line);
			cout << i << ' ' << line << endl;
			ASSERT_EQ(line, expectedLines[i++]);
			//i = i + 1;
		}
		ASSERT_EQ( i , expectedLines.size() );
	}

	TEST(GZFileStreamBuffer, method_bad_is_true_for_bad_data)
	{
		const char* testDataPath = "./bad-data.txt.gz";
		ifstream testDataFileStream(testDataPath, ios::in | ios::binary);
		if(!testDataFileStream)
		{
			FAIL() << "Cannot open file '" << testDataPath << "' as stream. Check if file exist";
		}
		GZFileStreamBuffer gzBuffer(&testDataFileStream);

		istream decompressedStream(&gzBuffer);
		size_t i = 0;
		while(!decompressedStream.eof() && !decompressedStream.bad() )
		{
			string line;
			getline(decompressedStream, line);
			cout << i << ' ' << line << endl;
			i = i + 1;
		}
		ASSERT_TRUE(decompressedStream.bad() );
	}
}