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

#include "streamadapter.h"
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <cassert>


int main()
{
    char buf[256];
    for(auto i = 0; i < 256; i++)
    {
        buf[i] = i;
    }

    std::cout << "Testing StreamAdapter::StreamAdapter()" << std::endl;
    lax::StreamAdapter adapter(1024);
    assert(adapter.max_size() == 1024);
    std::cout << "constructor, adapter.max_size () = " << adapter.max_size() << std::endl << std::endl;

    std::cout << "Testing StreamAdapter::push()" << std::endl;
    adapter.push((unsigned char*)buf,128);
    assert(adapter.available() == 128);
    std::cout << "push(), adapter.available () = " << adapter.available() << std::endl << std::endl;


    std::cout << "Testing StreamAdapter::flush()" << std::endl;
    adapter.flush(64);
    std::cout << "flush(), adapter.available () = " << adapter.available() << std::endl;
    assert(adapter.available() == 64);

    adapter.flush(32);
    assert(adapter.available() == 32);
    std::cout << "flush(), adapter.available () = " << adapter.available() << std::endl;

    adapter.flush(64);
    assert(adapter.available() == 0);
    std::cout << "flush(), adapter.available () = " << adapter.available() << std::endl << std::endl;

    std::cout << "Testing StreamAdapter::available()" << std::endl;
    adapter.push((unsigned char*)buf,256);
    assert(adapter.available() == 256);
    std::cout << "adapter.available () = " << adapter.available() << std::endl << std::endl;

    std::cout << "Testing StreamAdapter::clear()" << std::endl;
    adapter.clear();
    assert(adapter.available() == 0);
    std::cout << "adapter.available () = " << adapter.available() << std::endl << std::endl;

    std::cout << "Testing StreamAdapter::take()" << std::endl;
    adapter.clear();
    adapter.push((unsigned char*)buf,256);
    auto obuf = adapter.take(128);
    std::cout << "take(), adapter.available () = " << adapter.available() << std::endl;
    for(auto i=0;i<128;i++) {
        if( i%16 == 0) {
            std::cout<<std::endl;
        }
        std::cout << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(*(obuf.get()+i)) << " ";
    }
    std::cout << std::endl << std::endl;
    obuf = adapter.take(128);
    std::cout << "take(), adapter.available () = " << adapter.available() << std::endl;
    for(auto i=0;i<128;i++) {
        if( i%16 == 0) {
            std::cout<<std::endl;
        }
        std::cout << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(*(obuf.get()+i)) << " ";
    }
    std::cout << std::dec << std::endl << std::endl;

    std::cout << "Testing StreamAdapter::copy()" << std::endl;
    adapter.clear();
    adapter.push((unsigned char*)buf,256);
    assert(adapter.available() == 256);
    std::cout << "push(), adapter.available () = " << adapter.available() << std::endl;

    unsigned char ibuf[128];
    adapter.copy(ibuf,0,128);
    assert(adapter.available() == 256);
    std::cout << "copy(), adapter.available () = " << adapter.available() << std::endl;
    for(auto i=0;i<128;i++) {
        if( i%16 == 0) {
            std::cout<<std::endl;
        }
        std::cout << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(*(ibuf+i)) << " ";
    }
    std::cout << std::dec << std::endl << std::endl;

    adapter.copy(ibuf,128,128);
    assert(adapter.available() == 256);
    std::cout << "copy(),adapter.available () = " << adapter.available() << std::endl;
    for(auto i=0;i<128;i++) {
        if( i%16 == 0) {
            std::cout<<std::endl;
        }
        std::cout << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(*(ibuf+i)) << " ";
    }
    std::cout << std::endl;
}
