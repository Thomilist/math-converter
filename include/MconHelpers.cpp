#include "MconHelpers.hpp"


namespace mcon
{
    // Wrapper function for SendInput to send an entire unicode string
    void SendInputString(std::wstring str)
    {
        std::vector<INPUT> characters;
        INPUT input_base;
        input_base.type = INPUT_KEYBOARD;
        WORD current_character;
        WORD previous_character = L'\0';

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
        for (std::size_t i = 0; i < str.length(); i++)
        {
            previous_character = current_character;
            current_character = str.at(i);
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
    
    void MathConversionHotkey(std::shared_ptr<Settings> a_settings, std::shared_ptr<std::mutex> a_settings_mutex)
    {
        auto character_stream = std::make_unique<mcon::CharacterStream>();
        auto character_set = std::make_shared<mcon::CharacterSet>();
        character_set->LoadFromFolder(".\\resources\\character-sets");
        auto lexer = std::make_unique<mcon::Lexer>(std::move(character_stream), character_set);
        auto mathcad_parser = std::make_unique<mcon::MathcadParser>(std::move(lexer));
        auto latex_generator = std::make_unique<mcon::LatexGenerator>(character_set);
        auto parsing_tree = std::make_shared<mcon::ParsingTree>(std::move(mathcad_parser), std::move(latex_generator));

        // Register hotkey ALT + G or print error if it fails to register
        if (RegisterHotKey(NULL, MCON_HOTKEY_SEND, MOD_CONTROL | MOD_SHIFT, 'V'))
        {
            std::wcout << L"Hotkey registered successfully.\n" << std::endl;
        }
        else
        {
            std::wcerr << L"Hotkey registration error.\n" << std::endl;
            return;
        }

        // Poll the message queue for hotkey triggers
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
        MSG msg = {0};
        #pragma GCC diagnostic push
        while (GetMessage(&msg, NULL, 0, 0) != 0)
        {
            // Hotkeys trigger the WM_HOTKEY message with the hotkey ID in wParam
            if (msg.message == WM_HOTKEY)
            {
                a_settings_mutex->lock();

                switch (a_settings->decimal_separator)
                {
                    case DecimalSeparator::Period:
                    {
                        parsing_tree->decimal_separator = L".";
                        break;
                    }
                    case DecimalSeparator::Comma:
                    {
                        parsing_tree->decimal_separator = L",";
                        break;
                    }
                }

                a_settings_mutex->unlock();
                
                std::wcout << std::endl;
                parsing_tree->Reset();
                parsing_tree->parser->lexer->character_stream->ReadFromClipboard();
                parsing_tree->parser->lexer->Scan();
                parsing_tree->parser->Parse(parsing_tree);
                parsing_tree->parser->Clean(parsing_tree->root_node);
                parsing_tree->Clean(parsing_tree->root_node);
                parsing_tree->generator->Generate(parsing_tree);
                parsing_tree->generator->Substitute(parsing_tree);
                std::wcout << L"Input:  " << parsing_tree->parser->lexer->character_stream->buffer << std::endl;
                std::wcout << L"Output: " << parsing_tree->output << std::endl;
                mcon::SendInputString(parsing_tree->output);
            }
        }

        return;
    }
    
    void ConfigInput(   std::shared_ptr<Settings> a_settings, 
                        std::shared_ptr<std::mutex> a_settings_mutex
    )
    {
        std::wstring input;

        while (true)
        {
            std::getline(std::wcin, input);
            a_settings_mutex->lock();
            a_settings->UpdateSettings(input);
            a_settings_mutex->unlock();
        }

        return;
    }
}