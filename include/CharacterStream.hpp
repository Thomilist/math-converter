#ifndef __CHARACTERSTREAM_H__
#define __CHARACTERSTREAM_H__

// Standard libraries
#include <string>
#include <iostream>

// Clipboard
#include "../clip/clip.h"

// Definitions and forward declarations
#include "Definitions.hpp"
#include "ForwardDeclarations.hpp"

// OS-specific libraries
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <locale>
#include <codecvt>
#else
#include <X11/Xlib.h>
#endif

namespace mcon
{
    class CharacterStream
    {
        public:
            CharacterStream(String a_buffer = STR(""));
            ~CharacterStream();

            void Read(String a_input);
            void ReadFromClipboard();
            String Peek(uint8_t a_offset = 0);
            String Consume(uint8_t a_offset = 0);
            
            String buffer;

        private:
            uint64_t index = 0;

    };
}

#endif // __CHARACTERSTREAM_H__