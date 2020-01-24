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
        :m_pIn(pIn)
        , m_lzmaStream(LZMA_STREAM_INIT)
    {
        m_pCompressedBuf.reset(new char[m_nBufLen]);
        m_pDecompressedBuf.reset(new char[m_nBufLen]);

        // Initially indicate that the buffer is empty
        setg(&m_pDecompressedBuf[0], &m_pDecompressedBuf[1], &m_pDecompressedBuf[1]);

        // try to open the encoder:
        lzma_ret ret = lzma_stream_decoder
               (&m_lzmaStream, std::numeric_limits<uint64_t>::max(), LZMA_CONCATENATED);
        if(ret != LZMA_OK)
            throw std::runtime_error("LZMA decoder could not be opened\n");

        m_lzmaStream.avail_in = 0;
    }

    virtual ~LZMAStreamBuf() override final
    {
    }

    virtual int underflow() override final;

private:
    std::istream* m_pIn;
    std::unique_ptr<char[]> m_pCompressedBuf, m_pDecompressedBuf;
    const size_t m_nBufLen = 10240;
    lzma_stream m_lzmaStream;
};
