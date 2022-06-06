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
                std::wcerr << L"Out-of-range exception in " << e.what() << L"\n" << std::endl;
                return;
            }
        }
        else
        {
            CommandNotRecognised(current_token.content);
            std::wcerr << std::endl;
        }
        
        if (setting == Setting::Help)
        {
            PrintHelp();
            return;
        }

        current_token = lexer.Consume(0);

        // Read argument
        while (true)
        {
            if (current_token.type == TokenType::EndOfStream)
            {
                std::wcerr  << L"The command is incomplete. Please provide an argument.\n"
                            << L"Use the command \"help\" to display help.\n" << std::endl;
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
                case Setting::DecimalSeparator:
                {
                    decimal_separator = decimal_separator_settings.at(current_token.content);
                    std::wcout << L"Decimal separator updated.\n" << std::endl;
                    break;
                }
                case Setting::InputLanguage:
                {
                    input_language = input_language_settings.at(current_token.content);
                    std::wcout << L"Input language updated.\n" << std::endl;
                    break;
                }
                case Setting::OutputLanguage:
                {
                    output_language = output_language_settings.at(current_token.content);
                    std::wcout << L"Output language updated.\n" << std::endl;
                    break;
                }
            }
        }
        catch(const std::out_of_range& e)
        {
            CommandNotRecognised(current_token.content);
            std::wcerr << L"Out-of-range exception in " << e.what() << L"\n" << std::endl;
        }

        return;
    }
    
    void Settings::CommandNotRecognised(std::wstring a_unknown_command)
    {
        std::wcerr  << L"Command \"" << a_unknown_command << L"\" not recognised.\n"
                    << L"Use the command \"help\" to display help.\n";
        return;
    }
    
    void Settings::PrintHelp()
    {
        std::wcout
        << L"\n"
        << L"Displaying help for math-converter\n"
        << L"\n"
        << L"Command format:\n"
        << L"COMMAND ARGUMENT\n"
        << L"\n"
        << L"Available commands:\n"
        << L"\n"
        << L"Display help\n"
        << L"COMMAND aliases:       h help ?\n"
        << L"Available ARGUMENTs:   none\n"
        << L"\n"
        << L"Set decimal separator\n"
        << L"COMMAND aliases:       d dec ds sep\n"
        << L"Available ARGUMENTs:   period . comma ,\n"
        << L"\n"
        << L"Set input language\n"
        << L"COMMAND aliases:       i in input\n"
        << L"Available ARGUMENTs:   Mathcad LaTeX UnicodeMath MathML\n"
        << L"\n"
        << L"Set output language\n"
        << L"COMMAND aliases:       o out output\n"
        << L"Available ARGUMENTs:   Mathcad LaTeX UnicodeMath MathML\n"
        << std::endl;
        
        return;
    }
}