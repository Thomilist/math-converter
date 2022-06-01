#ifndef __CHARACTERSTREAM_H__
#define __CHARACTERSTREAM_H__

// Standard libraries
#include <string>
#include <iostream>

// Windows header
#include <windows.h>

namespace mcon
{
    class CharacterStream
    {
        public:
            CharacterStream(std::wstring a_buffer = L"");
            ~CharacterStream();

            bool ReadFromClipboard();
            std::wstring Peek(uint8_t a_offset);
            std::wstring Consume(uint8_t a_offset);

        private:
            std::wstring buffer;
            uint64_t index = 0;

    };
}

#endif // __CHARACTERSTREAM_H__