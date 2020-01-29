#include <iostream>
#include <fstream>

#include "gtest/gtest.h"
#include "gz_stream.hpp"

using namespace std;

namespace {
	TEST(gz_stream, ungzip_file) {
		const char* testDataPath = "./data.txt.gz";
		ifstream testDataFileStream(testDataPath, ios::in | ios::binary);
		if(!testDataFileStream) {
			FAIL() << "Cannot open file '" << testDataPath << "' as stream. Check if file exist";
		}
		GZFileStreamBuffer gzBuffer(&testDataFileStream);
		vector<string> expectedLines {
			"line 1", "line 2", "line 3"
		};
		istream decompressedStream(&gzBuffer);
		size_t i = 0;
		while(!decompressedStream.eof() && !decompressedStream.bad())
		{
			string line;
			getline(decompressedStream, line);
			cout << i << ' ' << line << endl;
			ASSERT_EQ(line, expectedLines[i++]);
		}
	}
}