#ifndef __MATHCADPARSER_H__
#define __MATHCADPARSER_H__

// Standard libraries
#include <string>
#include <memory>

// Custom headers
#include "../Parser.hpp"

namespace mcon
{
    class MathcadParser : public Parser
    {
        public:
            MathcadParser(std::unique_ptr<Lexer> a_lexer);
            ~MathcadParser();
        
        private:
            
    };
}

#endif // __MATHCADPARSER_H__