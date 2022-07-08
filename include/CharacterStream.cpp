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

        std::string data;
        clip::get_text(data);

        #ifdef WIN32
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        buffer = converter.from_bytes(data);
        #else
        buffer = data;
        #endif

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