#include "CharacterStream.hpp"

namespace mcon
{
    CharacterStream::CharacterStream(std::wstring a_buffer)
        : buffer(a_buffer)
    { }
    
    CharacterStream::~CharacterStream() 
    { }
    
    // Read a string of characters from the clipboard.
    // Returns true on error, false on success.
    bool CharacterStream::ReadFromClipboard() 
    {
        if (!OpenClipboard(NULL))
        {
            return true;
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
                    buffer = std::wstring(clipboard_text);
                    GlobalUnlock(clip_handle);
                }
                else
                {
                    CloseClipboard();
                    return true;
                }
                
                #pragma GCC diagnostic pop
            }
        }

        CloseClipboard();

        std::wcout << buffer << "\n";
        
        return false;
    }
    
    // Return the character at the position (index + a_offset) in "buffer",
    // or the null character if the position is out of bounds.
    std::wstring CharacterStream::Peek(uint8_t a_offset) 
    {
        return std::wstring(1, (index + a_offset) < buffer.size() ? buffer[index + a_offset] : char(0));
    }
    
    // Return the character at the position {index + a_offset} in "buffer",
    // and set {index = index + a_offset}.
    std::wstring CharacterStream::Consume(uint8_t a_offset) 
    {
        std::wstring character = Peek(a_offset);
        index += a_offset + 1;
        return character;
    }
}