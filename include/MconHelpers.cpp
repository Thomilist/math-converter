#include "MconHelpers.hpp"


namespace mcon
{
    nlohmann::json LoadJSON(std::string a_file_path) 
    {
        std::ifstream json_file(a_file_path);

        if (!json_file.is_open())
        {
            throw std::runtime_error("Unable to open JSON file.");
        }

        // Parse JSON file, allowing JavaScript-style comments
        nlohmann::json json_object = nlohmann::json::parse(json_file, nullptr, true, true);

        json_file.close();

        return json_object;
    }

    // Wrapper function for SendInput to send an entire unicode string
    void SendInputString(std::wstring str)
    {
        std::vector<INPUT> characters;
        INPUT input_base;
        input_base.type = INPUT_KEYBOARD;
        WORD character;

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
            character = str.at(i);
            input_base.ki.wScan = character;
            characters.push_back(input_base);

            // Convert the vector to a pointer to an array
            INPUT* characters_array = &characters[0];

            // Send the actual character array
            SendInput(characters.size(), characters_array, sizeof(INPUT));

            // Clear the character vector so only one character is sent per SendInput
            characters.clear();

            // Insert delay, otherwise consecutive curly braces only print the first
            // If not for this, all characters could be sent in the same SendInput call
            if (character == L'{' || character == L'}')
            {
                Sleep(1);
            }
        }

        return;
    }
}