/*
Copyright (c) <2015>, <gwang at larixsoft.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <new>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include "streamadapter.h"


namespace lax{

StreamAdapter::StreamAdapter(std::size_t capacity )
    :m_streambuf(new boost::asio::streambuf(capacity))
{
}

StreamAdapter::~StreamAdapter()
{

}

void StreamAdapter::clear ()
{
    if(m_streambuf)
    {
        m_streambuf->consume(m_streambuf->size());
    }
}


void StreamAdapter::push (unsigned char *buf, std::size_t size)
{

    if(m_streambuf)
    {
        if(size + m_streambuf->size() > m_streambuf->max_size())
        {
            throw std::length_error("StreamAdapter::push() : not enough memory space for new push");
        }
        boost::asio::streambuf::mutable_buffers_type bufs = m_streambuf->prepare(size);
        auto it = boost::asio::buffers_begin(bufs);
        std::copy(buf,buf+size,it);
        m_streambuf->commit(size);
    }
}

void StreamAdapter::copy (unsigned char *dest, std::size_t offset, std::size_t size) const
{
    if((offset > m_streambuf->size()) || (offset + size > m_streambuf->size()))
    {
        throw std::length_error("StreamAdapter::copy() : out of range");
    }

    if(m_streambuf && dest)
    {
        const unsigned char* ts = boost::asio::buffer_cast<const unsigned char*>(m_streambuf->data());
        std::memcpy(dest,ts+offset,size);
    }
}

void StreamAdapter::flush (std::size_t flush)
{
    if(m_streambuf)
    {
        m_streambuf->consume( flush < m_streambuf->size() ? flush : m_streambuf->size() );
    }
}

std::size_t StreamAdapter::available () const
{
    return m_streambuf ? m_streambuf->size() : 0 ;
}

std::unique_ptr<unsigned char[]> StreamAdapter::take (std::size_t nbytes)
{
    std::unique_ptr<unsigned char[]> rtnPtr = nullptr;
    if(m_streambuf)
    {
        if(nbytes > m_streambuf->size())
        {
            throw std::length_error("StreamAdapter::take");
        }
        rtnPtr.reset(new unsigned char[nbytes]);
        const unsigned char* ts = boost::asio::buffer_cast<const unsigned char*>(m_streambuf->data());
        if(rtnPtr)
        {
            std::memcpy(rtnPtr.get(),ts,nbytes);
            m_streambuf->consume(nbytes);
        }
    }
    return rtnPtr;
}

std::size_t StreamAdapter::max_size () const
{
    return m_streambuf ? m_streambuf->max_size() : 0 ;
}


} //namespace lax
