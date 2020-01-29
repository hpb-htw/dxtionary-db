#include "gz_stream.hpp"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, const char** argv) {

    const int BAD_ARGUMENT = 1;
    const int BAD_DICTIONARY_FILE = 2;
    const int BAD_DATABASE_FILE = 3;

    if (argc != 3)
    {
        cerr << "Error: experted Dictionary file and databse file."
             << endl;
        cout << argv[0] << " <Dictionary file> <Database file>"
             << endl;
        return BAD_ARGUMENT;
    }
    const char* dicFileName = argv[1];
    //TODO: check if dicFileName exist
    try {
        ifstream ifs(dicFileName, ios::in | ios::binary);
        if (!ifs)
		{
        	//NOTE on C++20
        	//std::format("Cannot read file '{}'. Check if file exists", dicFileName)
        	string message = "Cannot read file '";
        	message += dicFileName;
        	message += "'. Check if file exists";
        	throw runtime_error(message);
		}
		GZFileStreamBuffer gzBuffer(&ifs);
        istream decompressedStream(&gzBuffer);


        while(!decompressedStream.eof() && !decompressedStream.bad())
        {
            string line;
            getline(decompressedStream, line);
            cout <<  line ;
            if (!decompressedStream.eof() ) {
            	cout << endl;
            }
        }

    }catch (const runtime_error& ex) {
    	cerr << ex.what();
        return BAD_DICTIONARY_FILE;
    }

    return 0;
}
