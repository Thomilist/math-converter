#ifndef __CHARACTERSTREAM_H__
#define __CHARACTERSTREAM_H__

// Standard libraries
#include <string>

// Windows header
#include <windows.h>

namespace mcon
{
    class CharacterStream
    {
        public:
            CharacterStream();
            ~CharacterStream();

            bool ReadFromClipboard();
            std::string Peek(uint8_t a_offset);
            std::string Consume(uint8_t a_offset);

        private:
            std::string buffer;
            uint64_t index = 0;

    };
}

#endif // __CHARACTERSTREAM_H__