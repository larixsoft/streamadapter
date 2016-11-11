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
