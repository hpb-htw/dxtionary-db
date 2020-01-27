#include "gz_stream.hpp"
#include <iostream>
#include <string>
#include <memory>


using namespace std;

GZFileStreamBuffer::GZFileStreamBuffer(std::istream *pIn)
	: inStream(pIn)
	, inflateBuffer(new char[INFLATE_BUFFER_SIZE])
	, deflateBuffer(new char_type[static_cast<std::size_t>(BUFFER_SIZE)] )
{

	this->setg(deflateBuffer + 4, deflateBuffer + 4, deflateBuffer + 4);
	this->setp(deflateBuffer, deflateBuffer + BUFFER_SIZE);

	zStream.next_in   = 0;
	zStream.avail_in  = 0;
	zStream.total_in  = 0;
	zStream.next_out  = 0;
	zStream.avail_out = 0;
	zStream.total_out = 0;
	zStream.msg       = 0;
	zStream.state     = 0;
	zStream.zalloc    = Z_NULL;
	zStream.zfree     = Z_NULL;
	zStream.opaque    = Z_NULL;
	zStream.data_type = 0;
	zStream.adler     = 0;
	zStream.reserved  = 0;

	// document here: https://www.zlib.net/manual.html#Advanced
	int rc = inflateInit2(&zStream, GZIP_WINDOW_BITS);
	if(rc != Z_OK)
	{
		delete[] inflateBuffer;
		delete[] deflateBuffer;
		throw std::runtime_error(zError(rc));
	}
}

GZFileStreamBuffer::~GZFileStreamBuffer()
{
	delete [] inflateBuffer;
	delete [] deflateBuffer;
	inflateEnd(&zStream);
}


int GZFileStreamBuffer::underflow()
{
	if (!inStream) {
		return traits_type::eof();
	}
	if (this->gptr() && (this->gptr() < this->egptr()))
	{
		return traits_type::to_int_type(*this->gptr());
	}

	int putback = int(this->gptr() - this->eback());
	if (putback > 4) {
		putback = 4;
	}

	traits_type::move(deflateBuffer + (4 - putback), this->gptr() - putback, putback);

	int n = readFromGzStream(deflateBuffer + 4, BUFFER_SIZE - 4);
	if (n <= 0) {
		return traits_type::eof();
	}

	this->setg(deflateBuffer + (4 - putback), deflateBuffer + 4, deflateBuffer + 4 + n);

	// return next character
	return traits_type::to_int_type(*this->gptr());
}

int GZFileStreamBuffer::readFromGzStream(char *buffer, std::streamsize length)
{
	if (eof || !inStream) {
		return 0;
	}

	if (zStream.avail_in == 0)
	{
		int n = 0;
		if (inStream->good())
		{
			inStream->read(inflateBuffer, INFLATE_BUFFER_SIZE);
			n = static_cast<int>(inStream->gcount());
		}
		zStream.next_in   = (unsigned char*) inflateBuffer;
		zStream.avail_in  = n;
	}
	zStream.next_out  = (unsigned char*) buffer;
	zStream.avail_out = static_cast<unsigned>(length);
	while (true)
	{
		int rc = inflate(&zStream, Z_NO_FLUSH);
		if (rc == Z_DATA_ERROR )
		{
			if (zStream.avail_in == 0)
			{
				if (inStream->good())
				{
					rc = Z_OK;
				}else
				{
					rc = Z_STREAM_END;
				}
			}
		}
		if (rc == Z_STREAM_END)
		{
			eof = true;
			return static_cast<int>(length) - zStream.avail_out;
		}
		if (rc != Z_OK) {
			// an error has occurred while decoding; reset the buffer
			setg(nullptr, nullptr, nullptr);
			throw std::runtime_error(zError(rc));
		}
		if (zStream.avail_out == 0) {
			return static_cast<int>(length);
		}
		if (zStream.avail_in == 0)
		{
			int n = 0;
			if (inStream->good())
			{
				inStream->read(inflateBuffer, INFLATE_BUFFER_SIZE);
				n = static_cast<int>(inStream->gcount());
			}
			if (n > 0)
			{
				zStream.next_in  = (unsigned char*) inflateBuffer;
				zStream.avail_in = n;
			}else {
				return static_cast<int>(length) - zStream.avail_out;
			}
		}
	}
}