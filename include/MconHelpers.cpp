#include "MconHelpers.hpp"


namespace mcon
{
    // Wrapper function for SendInput to send an entire unicode string
    void SendInputString(String a_string)
    {
        #ifdef WIN32
        std::vector<INPUT> characters;
        INPUT input_base;
        input_base.type = INPUT_KEYBOARD;
        WORD current_character = STR('\0');
        WORD previous_character = STR('\0');

        // Release modifier keys SHIFT, CONTROL and ALT to avoid conflicts with user-pressed keys
        input_base.ki.dwFlags = KEYEVENTF_KEYUP;

        std::vector<char> release_modifiers = {
            VK_SHIFT,
            VK_CONTROL,
            VK_MENU
        };

        for (std::size_t i = 0; i < release_modifiers.size(); i++)
        {
            input_base.ki.wVk = release_modifiers.at(i);
            characters.push_back(input_base);
        }

        // To send unicode characters:
        // 1. use KEYEVENTF_UNICODE in dwFlags (not KEYEVENT_SCANCODE)
        // 2. set wVk to 0
        // 3. place character code in wScan
        input_base.ki.dwFlags = KEYEVENTF_UNICODE;
        input_base.ki.wVk = 0;

        // Send characters one at a time
        for (std::size_t i = 0; i < a_string.length(); ++i)
        {
            previous_character = current_character;
            current_character = a_string.at(i);
            input_base.ki.wScan = current_character;
            characters.push_back(input_base);

            // Convert the vector to a pointer to an array
            INPUT* characters_array = &characters[0];

            // Insert delay, otherwise consecutive identical characters (especially curly braces) sometimes only print the first
            // If not for this, all characters could be sent in the same SendInput call
            if (current_character == previous_character)
            {
                Sleep(5);
            }

            // Send the actual character array
            SendInput(characters.size(), characters_array, sizeof(INPUT));

            // Clear the character vector so only one character is sent per SendInput
            characters.clear();
        }
        #else
            Display* display = XOpenDisplay(NULL);
            FakeKey* key = utf8_fakekey_init(display);

            // Release modifiers
            utf8_fakekey_send_keyevent(key, XKeysymToKeycode(display, XK_Shift_L), false, 0);
            utf8_fakekey_send_keyevent(key, XKeysymToKeycode(display, XK_Shift_R), false, 0);
            utf8_fakekey_send_keyevent(key, XKeysymToKeycode(display, XK_Control_L), false, 0);
            utf8_fakekey_send_keyevent(key, XKeysymToKeycode(display, XK_Control_R), false, 0);

            // Print string one character at a time
            for (std::size_t i = 0; i < a_string.length(); ++i)
            {
                char character = a_string.at(i);
                auto character_pointer = reinterpret_cast<unsigned char*>(&character);
                utf8_fakekey_press(key, character_pointer, -1, 0);
                utf8_fakekey_release(key);
            }
            XCloseDisplay(display);
        #endif

        return;
    }

    // Replace the contents of the clipboard with the provided unicode string
    void SetClipboardString(String a_string)
    {
        #ifdef WIN32
        if (!OpenClipboard(NULL))
        {
            return;
        }

        EmptyClipboard();

        HGLOBAL memory_buffer = GlobalAlloc(GMEM_MOVEABLE, (wcslen(a_string.c_str()) + 1) * sizeof(WCHAR));

        if (memory_buffer == NULL)
        {
            CloseClipboard();
            return;
        }

        LPTSTR buffer_handle = static_cast<LPTSTR>(GlobalLock(memory_buffer));
        memcpy(buffer_handle, a_string.c_str(), (wcslen(a_string.c_str()) + 1) * sizeof(WCHAR));
        GlobalUnlock(memory_buffer);

        SetClipboardData(CF_UNICODETEXT, memory_buffer);

        CloseClipboard();
        #else
        ERROR_OUTPUT
            << STR("Unable to place converted math expression in clipboard:\n")
            << STR("Clipboard mode is not implemented on Linux.\n")
            << std::endl;
        
        a_string.length(); // Prevent unused parameter warning

        // The code below is intended to provide clipboard mode functionality, but does not work

        /*
        Display* display = XOpenDisplay(NULL);
        Window window = XDefaultRootWindow(display);
        Atom clipboard = XInternAtom(display, "CLIPBOARD", true);
        XSetSelectionOwner(display, clipboard, window, CurrentTime);

        XEvent event_request;

        do
        {
            XNextEvent(display, &event_request);
        } while (event_request.type != SelectionRequest);

        XSelectionRequestEvent* request = &event_request.xselectionrequest;

        if (request->selection != clipboard)
        {
            ERROR_OUTPUT << STR("Clipboard error.\n") << std::endl;
            XCloseDisplay(display);
            return;
        }

        auto string_data = reinterpret_cast<const unsigned char*>(a_string.c_str());

        XChangeProperty
        (
            display,
            request->requestor,
            request->property,
            XA_STRING,
            8,
            PropModeReplace,
            string_data,
            strlen(a_string.c_str())
        );

        XEvent event_notify;
        event_notify.xselection.type = SelectionNotify;
        event_notify.xselection.display = request->display;
        event_notify.xselection.requestor = request->requestor;
        event_notify.xselection.selection = request->selection;
        event_notify.xselection.target = request->target;
        event_notify.xselection.property = request->property;
        event_notify.xselection.time = request->time;

        XSendEvent(display, request->requestor, false, 0, &event_notify);

        Window owner = XGetSelectionOwner(display, clipboard);
        STRING_OUTPUT << window << std::endl << owner << std::endl;

        XCloseDisplay(display);
        */
        #endif
        return;
    }

    void MathConversionHotkey(std::shared_ptr<Settings> a_settings, std::shared_ptr<std::mutex> a_settings_mutex)
    {
        auto character_stream = std::make_unique<mcon::CharacterStream>();
        auto character_set = std::make_shared<mcon::CharacterSet>();
        character_set->LoadFromFolder("./resources/character-sets");
        auto lexer = std::make_shared<mcon::Lexer>(std::move(character_stream), character_set);

        std::unordered_map<mcon::InputLanguage, std::unique_ptr<mcon::Parser>> parsers;
        parsers.insert({mcon::InputLanguage::Mathcad, std::make_unique<mcon::MathcadParser>(lexer)});

        std::unordered_map<mcon::OutputLanguage, std::unique_ptr<mcon::Generator>> generators;
        generators.insert({mcon::OutputLanguage::Mathcad,       std::make_unique<mcon::MathcadGenerator>(character_set)});
        generators.insert({mcon::OutputLanguage::Latex,         std::make_unique<mcon::LatexGenerator>(character_set)});
        generators.insert({mcon::OutputLanguage::MathML,        std::make_unique<mcon::MathMLGenerator>(character_set)});
        generators.insert({mcon::OutputLanguage::UnicodeMath,   std::make_unique<mcon::UnicodeMathGenerator>(character_set)});
        
        auto parsing_tree = std::make_shared<mcon::ParsingTree>(std::move(parsers), std::move(generators));

        // Register hotkey CTRL + SHIFT + V or print error if it fails to register
        #ifdef WIN32
        if (RegisterHotKey(NULL, MCON_HOTKEY_SEND, MOD_CONTROL | MOD_SHIFT, 'V'))
        {
            STRING_OUTPUT << STR("Hotkey registered successfully. Use CTRL + SHIFT + V to convert math expressions from the clipboard.\n") << std::endl;
        }
        else
        {
            ERROR_OUTPUT << STR("Hotkey registration error.\n") << std::endl;
            return;
        }
        #endif

        // Print the current settings
        a_settings->ShowSettings();

        #ifdef WIN32
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
        MSG msg = {0};
        #pragma GCC diagnostic push

        // Poll the message queue for hotkey triggers
        while (GetMessage(&msg, NULL, 0, 0) != 0)
        {
            // Hotkeys trigger the WM_HOTKEY message with the hotkey ID in wParam
            if (msg.message == WM_HOTKEY)
            {
                ConvertMath(a_settings, a_settings_mutex, parsing_tree);
            }
        }
        #else
        while (true)
        {
            // https://stackoverflow.com/q/4037230/17557793
            Display* display = XOpenDisplay(NULL);
            Window root_window = XDefaultRootWindow(display);
            XEvent event;

            int keycode = XKeysymToKeycode(display, XK_V);

            ChangeKeyGrab
            (
                display,
                root_window,
                true,
                keycode,
                ControlMask | ShiftMask,
                LockMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask | Mod5Mask
            );

            XSelectInput(display, root_window, KeyPressMask);
            
            XNextEvent(display, &event);
            if (event.type == KeyPress)
            {
                ChangeKeyGrab
                (
                    display,
                    root_window,
                    false,
                    keycode,
                    ControlMask | ShiftMask,
                    LockMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask | Mod5Mask
                );
                ConvertMath(a_settings, a_settings_mutex, parsing_tree);
                usleep(5000);
            }
            else
            {
                XAllowEvents(display, ReplayKeyboard, CurrentTime);
            }

            XFlush(display);
            XCloseDisplay(display);
        }
        #endif

        return;
    }
    
    void ConfigInput(   std::shared_ptr<Settings> a_settings, 
                        std::shared_ptr<std::mutex> a_settings_mutex
    )
    {
        String input;

        while (true)
        {
            std::getline(STRING_INPUT, input);
            a_settings_mutex->lock();
            a_settings->UpdateSettings(input, true);
            a_settings_mutex->unlock();
        }

        return;
    }
    
    void ConvertMath(std::shared_ptr<Settings> a_settings, std::shared_ptr<std::mutex> a_settings_mutex, std::shared_ptr<ParsingTree> a_parsing_tree)
    {
        // Read and apply settings
        a_settings_mutex->lock();

        switch (a_settings->decimal_separator.first)
        {
            case DecimalSeparator::Period:
            {
                a_parsing_tree->decimal_separator = STR(".");
                break;
            }
            case DecimalSeparator::Comma:
            {
                a_parsing_tree->decimal_separator = STR(",");
                break;
            }
        }

        InputLanguage input_language = a_settings->input_language.first;
        OutputLanguage output_language = a_settings->output_language.first;
        OutputMode output_mode = a_settings->output_mode.first;

        a_settings_mutex->unlock();
        
        // Parse and convert math
        a_parsing_tree->Reset();
        a_parsing_tree->parsers.at(input_language)->lexer->character_stream->ReadFromClipboard();
        a_parsing_tree->parsers.at(input_language)->lexer->Scan();
        a_parsing_tree->parsers.at(input_language)->Parse(a_parsing_tree);
        a_parsing_tree->parsers.at(input_language)->Clean(a_parsing_tree->root_node);
        a_parsing_tree->Clean(a_parsing_tree->root_node);
        a_parsing_tree->generators.at(output_language)->Generate(a_parsing_tree);

        STRING_OUTPUT
            << STR("Input:  ")
            << a_parsing_tree->parsers.at(input_language)->lexer->character_stream->buffer
            << std::endl;
        STRING_OUTPUT
            << STR("Output: ")
            << a_parsing_tree->output
            << STR("\n")
            << std::endl;

        switch (output_mode)
        {
            case OutputMode::Keystrokes:
            {
                SendInputString(a_parsing_tree->output);
                break;
            }
            case OutputMode::Clipboard:
            {
                SetClipboardString(a_parsing_tree->output);
                break;
            }
        }

        return;
    }
    
    #ifndef WIN32
    void ChangeKeyGrab
    (
        Display* a_display,
        Window a_window,
        bool a_grab,
        unsigned int a_keycode,
        unsigned int a_modifiers,
        unsigned int a_ignored_modifiers
    )
    {
        unsigned int ignored_mask = 0;

        while (ignored_mask <= a_ignored_modifiers)
        {
            if (ignored_mask & ~a_ignored_modifiers)
            {
                ++ignored_mask;
                continue;
            }

            if (a_grab)
            {
                XGrabKey
                (
                    a_display,
                    a_keycode,
                    a_modifiers | ignored_mask,
                    a_window,
                    1,
                    GrabModeAsync,
                    GrabModeSync
                );
            }
            else
            {
                XUngrabKey
                (
                    a_display,
                    a_keycode,
                    a_modifiers | ignored_mask,
                    a_window
                );
            }

            ++ignored_mask;
        }
    }
    #endif
}