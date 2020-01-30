#include "gz_stream.hpp"
#include <iostream>
#include <string>
#include <memory>


using namespace std;

GZFileStreamBuffer::GZFileStreamBuffer(std::istream *pIn)
	: inStream(pIn)
{
	inflateBuffer.reset(new char[INFLATE_BUFFER_SIZE]);
	deflateBuffer.reset(new char_type[BUFFER_SIZE]);
	this->setg(&deflateBuffer[0] + 4, &deflateBuffer[0] + 4, &deflateBuffer[0] + 4);
	this->setp(&deflateBuffer[0], &deflateBuffer[0] + BUFFER_SIZE);

	zStream.next_in   = nullptr;
	zStream.avail_in  = 0;
	zStream.total_in  = 0;
	zStream.next_out  = nullptr;
	zStream.avail_out = 0;
	zStream.total_out = 0;
	zStream.msg       = nullptr;
	zStream.state     = nullptr;
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
		throw std::runtime_error(zError(rc));
	}
}

GZFileStreamBuffer::~GZFileStreamBuffer()
{
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

	traits_type::move(&deflateBuffer[0] + (4 - putback), this->gptr() - putback, putback);

	int n = readFromGzStream(&deflateBuffer[0] + 4, BUFFER_SIZE - 4);
	if (n <= 0) {
		return traits_type::eof();
	}

	this->setg(&deflateBuffer[0] + (4 - putback), &deflateBuffer[0] + 4, &deflateBuffer[0] + 4 + n);

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
			inStream->read(&inflateBuffer[0], INFLATE_BUFFER_SIZE);
			n = static_cast<int>(inStream->gcount());
			zStream.next_in   = reinterpret_cast<unsigned  char*>(inflateBuffer.get());
			zStream.avail_in  = n;
		}
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
				inStream->read(&inflateBuffer[0], INFLATE_BUFFER_SIZE);
				n = static_cast<int>(inStream->gcount());
			}
			if (n > 0)
			{
				zStream.next_in   = reinterpret_cast<unsigned  char*>(inflateBuffer.get());
				zStream.avail_in = n;
			}else {
				return static_cast<int>(length) - zStream.avail_out;
			}
		}
	}
}