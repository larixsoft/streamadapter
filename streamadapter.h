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

#ifndef STREAM_ADAPTER_H
#define STREAM_ADAPTER_H


#include <cstddef>
#include <iostream>
#include <streambuf>
#include <boost/asio.hpp>

namespace lax {

/** @brief StreamAdapter - adapts incoming data into chunks of N bytes
 *  @remarks
 *  Note that this class is reentrant, but not thread-safe!
 *
 */

class StreamAdapter
{
public:
    /**
     * @brief StreamAdapter constructor
     * @param capacity the max number of bytes this adapter can hold
     */
    explicit StreamAdapter(std::size_t capacity = 2*1024*1024);
    StreamAdapter(const StreamAdapter&) = delete;
    StreamAdapter& operator=(const StreamAdapter&) = delete;
    ~StreamAdapter();

    /**
     * @brief Removes all buffers from adapter
     */
    void clear();

    /**
     * @brief Adds the data from @a buf to the data stored inside adapter and takes ownership of the buffer.
     *
     * @param buf: a buffer to add to queue in the adapter
     * @param size: the number of bytes to copy from @a buf
     * @throw std::length_error if @c available() + @a size > @c max_size()
     */
    void push(unsigned char* buf, std::size_t size);

    /**
     * @brief
     * Copies @a size bytes of data starting at @a offset out of the buffers contained in current dapter into an array
     * @a dest provided by the caller. The buffer is copied from [@a offset @a offset + @a size)
     *
     * The array @a dest should be large enough to contain @a size bytes. The user should check that the adapter has
     * (@a offset + @a size ) bytes available before calling this function.
     *
     * @param dest: the memory to copy into
     * @param offset: the bytes offset in the adapter to start from
     * @param size: the number of bytes to copy
     * @throw std::length_error if @a offset > @c available() or (@a offset + @a size) > @c available()
     */
    void copy(unsigned char* dest, std::size_t offset, std::size_t size) const;

    /**
     * @brief Flushes the first @a flush bytes in the adapter.
     *
     * The caller must ensure that at least this @a flush bytes are available.
     * @param flush the number of bytes to flush
     */
    void flush(std::size_t flush);

    /**
     * @brief Gets the maximum amount of bytes available, that is it returns the maximum value that can be supplied.
     * @return number of bytes available in adapter
     */
    std::size_t available() const;

    /**
     * @brief Returns a freshly allocated buffer containing the first @a nbytes bytes of the adapter.
     * The returned bytes will be flushed from the adapter.Caller owns returned value.
     * @param nbytes the number of bytes to take
     * @return
     * oven-fresh hot data, or @c nullptr if @a nbytes bytes are not available.
     */
    std::unique_ptr<unsigned char[]> take(std::size_t nbytes);

    /**
     * @brief get the capacity in bytes of this adapter
     * @return the capacity of this adapter in bytes
     */
    std::size_t max_size() const;

private:
    std::unique_ptr<boost::asio::streambuf> m_streambuf;
};
}

#endif // STREAM_ADAPTER_H
