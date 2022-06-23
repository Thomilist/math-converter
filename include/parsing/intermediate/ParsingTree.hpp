#ifndef __PARSINGTREE_H__
#define __PARSINGTREE_H__

// Standard libraries
#include <string>
#include <memory>
#include <unordered_map>

// Definitions and forward declarations
#include "../../Definitions.hpp"
#include "../../ForwardDeclarations.hpp"

// Custom headers
#include "Node.hpp"
#include "../Parser.hpp"
#include "../Generator.hpp"

namespace mcon
{
    class ParsingTree : public std::enable_shared_from_this<ParsingTree>
    {
        public:
            ParsingTree(    std::unordered_map<InputLanguage, std::unique_ptr<Parser>> a_parsers, 
                            std::unordered_map<OutputLanguage, std::unique_ptr<Generator>> a_generators
            );
            ~ParsingTree();

            void SetCurrentNode(std::shared_ptr<Node> a_node);
            void Reset();
            void Clean(std::shared_ptr<Node> a_node);

            std::unordered_map<InputLanguage, std::unique_ptr<Parser>> parsers;
            std::unordered_map<OutputLanguage, std::unique_ptr<Generator>> generators;
            std::shared_ptr<Node> root_node;
            std::weak_ptr<Node> current_node;
            String output;
            String decimal_separator = STR(".");

        private:

    };
}

#endif // __PARSINGTREE_H__