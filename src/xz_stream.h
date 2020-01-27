#include "lzma.h"
#include <streambuf>
#include <memory>
#include <limits>
#include <fstream>
#include <cassert>
#include <sstream>

/**
  * Origin from
  * https://www.codeproject.com/Tips/5098831/Reading-Compressed-LZMA-Files-on-the-fly-using-a-C
  */

class LZMAStreamBuf : public std::streambuf
{
public:
    LZMAStreamBuf(std::istream* pIn)
        : instream(pIn)
        , lzmaStream(LZMA_STREAM_INIT)
    {
        compressedBuffer.reset(new char[CHUNK_SIZE]);
        decompressedBuffer.reset(new char[CHUNK_SIZE]);

        // Initially indicate that the buffer is empty
        setg(&decompressedBuffer[0], &decompressedBuffer[1], &decompressedBuffer[1]);

        // try to open the encoder:
        lzma_ret ret = lzma_stream_decoder
               (&lzmaStream, std::numeric_limits<uint64_t>::max(), LZMA_CONCATENATED);
        if(ret != LZMA_OK)
            throw std::runtime_error("LZMA decoder could not be opened\n");

        lzmaStream.avail_in = 0;
    }

    virtual ~LZMAStreamBuf() override final
    {
    }

    virtual int underflow() override final;

private:
    std::istream* instream;
    std::unique_ptr<char[]> compressedBuffer,
                            decompressedBuffer;
    const size_t CHUNK_SIZE = 10240;
    lzma_stream lzmaStream;
};
