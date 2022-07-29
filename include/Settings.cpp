#include "Settings.hpp"



namespace mcon
{
    Settings::Settings():
        lexer(std::move(character_stream), character_set)
    {
        character_set->LoadFromFolder("./resources/character-sets");
        LoadSettings();
    }
    
    Settings::~Settings()
    { }
    
    void Settings::UpdateSettings(String a_console_input, bool a_user_triggered)
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
                    setting == Setting::ShowGuide       ||
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
                case Setting::ShowGuide:
                {
                    ShowGuide();
                    break;
                }
                case Setting::Download:
                {
                    Download();
                    break;
                }
                case Setting::CheckUpdate:
                {
                    check_update.first = check_update_settings.at(current_token.content);
                    check_update.second = a_console_input;
                    if (a_user_triggered)
                    {
                        STRING_OUTPUT << STR("\nUpdate checking behavior updated.\n") << std::endl;
                    }
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
    
    void Settings::CommandNotRecognised(String a_unknown_command)
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
        << STR("----------------------------------------------------------------------------------------------------\n")
        << STR("\n")
        << STR("Additional information can be found in the associated user guide.\n")
        << STR("\n")
        << STR("Math conversion hotkey:\n")
        << STR("CTRL + SHIFT + V\n")
        << STR("\n")
        << STR("Supported formats:\n")
        << STR("\n")
        << STR("Format      Input   Output\n")
        << STR("Mathcad       X       X\n")
        << STR("LaTeX                 X\n")
        << STR("MathML                X\n")
        << STR("UnicodeMath           X\n")
        << STR("\n")
        << STR("   Command                  Command aliases     Possible arguments                      Example\n")
        << STR("----------------------------------------------------------------------------------------------------\n")
        << STR("1. Show help                h help ?            [no arguments]                          h\n")
        << STR("\n")
        << STR("2. Show current settings    s show settings     [no arguments]                          s\n")
        << STR("\n")
        << STR("3. Open user guide          g guide userguide   [no arguments]                          g\n")
        << STR("\n")
        << STR("4. Open download page       download            [no arguments]                          download\n")
        << STR("\n")
        << STR("5. Set update checking      check               enabled true                            check false\n")
        << STR("                                                disabled false\n")
        << STR("\n")
        << STR("6. Set decimal separator    d dec ds sep        period .                                d ,\n")
        << STR("                                                comma ,\n")
        << STR("\n")
        << STR("7. Set output mode          m mode              keys keystrokes                         m clip\n")
        << STR("                                                clip clipboard\n")
        << STR("\n")
        << STR("8. Set input format         i in input          Mathcad mathcad                         i mathcad\n")
        << STR("\n")
        << STR("9. Set output format        o out output        Mathcad mathcad                         o latex\n")
        << STR("                                                LaTeX Latex latex\n")
        << STR("                                                MathML mathml\n")
        << STR("                                                UnicodeMath unicodemath Unicode unicode\n")
        << STR("\n")
        << std::endl;
        
        return;
    }
    
    void Settings::ShowSettings()
    {
        String current_update_checking;
        String current_decimal_separator;
        String current_input_language;
        String current_output_language;
        String current_output_mode;

        try
        {
            current_update_checking = check_update_names.at(check_update.first);
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
        << STR("----------------------------------------------------------------------------------------------------\n")
        << STR("Update checking:    ")
        << current_update_checking
        << STR("\n")
        << STR("Decimal separator:  ")
        << current_decimal_separator
        << STR("\n")
        << STR("Output mode:        ")
        << current_output_mode
        << STR("\n")
        << STR("Input format:       ")
        << current_input_language
        << STR("\n")
        << STR("Output format:      ")
        << current_output_language
        << STR("\n")
        << std::endl;

        return;
    }
    
    void Settings::LoadSettings()
    {
        INPUT_FILE_STREAM config_file("./config/settings.conf");
        
        for (String command; std::getline(config_file, command); )
        {
            UpdateSettings(command, false);
        }

        return;
    }
    
    void Settings::SaveSettings()
    {
        String commands =
            check_update.second + STR("\n") +
            decimal_separator.second + STR("\n") +
            output_mode.second + STR("\n") +
            input_language.second + STR("\n") +
            output_language.second + STR("\n");
        
        OUTPUT_FILE_STREAM config_file("./config/settings.conf");
        config_file << commands;

        return;
    }
    
    void Settings::PrintTip()
    {
        STRING_OUTPUT
            << STR("For usage information, enter \"help\" (without quote marks) or read the associated user guide.")
            << std::endl;
        return;
    }
    
    void Settings::Download()
    {
        STRING_OUTPUT << STR("\nOpening download page in browser...\n") << std::endl;

        #ifdef WIN32
        (void)! system("start https://github.com/Thomilist/math-converter/releases/latest");
        #else
        (void)! system("xdg-open https://github.com/Thomilist/math-converter/releases/latest");
        #endif

        return;
    }
    
    void Settings::ShowGuide()
    {
        STRING_OUTPUT << STR("\nOpening user guide in browser...\n") << std::endl;

        #ifdef WIN32
        (void)! system("start https://github.com/Thomilist/math-converter/blob/main/userguide.md");
        #else
        (void)! system("xdg-open https://github.com/Thomilist/math-converter/blob/main/userguide.md");
        #endif

        return;
    }
}