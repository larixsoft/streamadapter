/*
The MIT License (MIT)

Copyright (c) 2015 gwang at larixsoft.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
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
