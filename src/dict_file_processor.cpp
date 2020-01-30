#include <iostream>
#include <fstream>
#include <string>
#include <experimental/filesystem>
#include "dict_file_processor.hpp"
#include "gz_stream.hpp"

namespace fs = std::experimental::filesystem;
using namespace std;

bool checkFileExist(const char* path)
{
	fs::path p = path;
	return fs::exists(path);
}

void processDictFile(const char* dictPath, const char* databasePath)
{
	ifstream ifs(dictPath, ios::in | ios::binary);
	if (!ifs) {
		throw BadDictFileException(dictPath);
	}

	GZFileStreamBuffer gzBuffer(&ifs);
	istream decompressedStream(&gzBuffer);
	while (!decompressedStream.eof() && !decompressedStream.bad()) {
		string line;
		getline(decompressedStream, line);
		cout << line;
		if (!decompressedStream.eof()) {
			cout << endl;
		}
	}
}

BadDictFileException::BadDictFileException(const char* path_)
	:path(path_)
{
	//NOTE on C++20
	//std::format("Cannot read file '{}'. Check if file exists", dicFileName)
	msg = (std::string("Cannot read file '") += path_)
		+= "'. Check if file is empty or if file exists.";
}

const char* BadDictFileException::what() const  throw ()
{
	return msg.c_str();
}