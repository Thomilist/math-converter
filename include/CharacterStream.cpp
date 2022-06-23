#include "CharacterStream.hpp"

namespace mcon
{
    CharacterStream::CharacterStream(String a_buffer)
        : buffer(a_buffer)
    { }
    
    CharacterStream::~CharacterStream() 
    { }
    
    void CharacterStream::Read(String a_input)
    {
        index = 0;
        buffer = a_input;
        return;
    }
    
    // Read a string of characters from the clipboard
    void CharacterStream::ReadFromClipboard() 
    {
        index = 0;
        
        if (!OpenClipboard(NULL))
        {
            return;
        }

        if (IsClipboardFormatAvailable(CF_UNICODETEXT))
        {
            HANDLE clip_handle = GetClipboardData(CF_UNICODETEXT);

            if (clip_handle != NULL)
            {
                WCHAR* clipboard_text(static_cast<WCHAR*>(GlobalLock(clip_handle)));

                // GlobalLock can return NULL on failure, so NULL check must remain
                #pragma GCC diagnostic push
                #pragma GCC diagnostic ignored "-Waddress"

                if (&clipboard_text != nullptr)
                {
                    buffer = String(clipboard_text);
                    GlobalUnlock(clip_handle);
                }
                else
                {
                    CloseClipboard();
                    return;
                }
                
                #pragma GCC diagnostic pop
            }
        }

        CloseClipboard();
        
        return;
    }
    
    // Return the character at the position (index + a_offset) in "buffer",
    // or the null character if the position is out of bounds.
    String CharacterStream::Peek(uint8_t a_offset) 
    {
        return String(1, (index + a_offset) < buffer.size() ? buffer[index + a_offset] : char(0));
    }
    
    // Return the character at the position {index + a_offset} in "buffer",
    // and set {index = index + a_offset}.
    String CharacterStream::Consume(uint8_t a_offset) 
    {
        String character = Peek(a_offset);
        index += a_offset + 1;
        return character;
    }
}