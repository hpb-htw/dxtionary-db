#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

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

void Dxtionary::createTextTable(const vector<string> &columnNames)
{
	// TODO: implement this
}

void Dxtionary::insertText(const vector<string>& textRow)
{
	// TODO: implement it
}

void Dxtionary::flush()
{
	// TODO: implement it
}

void DictFileProcessor::processDictFile(const char* dictPath, Dxtionary& dxtionary) const
{

	ifstream ifs(dictPath, ios::in | ios::binary);
	if (!ifs) {
		throw DictFileNotReadable(dictPath);
	}
	GZFileStreamBuffer gzBuffer(&ifs);
	istream decompressedStream(&gzBuffer);
	importEntryField(decompressedStream, dxtionary);
	importDictionaryContent(decompressedStream, dxtionary);
}

void DictFileProcessor::importEntryField(istream& rawDictionaryDataStream,  Dxtionary& dxtionary) const
{
	string columnDefinitionLine; // is the first line in file, which is not empty and not begins with a #-charater
	while (!rawDictionaryDataStream.eof() && !rawDictionaryDataStream.bad())
	{
		getline(rawDictionaryDataStream, columnDefinitionLine);
		trim(columnDefinitionLine);
		if( !columnDefinitionLine.empty() && columnDefinitionLine[0] != LINE_COMMENT_SIGN)
		{
			break;
		}
	}
	vector<string> columnName = parseTextToVector(columnDefinitionLine, delimiter);
	dxtionary.createTextTable(columnName);
}

void DictFileProcessor::importDictionaryContent(istream& decompressedStream, Dxtionary& dxtionary) const
{
	while (!decompressedStream.eof() && !decompressedStream.bad())
	{
		string line;
		getline(decompressedStream, line);
		trim(line);
		if( ! (line.empty() || line[0] == LINE_COMMENT_SIGN) )
		{
			vector<string> content = parseTextToVector(line, delimiter);
			dxtionary.insertText(content);
		}
	}
	dxtionary.flush();
}

// helpers
vector<string> parseTextToVector(const string& s, const string& delimiter)
{
	size_t last = 0;
	size_t next = 0;
	vector<string> tokens;
	while ( (next = s.find(delimiter, last)) != string::npos)
	{
		string token = s.substr(last, next-last);
		tokens.push_back(token);
		last = next + delimiter.size() ;
	}

	tokens.push_back(s.substr(last));
	return tokens;
}


BadDictFileException::BadDictFileException(const char* path_, const string& info_)
	:path(path_)
	,info(info_)
{
	//NOTE on C++20
	//std::format("Cannot read file '{}'. Check if file exists", dicFileName)
	msg = ((std::string("Bad file '") += path_) + "': ") + info;
}

const char* BadDictFileException::what() const  noexcept
{
	return msg.c_str();
}

