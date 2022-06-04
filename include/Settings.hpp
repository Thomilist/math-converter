#ifndef __SETTINGS_H__
#define __SETTINGS_H__

// Standard libraries
#include <string>
#include <memory>
#include <unordered_map>

// Custom headers
#include "Lexer.hpp"

namespace mcon
{
    enum class Setting
    {
        Help,
        DecimalSeparator,
        InputLanguage,
        OutputLanguage
    };
    
    enum class DecimalSeparator
    {
        Period,
        Comma
    };

    enum class InputLanguage
    {
        Mathcad,
        Latex,
        UnicodeMath,
        MathML
    };

    enum class OutputLanguage
    {
        Mathcad,
        Latex,
        UnicodeMath,
        MathML
    };
    
    class Settings : public std::enable_shared_from_this<Settings>
    {
        public:
            Settings();
            ~Settings();
            
            void UpdateSettings(std::wstring a_console_input);

            DecimalSeparator decimal_separator;
            InputLanguage input_language;
            OutputLanguage output_language;

        private:
            void CommandNotRecognised(std::wstring a_unknown_command);
            void PrintHelp();

            std::unique_ptr<CharacterStream> character_stream = std::make_unique<CharacterStream>();
            std::shared_ptr<CharacterSet> character_set = std::make_shared<CharacterSet>();
            Lexer lexer;

            const std::unordered_map<std::wstring, Setting> commands = 
            {
                {L"h",      Setting::Help},
                {L"help",   Setting::Help},
                {L"?",      Setting::Help},
                {L"d",      Setting::DecimalSeparator},
                {L"dec",    Setting::DecimalSeparator},
                {L"ds",     Setting::DecimalSeparator},
                {L"sep",    Setting::DecimalSeparator},
                {L"i",      Setting::InputLanguage},
                {L"in",     Setting::InputLanguage},
                {L"input",  Setting::InputLanguage},
                {L"o",      Setting::OutputLanguage},
                {L"out",    Setting::OutputLanguage},
                {L"output", Setting::OutputLanguage},
            };

            const std::unordered_map<std::wstring, DecimalSeparator> decimal_separator_settings = 
            {
                {L"period",     DecimalSeparator::Period},
                {L".",          DecimalSeparator::Period},
                {L"comma",      DecimalSeparator::Comma},
                {L",",          DecimalSeparator::Comma},
            };

            const std::unordered_map<std::wstring, InputLanguage> input_language_settings = 
            {
                {L"Mathcad",        InputLanguage::Mathcad},
                {L"mathcad",        InputLanguage::Mathcad},
                {L"LaTeX",          InputLanguage::Latex},
                {L"Latex",          InputLanguage::Latex},
                {L"latex",          InputLanguage::Latex},
                {L"UnicodeMath",    InputLanguage::UnicodeMath},
                {L"unicodemath",    InputLanguage::UnicodeMath},
                {L"Unicode",        InputLanguage::UnicodeMath},
                {L"unicode",        InputLanguage::UnicodeMath},
                {L"MathML",         InputLanguage::MathML},
                {L"mathml",         InputLanguage::MathML},
            };

            const std::unordered_map<std::wstring, OutputLanguage> output_language_settings = 
            {
                {L"Mathcad",        OutputLanguage::Mathcad},
                {L"mathcad",        OutputLanguage::Mathcad},
                {L"LaTeX",          OutputLanguage::Latex},
                {L"Latex",          OutputLanguage::Latex},
                {L"latex",          OutputLanguage::Latex},
                {L"UnicodeMath",    OutputLanguage::UnicodeMath},
                {L"unicodemath",    OutputLanguage::UnicodeMath},
                {L"Unicode",        OutputLanguage::UnicodeMath},
                {L"unicode",        OutputLanguage::UnicodeMath},
                {L"MathML",         OutputLanguage::MathML},
                {L"mathml",         OutputLanguage::MathML},
            };
    };
}

#endif // __SETTINGS_H__