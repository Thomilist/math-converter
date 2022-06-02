#ifndef __PARSINGTREE_H__
#define __PARSINGTREE_H__

// Standard libraries
#include <string>
#include <memory>

// Custom headers
#include "Node.hpp"
#include "../Parser.hpp"
#include "../Generator.hpp"

namespace mcon
{
    // Forward declaration to work around circular dependency
    class Parser;
    class Generator;
    
    class ParsingTree : public std::enable_shared_from_this<ParsingTree>
    {
        public:
            ParsingTree(    std::unique_ptr<Parser> a_parser, 
                            std::unique_ptr<Generator> a_generator  );
            ~ParsingTree();

            void SetCurrentNode(std::shared_ptr<Node> a_node);
            void Reset();
            void Clean(std::shared_ptr<Node> a_node);

            std::unique_ptr<Parser> parser;
            std::unique_ptr<Generator> generator;
            std::shared_ptr<Node> root_node;
            std::weak_ptr<Node> current_node;
            std::wstring output;
            std::wstring decimal_point = L".";

        private:

    };
}

#endif // __PARSINGTREE_H__