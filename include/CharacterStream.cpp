#include "CharacterStream.hpp"

namespace mcon
{
    CharacterStream::CharacterStream() 
    {
        
    }
    
    CharacterStream::~CharacterStream() 
    {
        
    }
    
    // Read a string of characters from the clipboard.
    // Returns true on error, false on success.
    bool CharacterStream::ReadFromClipboard() 
    {
        if (!OpenClipboard(NULL))
        {
            return true;
        }

        if (IsClipboardFormatAvailable(CF_TEXT))
        {
            HANDLE clip_handle = GetClipboardData(CF_TEXT);

            if (clip_handle != NULL)
            {
                char* clipboard_text(static_cast<char*>(GlobalLock(clip_handle)));

                if (&clipboard_text != nullptr)
                {
                    buffer = std::string(clipboard_text);
                    GlobalUnlock(clip_handle);
                }
                else
                {
                    CloseClipboard();
                    return true;
                }
            }
        }

        CloseClipboard();
        
        return false;
    }
    
    // Return the character at the position (index + a_offset) in "buffer",
    // or the null character if the position is out of bounds.
    char CharacterStream::Peek(uint8_t a_offset) 
    {
        return (index + a_offset) < buffer.size() ? buffer[index + a_offset] : char(0);
    }
    
    // Return the character at the position {index + a_offset} in "buffer",
    // and set {index = index + a_offset}.
    char CharacterStream::Consume(uint8_t a_offset) 
    {
        char character = Peek(a_offset);
        index += a_offset + 1;
        return character;
    }
}