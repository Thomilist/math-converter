#include "Settings.hpp"



namespace mcon
{
    Settings::Settings():
        lexer(std::move(character_stream), character_set)
    {
        character_set->LoadFromFolder(".\\resources\\character-sets");
    }
    
    Settings::~Settings()
    { }
    
    void Settings::UpdateSettings(std::wstring a_console_input)
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
        Setting setting;
        
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
            if (    setting == Setting::Help    ||
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
                case Setting::Download:
                {
                    Download();
                    break;
                }
                case Setting::DecimalSeparator:
                {
                    decimal_separator = decimal_separator_settings.at(current_token.content);
                    STRING_OUTPUT << STR("Decimal separator updated.\n") << std::endl;
                    break;
                }
                case Setting::InputLanguage:
                {
                    input_language = input_language_settings.at(current_token.content);
                    STRING_OUTPUT << STR("Input language updated.\n") << std::endl;
                    break;
                }
                case Setting::OutputLanguage:
                {
                    output_language = output_language_settings.at(current_token.content);
                    STRING_OUTPUT << STR("Output language updated.\n") << std::endl;
                    break;
                }
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
        ERROR_OUTPUT  << STR("Command \"") << a_unknown_command << STR("\" not recognised.\n")
                    << STR("Use the command \"help\" to display help.\n");
        return;
    }
    
    void Settings::PrintHelp()
    {
        STRING_OUTPUT
        << STR("\n")
        << STR("Displaying help for math-converter\n")
        << STR("\n")
        << STR("Command format:\n")
        << STR("COMMAND ARGUMENT\n")
        << STR("\n")
        << STR("Available commands:\n")
        << STR("\n")
        << STR("Display help\n")
        << STR("COMMAND aliases:       h help ?\n")
        << STR("Available ARGUMENTs:   none\n")
        << STR("\n")
        << STR("Open download page\n")
        << STR("COMMAND aliases:       download\n")
        << STR("Available ARGUMENTs:   none\n")
        << STR("\n")
        << STR("Set decimal separator\n")
        << STR("COMMAND aliases:       d dec ds sep\n")
        << STR("Available ARGUMENTs:   period . comma ,\n")
        /*
        << STR("\n")
        << STR("Set input language\n")
        << STR("COMMAND aliases:       i in input\n")
        << STR("Available ARGUMENTs:   Mathcad LaTeX UnicodeMath MathML\n")
        << STR("\n")
        << STR("Set output language\n")
        << STR("COMMAND aliases:       o out output\n")
        << STR("Available ARGUMENTs:   Mathcad LaTeX UnicodeMath MathML\n")
        */
        << std::endl;
        
        return;
    }
    
    void Settings::Download()
    {
        system("start https://github.com/Thomilist/math-converter/releases");
        STRING_OUTPUT << STR("Opening download page in browser...\n") << std::endl;
        return;
    }
}