#ifndef __MATHCADGENERATOR_H__
#define __MATHCADGENERATOR_H__

// Standard libraries
#include <string>
#include <memory>

// Custom headers
#include "../Generator.hpp"

namespace mcon
{
    class MathcadGenerator : public Generator
    {
        public:
            MathcadGenerator(std::unique_ptr<CharacterSet> a_character_set);
            ~MathcadGenerator();

        private:
            
    };
}

#endif // __MATHCADGENERATOR_H__