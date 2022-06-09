#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "definitions.hpp"

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
        Download,
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
        AsciiMath,
        MathML,
        Mathematica,
        Excel,
        MathJSON
    };

    enum class OutputLanguage
    {
        Mathcad,
        Latex,
        UnicodeMath,
        AsciiMath,
        MathML,
        Mathematica,
        Excel,
        MathJSON
    };
    
    class Settings : public std::enable_shared_from_this<Settings>
    {
        public:
            Settings();
            ~Settings();
            
            void UpdateSettings(String a_console_input);

            DecimalSeparator decimal_separator;
            InputLanguage input_language;
            OutputLanguage output_language;

        private:
            void CommandNotRecognised(String a_unknown_command);
            void PrintHelp();
            void Download();

            std::unique_ptr<CharacterStream> character_stream = std::make_unique<CharacterStream>();
            std::shared_ptr<CharacterSet> character_set = std::make_shared<CharacterSet>();
            Lexer lexer;

            const std::unordered_map<String, Setting> commands = 
            {
                {STR("h"),          Setting::Help},
                {STR("help"),       Setting::Help},
                {STR("?"),          Setting::Help},
                {STR("download"),   Setting::Download},
                {STR("d"),          Setting::DecimalSeparator},
                {STR("dec"),        Setting::DecimalSeparator},
                {STR("ds"),         Setting::DecimalSeparator},
                {STR("sep"),        Setting::DecimalSeparator},
                {STR("i"),          Setting::InputLanguage},
                {STR("in"),         Setting::InputLanguage},
                {STR("input"),      Setting::InputLanguage},
                {STR("o"),          Setting::OutputLanguage},
                {STR("out"),        Setting::OutputLanguage},
                {STR("output"),     Setting::OutputLanguage},
            };

            const std::unordered_map<String, DecimalSeparator> decimal_separator_settings = 
            {
                {STR("period"),     DecimalSeparator::Period},
                {STR("."),          DecimalSeparator::Period},
                {STR("comma"),      DecimalSeparator::Comma},
                {STR(","),          DecimalSeparator::Comma},
            };

            const std::unordered_map<String, InputLanguage> input_language_settings = 
            {
                {STR("Mathcad"),        InputLanguage::Mathcad},
                {STR("mathcad"),        InputLanguage::Mathcad},
                {STR("LaTeX"),          InputLanguage::Latex},
                {STR("Latex"),          InputLanguage::Latex},
                {STR("latex"),          InputLanguage::Latex},
                {STR("UnicodeMath"),    InputLanguage::UnicodeMath},
                {STR("unicodemath"),    InputLanguage::UnicodeMath},
                {STR("Unicode"),        InputLanguage::UnicodeMath},
                {STR("unicode"),        InputLanguage::UnicodeMath},
                {STR("MathML"),         InputLanguage::MathML},
                {STR("mathml"),         InputLanguage::MathML},
            };

            const std::unordered_map<String, OutputLanguage> output_language_settings = 
            {
                {STR("Mathcad"),        OutputLanguage::Mathcad},
                {STR("mathcad"),        OutputLanguage::Mathcad},
                {STR("LaTeX"),          OutputLanguage::Latex},
                {STR("Latex"),          OutputLanguage::Latex},
                {STR("latex"),          OutputLanguage::Latex},
                {STR("UnicodeMath"),    OutputLanguage::UnicodeMath},
                {STR("unicodemath"),    OutputLanguage::UnicodeMath},
                {STR("Unicode"),        OutputLanguage::UnicodeMath},
                {STR("unicode"),        OutputLanguage::UnicodeMath},
                {STR("MathML"),         OutputLanguage::MathML},
                {STR("mathml"),         OutputLanguage::MathML},
            };
    };
}

#endif // __SETTINGS_H__