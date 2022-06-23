#include "MconHelpers.hpp"


namespace mcon
{
    // Wrapper function for SendInput to send an entire unicode string
    void SendInputString(String a_string)
    {
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
        for (std::size_t i = 0; i < a_string.length(); i++)
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

        return;
    }

    // Replace the contents of the clipboard with the provided unicode string
    void SetClipboardString(String a_string)
    {
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
        return;
    }

    void MathConversionHotkey(std::shared_ptr<Settings> a_settings, std::shared_ptr<std::mutex> a_settings_mutex)
    {
        auto character_stream = std::make_unique<mcon::CharacterStream>();
        auto character_set = std::make_shared<mcon::CharacterSet>();
        character_set->LoadFromFolder(".\\resources\\character-sets");
        auto lexer = std::make_shared<mcon::Lexer>(std::move(character_stream), character_set);

        std::unordered_map<mcon::InputLanguage, std::unique_ptr<mcon::Parser>> parsers;
        parsers.insert({mcon::InputLanguage::Mathcad, std::make_unique<mcon::MathcadParser>(lexer)});

        std::unordered_map<mcon::OutputLanguage, std::unique_ptr<mcon::Generator>> generators;
        generators.insert({mcon::OutputLanguage::Mathcad, std::make_unique<mcon::MathcadGenerator>(character_set)});
        generators.insert({mcon::OutputLanguage::Latex,   std::make_unique<mcon::LatexGenerator>(character_set)});
        
        auto parsing_tree = std::make_shared<mcon::ParsingTree>(std::move(parsers), std::move(generators));

        // Register hotkey ALT + G or print error if it fails to register
        if (RegisterHotKey(NULL, MCON_HOTKEY_SEND, MOD_CONTROL | MOD_SHIFT, 'V'))
        {
            STRING_OUTPUT << STR("Hotkey registered successfully.\n") << std::endl;
        }
        else
        {
            ERROR_OUTPUT << STR("Hotkey registration error.\n") << std::endl;
            return;
        }

        // Print the current settings
        a_settings->ShowSettings();

        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
        MSG msg = {0};
        #pragma GCC diagnostic push

        InputLanguage input_language;
        OutputLanguage output_language;
        OutputMode output_mode;

        // Poll the message queue for hotkey triggers
        while (GetMessage(&msg, NULL, 0, 0) != 0)
        {
            // Hotkeys trigger the WM_HOTKEY message with the hotkey ID in wParam
            if (msg.message == WM_HOTKEY)
            {
                // Read and apply settings
                a_settings_mutex->lock();

                switch (a_settings->decimal_separator.first)
                {
                    case DecimalSeparator::Period:
                    {
                        parsing_tree->decimal_separator = STR(".");
                        break;
                    }
                    case DecimalSeparator::Comma:
                    {
                        parsing_tree->decimal_separator = STR(",");
                        break;
                    }
                }

                input_language = a_settings->input_language.first;
                output_language = a_settings->output_language.first;
                output_mode = a_settings->output_mode.first;

                a_settings_mutex->unlock();
                
                // Parse and convert math
                parsing_tree->Reset();
                parsing_tree->parsers.at(input_language)->lexer->character_stream->ReadFromClipboard();
                parsing_tree->parsers.at(input_language)->lexer->Scan();
                parsing_tree->parsers.at(input_language)->Parse(parsing_tree);
                parsing_tree->parsers.at(input_language)->Clean(parsing_tree->root_node);
                parsing_tree->Clean(parsing_tree->root_node);
                parsing_tree->generators.at(output_language)->Generate(parsing_tree);
                parsing_tree->generators.at(output_language)->Substitute(parsing_tree);

                STRING_OUTPUT
                    << STR("Input:  ")
                    << parsing_tree->parsers.at(input_language)->lexer->character_stream->buffer
                    << std::endl;
                STRING_OUTPUT
                    << STR("Output: ")
                    << parsing_tree->output
                    << STR("\n")
                    << std::endl;

                switch (output_mode)
                {
                    case OutputMode::Keystrokes:
                    {
                        SendInputString(parsing_tree->output);
                        break;
                    }
                    case OutputMode::Clipboard:
                    {
                        SetClipboardString(parsing_tree->output);
                        break;
                    }
                }
            }
        }

        return;
    }
    
    void ConfigInput(   std::shared_ptr<Settings> a_settings, 
                        std::shared_ptr<std::mutex> a_settings_mutex
    )
    {
        String input;

        while (true)
        {
            std::getline(std::wcin, input);
            a_settings_mutex->lock();
            a_settings->UpdateSettings(input, true);
            a_settings_mutex->unlock();
        }

        return;
    }
}