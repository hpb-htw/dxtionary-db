#include "xz_stream.h"

#include <iostream>

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
        LZMAStreamBuf lzmaBuf(&ifs);
        istream decompressedStream(&lzmaBuf);

        while(!decompressedStream.eof() && !decompressedStream.bad())
        {
            string sLine;
            getline(decompressedStream, sLine);
            cout << sLine << endl;
        }
    }catch (...) {
        return BAD_DICTIONARY_FILE;
    }

    return 0;
}
