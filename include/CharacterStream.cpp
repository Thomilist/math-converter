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
        
        #ifdef WIN32
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
        #else
        // https://stackoverflow.com/a/44992938/17557793
        Display* display = XOpenDisplay(NULL);
        int N = DefaultScreen(display);
        Window window = XCreateSimpleWindow
        (
            display,
            RootWindow(display, N),
            0,
            0,
            1,
            1,
            0,
            BlackPixel(display, N),
            WhitePixel(display, N)
        );

        Atom clipboard = XInternAtom(display, "CLIPBOARD", 0);
        Atom xsel_data = XInternAtom(display, "XSEL_DATA", 0);
        Atom utf8 = XInternAtom(display, "UTF8_STRING", 1);
        XConvertSelection(display, clipboard, utf8, xsel_data, window, CurrentTime);

        XEvent event;
        do
        {
            XNextEvent(display, &event);
        }
        while (event.type != SelectionNotify || event.xselection.selection != clipboard);

        if (event.xselection.property)
        {
            char* result;
            unsigned long result_size;
            unsigned long result_tail;
            int result_bits;
            XGetWindowProperty
            (
                display,
                window,
                xsel_data,
                0,
                __LONG_MAX__/4,
                1,
                AnyPropertyType,
                &utf8,
                &result_bits,
                &result_size,
                &result_tail,
                (unsigned char**)&result
            );

            Atom incr = XInternAtom(display, "INCR", 0);

            if (utf8 == incr)
            {
                ERROR_OUTPUT << STR("Buffer is too large and INCR reading is not implemented.\n") << std::endl;
            }
            else
            {
                buffer = result;
            }

            XFree(result);

            return;
        }

        XCloseDisplay(display);
        
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