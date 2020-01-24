#include <streambuf>
#include <memory>
#include <limits>
#include <fstream>
#include <cassert>
#include <sstream>

#include "lzma.h"
#include "xz_stream.h"


int LZMAStreamBuf::underflow()
{
    lzma_action action = LZMA_RUN;
    lzma_ret ret = LZMA_OK;

    // Do nothing if data is still available (sanity check)
    if(this->gptr() < this->egptr())
        return traits_type::to_int_type(*this->gptr());

    while(true)
    {
        m_lzmaStream.next_out =
               reinterpret_cast<unsigned char*>(m_pDecompressedBuf.get());
        m_lzmaStream.avail_out = m_nBufLen;

        if(m_lzmaStream.avail_in == 0)
        {
            // Read from the file, maximum m_nBufLen bytes
            m_pIn->read(&m_pCompressedBuf[0], m_nBufLen);

            // check for possible I/O error
            if(m_pIn->bad())
                throw std::runtime_error
                 ("LZMAStreamBuf: Error while reading the provided input stream!");

            m_lzmaStream.next_in =
                 reinterpret_cast<unsigned char*>(m_pCompressedBuf.get());
            m_lzmaStream.avail_in = m_pIn->gcount();
        }

        // check for eof of the compressed file;
        // if yes, forward this information to the LZMA decoder
        if(m_pIn->eof())
            action = LZMA_FINISH;

        // DO the decoding
        ret = lzma_code(&m_lzmaStream, action);

        // check for data
        // NOTE: avail_out gives that amount of data which is available for LZMA to write!
        //         NOT the size of data which has been written for us!
        if(m_lzmaStream.avail_out < m_nBufLen)
        {
            const size_t nDataAvailable = m_nBufLen - m_lzmaStream.avail_out;

            // Let std::streambuf know how much data is available in the buffer now
            setg(&m_pDecompressedBuf[0], &m_pDecompressedBuf[0],
                               &m_pDecompressedBuf[0] + nDataAvailable);
            return traits_type::to_int_type(m_pDecompressedBuf[0]);
        }

        if(ret != LZMA_OK)
        {
            if(ret == LZMA_STREAM_END)
            {
                // This return code is desired if eof of the source file has been reached
                assert(action == LZMA_FINISH);
                assert(m_pIn->eof());
                assert(m_lzmaStream.avail_out == m_nBufLen);
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
