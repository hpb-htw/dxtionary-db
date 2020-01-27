#include <iostream>
#include "zlib.h"


class GZFileStreamBuffer : public std::streambuf
{

public:

	GZFileStreamBuffer(std::istream* pIn);

	~GZFileStreamBuffer();

	virtual int underflow() override final ;

private:
	const size_t INFLATE_BUFFER_SIZE = 32768;
	const size_t BUFFER_SIZE = 1024;

	const int DEFAULT_WINDOW_BITS = 16;
	const int GZIP_WINDOW_BITS = DEFAULT_WINDOW_BITS + MAX_WBITS; // magic number
	bool eof = false;
	z_stream zStream;

	char* inflateBuffer;
	char_type* deflateBuffer;
	std::istream*  inStream; // gz stream source

	//const std::streamsize _bufsize = 1024;

	int readFromGzStream(char* buffer, std::streamsize length);
};