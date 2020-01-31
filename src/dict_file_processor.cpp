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

void Dxtionary::createTextTable(const vector<string> &columnNames) const
{
	// TODO: implement this
}

void Dxtionary::insertText(const vector<string>& textRow)
{
	// TODO: implement it
}

void Dxtionary::flush() const
{
	// TODO: implement it
}

void DictFileProcessor::processDictFile(const char* dictPath, Dxtionary& dxtionary)
{
	ifstream ifs(dictPath, ios::in | ios::binary);
	if (!ifs) {
		throw BadDictFileException(dictPath);
	}
	GZFileStreamBuffer gzBuffer(&ifs);
	istream decompressedStream(&gzBuffer);
	importEntryField(decompressedStream, dxtionary);
	importDictionaryContent(decompressedStream, dxtionary);
}

void DictFileProcessor::importEntryField(istream& rawDictionaryDataStream,  Dxtionary& dxtionary)
{
	string columnDefinitionLine; // is the first line in file, which is not empty and not begins with a #-charater
	while (!rawDictionaryDataStream.eof() && !rawDictionaryDataStream.bad())
	{
		getline(rawDictionaryDataStream, columnDefinitionLine);
		if(columnDefinitionLine.size() > 0 && columnDefinitionLine[0] != LINE_COMMENT_SIGN) {
			break;
		}
	}
	vector<string> columnName = parseTextToVector(columnDefinitionLine, delimiter);
	dxtionary.createTextTable(columnName);
}

void DictFileProcessor::importDictionaryContent(istream& decompressedStream, Dxtionary& dxtionary)
{
	while (!decompressedStream.eof() && !decompressedStream.bad())
	{
		string line;
		getline(decompressedStream, line);
		vector<string> content = parseTextToVector(line, delimiter);
		if(! content.empty() ) {
			dxtionary.insertText(content);
		}
	}
	dxtionary.flush();
}

vector<string> parseTextToVector(const string& s, const string& delimiter)
{
	size_t last = 0;
	size_t next = 0;
	vector<string> tokens;
	if (s.empty() || s[0] == LINE_COMMENT_SIGN) {
		return tokens;
	}
	while ( (next = s.find(delimiter, last)) != string::npos)
	{
		//cout << s.substr(last, next-last) << endl;
		string token = s.substr(last, next-last);
		tokens.push_back(token);
		last = next + delimiter.size() ;
	}
	//cout << s.substr(last) << endl;
	tokens.push_back(s.substr(last));
	return tokens;
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