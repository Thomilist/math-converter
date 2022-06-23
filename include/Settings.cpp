#include "Settings.hpp"



namespace mcon
{
    Settings::Settings():
        lexer(std::move(character_stream), character_set)
    {
        character_set->LoadFromFolder(".\\resources\\character-sets");
        STRING_OUTPUT << STR("For usage information, enter \"help\" (without the quote marks) or read the associated user guide.") << std::endl;
        LoadSettings();
    }
    
    Settings::~Settings()
    { }
    
    void Settings::UpdateSettings(std::wstring a_console_input, bool a_user_triggered)
    {
        lexer.character_stream->Read(a_console_input);
        lexer.Scan();

        // Initialise from start of token stream
        Token current_token = Token(TokenType::StartOfStream);

        while (current_token.type == TokenType::StartOfStream)
        {
            current_token = lexer.Consume(0);
        }

        // Read command
        Setting setting = Setting::Help;
        
        if (current_token.type == TokenType::EndOfStream)
        {
            return;
        }
        else if (current_token.type == TokenType::Text)
        {
            try
            {
                setting = commands.at(current_token.content);
            }
            catch(const std::out_of_range& e)
            {
                CommandNotRecognised(current_token.content);
                ERROR_OUTPUT << STR("Out-of-range exception in ") << e.what() << STR("\n") << std::endl;
                return;
            }
        }
        else
        {
            CommandNotRecognised(current_token.content);
            ERROR_OUTPUT << std::endl;
        }

        current_token = lexer.Consume(0);

        // Read argument
        while (true)
        {
            if (    setting == Setting::Help            ||
                    setting == Setting::ShowSettings    ||
                    setting == Setting::Download
            )
            {
                break;
            }
            else if (current_token.type == TokenType::EndOfStream)
            {
                ERROR_OUTPUT  << STR("The command is incomplete. Please provide an argument.\n")
                            << STR("Use the command \"help\" to display help.\n") << std::endl;
                return;
            }
            else if (   current_token.type == TokenType::Text   ||
                        current_token.type == TokenType::Symbol
            )
            {
                break;
            }
            
            current_token = lexer.Consume(0);
        }

        try
        {
            switch (setting)
            {
                case Setting::Help:
                {
                    PrintHelp();
                    break;
                }
                case Setting::ShowSettings:
                {
                    ShowSettings();
                    break;
                }
                case Setting::Download:
                {
                    Download();
                    break;
                }
                case Setting::DecimalSeparator:
                {
                    decimal_separator.first = decimal_separator_settings.at(current_token.content);
                    decimal_separator.second = a_console_input;
                    if (a_user_triggered)
                    {
                        STRING_OUTPUT << STR("\nDecimal separator updated.\n") << std::endl;
                    }
                    break;
                }
                case Setting::OutputMode:
                {
                    output_mode.first = output_mode_settings.at(current_token.content);
                    output_mode.second = a_console_input;
                    if (a_user_triggered)
                    {
                        STRING_OUTPUT << STR("\nOutput mode updated.\n") << std::endl;
                    }
                    break;
                }
                case Setting::InputLanguage:
                {
                    input_language.first = input_language_settings.at(current_token.content);
                    input_language.second = a_console_input;
                    if (a_user_triggered)
                    {
                        STRING_OUTPUT << STR("\nInput language updated.\n") << std::endl;
                    }
                    break;
                }
                case Setting::OutputLanguage:
                {
                    output_language.first = output_language_settings.at(current_token.content);
                    output_language.second = a_console_input;
                    if (a_user_triggered)
                    {
                        STRING_OUTPUT << STR("\nOutput language updated.\n") << std::endl;
                    }
                    break;
                }
            }

            if (a_user_triggered)
            {
                SaveSettings();
            }
        }
        catch(const std::out_of_range& e)
        {
            CommandNotRecognised(current_token.content);
            ERROR_OUTPUT << STR("Out-of-range exception in ") << e.what() << STR("\n") << std::endl;
        }

        return;
    }
    
    void Settings::CommandNotRecognised(std::wstring a_unknown_command)
    {
        ERROR_OUTPUT  << STR("\nCommand \"") << a_unknown_command << STR("\" not recognised.\n")
                    << STR("Use the command \"help\" to display help.\n");
        return;
    }
    
    void Settings::PrintHelp()
    {
        STRING_OUTPUT
        << STR("\n")
        << STR("Displaying help for math-converter\n")
        << STR("\n")
        << STR("Consult the user guide for additional information\n")
        << STR("\n")
        << STR("Convert and insert math string\n")
        << STR("Hotkey:\n")
        << STR("    CTRL + SHIFT + V\n")
        << STR("\n")
        << STR("Command format:\n")
        << STR("COMMAND ARGUMENT\n")
        << STR("\n")
        << STR("Available commands:\n")
        << STR("\n")
        << STR("Display help\n")
        << STR("COMMAND aliases:\n")
        << STR("    h help ?\n")
        << STR("Available ARGUMENTs: none\n")
        << STR("\n")
        << STR("Show current settings\n")
        << STR("COMMAND aliases:\n")
        << STR("    s show settings\n")
        << STR("Available ARGUMENTs: none\n")
        << STR("\n")
        << STR("Open download page\n")
        << STR("COMMAND aliases:\n")
        << STR("    download\n")
        << STR("Available ARGUMENTs: none\n")
        << STR("\n")
        << STR("Set decimal separator\n")
        << STR("COMMAND aliases:\n")
        << STR("    d dec ds sep\n")
        << STR("Available ARGUMENTs:\n")
        << STR("    period .\n")
        << STR("    comma ,\n")
        << STR("\n")
        << STR("Set output mode\n")
        << STR("COMMAND aliases:\n")
        << STR("    m mode\n")
        << STR("Available ARGUMENTs:\n")
        << STR("    keystrokes keys\n")
        << STR("    clipboard clip\n")
        << STR("\n")
        << STR("Set input language\n")
        << STR("COMMAND aliases:\n")
        << STR("    i in input\n")
        << STR("Available ARGUMENTs:\n")
        << STR("    Mathcad mathcad\n")
        /*
        << STR("    LaTeX Latex latex\n")
        << STR("    UnicodeMath unicodemath Unicode unicode\n")
        << STR("    AsciiMath asciimath Ascii ASCII ascii\n")
        << STR("    MathML mathml\n")
        << STR("    Mathematica mathematica\n")
        << STR("    Excel excel\n")
        << STR("    MathJSON mathjson json\n")
        */
        << STR("\n")
        << STR("Set output language\n")
        << STR("COMMAND aliases:\n")
        << STR("    o out output\n")
        << STR("Available ARGUMENTs:\n")
        << STR("    Mathcad mathcad\n")
        << STR("    LaTeX Latex latex\n")
        /*
        << STR("    UnicodeMath unicodemath Unicode unicode\n")
        << STR("    AsciiMath asciimath Ascii ASCII ascii\n")
        << STR("    MathML mathml\n")
        << STR("    Mathematica mathematica\n")
        << STR("    Excel excel\n")
        << STR("    MathJSON mathjson JSON json\n")
        */
        << std::endl;
        
        return;
    }
    
    void Settings::ShowSettings()
    {
        String current_decimal_separator;
        String current_input_language;
        String current_output_language;
        String current_output_mode;

        try
        {
            current_decimal_separator = decimal_separator_names.at(decimal_separator.first);
            current_output_mode = output_mode_names.at(output_mode.first);
            current_input_language = input_language_names.at(input_language.first);
            current_output_language = output_language_names.at(output_language.first);
        }
        catch(const std::out_of_range& e)
        {
            ERROR_OUTPUT 
            << STR("\n")
            << STR("Error while looking up settings.\n")
            << STR("Out-of-range exception in ") << e.what() << std::endl;
        }
        
        STRING_OUTPUT
        << STR("\n")
        << STR("Displaying current settings\n")
        << STR("\n")
        << STR("Decimal separator:  ")
        << current_decimal_separator
        << STR("\n")
        << STR("Output mode:        ")
        << current_output_mode
        << STR("\n")
        << STR("Input language:     ")
        << current_input_language
        << STR("\n")
        << STR("Output language:    ")
        << current_output_language
        << STR("\n")
        << std::endl;

        return;
    }
    
    void Settings::LoadSettings()
    {
        std::wifstream config_file(".\\config\\settings.conf");
        
        for (String command; std::getline(config_file, command); )
        {
            UpdateSettings(command, false);
        }

        return;
    }
    
    void Settings::SaveSettings()
    {
        String commands =
            decimal_separator.second + STR("\n") +
            output_mode.second + STR("\n") +
            input_language.second + STR("\n") +
            output_language.second + STR("\n");
        
        std::wofstream config_file(".\\config\\settings.conf");
        config_file << commands;

        return;
    }
    
    void Settings::Download()
    {
        system("start https://github.com/Thomilist/math-converter/releases");
        STRING_OUTPUT << STR("\nOpening download page in browser...\n") << std::endl;
        return;
    }
}