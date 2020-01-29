#include <streambuf>
#include <cassert>
#include <sstream>

#include "lzma.h"
#include "xz_stream.h"


int LZMAStreamBuf::underflow()
{
    lzma_action action = LZMA_RUN;
    lzma_ret ret = LZMA_OK;

    // Do nothing if data is still available (sanity check)
    if(this->gptr() < this->egptr()) {
        return traits_type::to_int_type(*this->gptr());
    }
    while(true)
    {
        lzmaStream.next_out =
               reinterpret_cast<unsigned char*>(decompressedBuffer.get());
        lzmaStream.avail_out = CHUNK_SIZE;

        if(lzmaStream.avail_in == 0)
        {
            // Read from the file, maximum CHUNK_SIZE bytes
            instream->read(&compressedBuffer[0], CHUNK_SIZE);

            // check for possible I/O error
            if(instream->bad())
                throw std::runtime_error
                 ("LZMAStreamBuf: Error while reading the provided input stream!");

            lzmaStream.next_in =
                 reinterpret_cast<unsigned char*>(compressedBuffer.get());
            lzmaStream.avail_in = instream->gcount();
        }

        // check for eof of the compressed file;
        // if yes, forward this information to the LZMA decoder
        if(instream->eof()) {
			action = LZMA_FINISH;
		}
        // DO the decoding
        ret = lzma_code(&lzmaStream, action);

        // check for data
        // NOTE: avail_out gives that amount of data which is available for LZMA to write!
        //         NOT the size of data which has been written for us!
        if(lzmaStream.avail_out < CHUNK_SIZE)
        {
            const size_t nDataAvailable = CHUNK_SIZE - lzmaStream.avail_out;

            // Let std::streambuf know how much data is available in the buffer now
            setg(&decompressedBuffer[0], &decompressedBuffer[0],
                 &decompressedBuffer[0] + nDataAvailable);
            return traits_type::to_int_type(decompressedBuffer[0]);
        }

        if(ret != LZMA_OK)
        {
            if(ret == LZMA_STREAM_END)
            {
                // This return code is desired if eof of the source file has been reached
                assert(action == LZMA_FINISH);
                assert(instream->eof());
                assert(lzmaStream.avail_out == CHUNK_SIZE);
                return traits_type::eof();
            }

            // an error has occurred while decoding; reset the buffer
            setg(nullptr, nullptr, nullptr);

            // Throwing an exception will set the bad bit of the istream object
            std::stringstream err;
            err << "Error " << ret << " occurred while decoding LZMA file!";
            throw std::runtime_error(err.str().c_str());
        }
    }
}
