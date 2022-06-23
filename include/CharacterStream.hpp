#ifndef __CHARACTERSTREAM_H__
#define __CHARACTERSTREAM_H__

// Standard libraries
#include <string>
#include <iostream>

// Definitions and forward declarations
#include "Definitions.hpp"
#include "ForwardDeclarations.hpp"

// Windows header
#include <windows.h>

namespace mcon
{
    class CharacterStream
    {
        public:
            CharacterStream(String a_buffer = STR(""));
            ~CharacterStream();

            void Read(String a_input);
            void ReadFromClipboard();
            String Peek(uint8_t a_offset);
            String Consume(uint8_t a_offset);
            
            String buffer;

        private:
            uint64_t index = 0;

    };
}

#endif // __CHARACTERSTREAM_H__