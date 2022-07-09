#ifndef __SETTINGS_H__
#define __SETTINGS_H__

// Standard libraries
#include <string>
#include <memory>
#include <unordered_map>
#include <fstream>

// Definitions and forward declarations
#include "Definitions.hpp"
#include "ForwardDeclarations.hpp"

// Custom headers
#include "Lexer.hpp"

namespace mcon
{
    enum class Setting
    {
        Help,
        ShowSettings,
        ShowGuide,
        Download,
        DecimalSeparator,
        InputLanguage,
        OutputLanguage,
        OutputMode
    };
    
    enum class DecimalSeparator
    {
        Period,
        Comma
    };

    enum class InputLanguage
    {
        Mathcad,
        /*
        Latex,
        MathML,
        UnicodeMath,
        AsciiMath,
        Mathematica,
        Excel,
        MathJSON
        */
    };

    enum class OutputLanguage
    {
        Mathcad,
        Latex,
        MathML,
        UnicodeMath,
        /*
        AsciiMath,
        Mathematica,
        Excel,
        MathJSON
        */
    };

    enum class OutputMode
    {
        Keystrokes,
        Clipboard
    };
    
    class Settings : public std::enable_shared_from_this<Settings>
    {
        public:
            Settings();
            ~Settings();
            
            void UpdateSettings(String a_console_input, bool a_user_triggered);
            void ShowSettings();
            void LoadSettings();
            void SaveSettings();

            std::pair<DecimalSeparator, String> decimal_separator = {DecimalSeparator::Period, STR("decimal period")};
            std::pair<OutputMode, String> output_mode = {OutputMode::Keystrokes, STR("mode keystrokes")};
            std::pair<InputLanguage, String> input_language = {InputLanguage::Mathcad, STR("input mathcad")};
            std::pair<OutputLanguage, String> output_language = {OutputLanguage::Latex, STR("output latex")};

        private:
            void CommandNotRecognised(String a_unknown_command);
            void PrintHelp();
            void Download();
            void ShowGuide();

            std::unique_ptr<CharacterStream> character_stream = std::make_unique<CharacterStream>();
            std::shared_ptr<CharacterSet> character_set = std::make_shared<CharacterSet>();
            Lexer lexer;

            const std::unordered_map<String, Setting> commands = 
            {
                {STR("h"),          Setting::Help},
                {STR("help"),       Setting::Help},
                {STR("?"),          Setting::Help},
                {STR("download"),   Setting::Download},
                {STR("s"),          Setting::ShowSettings},
                {STR("show"),       Setting::ShowSettings},
                {STR("settings"),   Setting::ShowSettings},
                {STR("g"),          Setting::ShowGuide},
                {STR("guide"),      Setting::ShowGuide},
                {STR("userguide"),  Setting::ShowGuide},
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
                {STR("mode"),       Setting::OutputMode},
                {STR("m"),          Setting::OutputMode},
            };

            const std::unordered_map<DecimalSeparator, String> decimal_separator_names =
            {
                {DecimalSeparator::Period,  STR("Period (.)")},
                {DecimalSeparator::Comma,   STR("Comma (,)")},
            };

            const std::unordered_map<String, DecimalSeparator> decimal_separator_settings = 
            {
                {STR("period"),     DecimalSeparator::Period},
                {STR("."),          DecimalSeparator::Period},
                {STR("comma"),      DecimalSeparator::Comma},
                {STR(","),          DecimalSeparator::Comma},
            };

            const std::unordered_map<OutputMode, String> output_mode_names =
            {
                {OutputMode::Keystrokes,    STR("Keystrokes")},
                {OutputMode::Clipboard,     STR("Clipboard")},
            };

            const std::unordered_map<String, OutputMode> output_mode_settings = 
            {
                {STR("keystrokes"),     OutputMode::Keystrokes},
                {STR("keys"),           OutputMode::Keystrokes},
                {STR("clipboard"),      OutputMode::Clipboard},
                {STR("clip"),           OutputMode::Clipboard},
            };

            const std::unordered_map<InputLanguage, String> input_language_names =
            {
                {InputLanguage::Mathcad,        STR("Mathcad")},
                /*
                {InputLanguage::Latex,          STR("LaTeX")},
                {InputLanguage::UnicodeMath,    STR("UnicodeMath")},
                {InputLanguage::AsciiMath,      STR("AsciiMath")},
                {InputLanguage::MathML,         STR("MathML")},
                {InputLanguage::Mathematica,    STR("Mathematica")},
                {InputLanguage::Excel,          STR("Excel")},
                {InputLanguage::MathJSON,       STR("MathJSON")},
                */
            };

            const std::unordered_map<String, InputLanguage> input_language_settings = 
            {
                {STR("Mathcad"),        InputLanguage::Mathcad},
                {STR("mathcad"),        InputLanguage::Mathcad},
                /*
                {STR("LaTeX"),          InputLanguage::Latex},
                {STR("Latex"),          InputLanguage::Latex},
                {STR("latex"),          InputLanguage::Latex},
                {STR("UnicodeMath"),    InputLanguage::UnicodeMath},
                {STR("unicodemath"),    InputLanguage::UnicodeMath},
                {STR("Unicode"),        InputLanguage::UnicodeMath},
                {STR("unicode"),        InputLanguage::UnicodeMath},
                {STR("AsciiMath"),      InputLanguage::AsciiMath},
                {STR("asciimath"),      InputLanguage::AsciiMath},
                {STR("Ascii"),          InputLanguage::AsciiMath},
                {STR("ASCII"),          InputLanguage::AsciiMath},
                {STR("ascii"),          InputLanguage::AsciiMath},
                {STR("MathML"),         InputLanguage::MathML},
                {STR("mathml"),         InputLanguage::MathML},
                {STR("Mathematica"),    InputLanguage::Mathematica},
                {STR("mathematica"),    InputLanguage::Mathematica},
                {STR("Excel"),          InputLanguage::Excel},
                {STR("excel"),          InputLanguage::Excel},
                {STR("MathJSON"),       InputLanguage::MathJSON},
                {STR("mathjson"),       InputLanguage::MathJSON},
                {STR("JSON"),           InputLanguage::MathJSON},
                {STR("json"),           InputLanguage::MathJSON},
                */
            };

            const std::unordered_map<OutputLanguage, String> output_language_names =
            {
                {OutputLanguage::Mathcad,       STR("Mathcad")},
                {OutputLanguage::Latex,         STR("LaTeX")},
                {OutputLanguage::MathML,        STR("MathML")},
                {OutputLanguage::UnicodeMath,   STR("UnicodeMath")},
                /*
                {OutputLanguage::AsciiMath,     STR("AsciiMath")},
                {OutputLanguage::Mathematica,   STR("Mathematica")},
                {OutputLanguage::Excel,         STR("Excel")},
                {OutputLanguage::MathJSON,      STR("MathJSON")},
                */
            };

            const std::unordered_map<String, OutputLanguage> output_language_settings = 
            {
                {STR("Mathcad"),        OutputLanguage::Mathcad},
                {STR("mathcad"),        OutputLanguage::Mathcad},
                {STR("LaTeX"),          OutputLanguage::Latex},
                {STR("Latex"),          OutputLanguage::Latex},
                {STR("latex"),          OutputLanguage::Latex},
                {STR("MathML"),         OutputLanguage::MathML},
                {STR("mathml"),         OutputLanguage::MathML},
                {STR("UnicodeMath"),    OutputLanguage::UnicodeMath},
                {STR("unicodemath"),    OutputLanguage::UnicodeMath},
                {STR("Unicode"),        OutputLanguage::UnicodeMath},
                {STR("unicode"),        OutputLanguage::UnicodeMath},
                /*
                {STR("AsciiMath"),      OutputLanguage::AsciiMath},
                {STR("asciimath"),      OutputLanguage::AsciiMath},
                {STR("Ascii"),          OutputLanguage::AsciiMath},
                {STR("ASCII"),          OutputLanguage::AsciiMath},
                {STR("ascii"),          OutputLanguage::AsciiMath},
                {STR("Mathematica"),    OutputLanguage::Mathematica},
                {STR("mathematica"),    OutputLanguage::Mathematica},
                {STR("Excel"),          OutputLanguage::Excel},
                {STR("excel"),          OutputLanguage::Excel},
                {STR("MathJSON"),       OutputLanguage::MathJSON},
                {STR("mathjson"),       OutputLanguage::MathJSON},
                {STR("JSON"),           OutputLanguage::MathJSON},
                {STR("json"),           OutputLanguage::MathJSON},
                */
            };
    };
}

#endif // __SETTINGS_H__