#include <iostream>
#include <memory>
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

	std::istream*  inStream; // gz stream source
	char* inflateBuffer;
	//std::unique_ptr<char[]> inflateBuffer;
	char_type* deflateBuffer;
	//std::unique_ptr<char_type []> deflateBuffer;
	int readFromGzStream(char* buffer, std::streamsize length);
};