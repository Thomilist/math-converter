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
    class ParsingTree
    {
        public:
            ParsingTree(    std::unique_ptr<Parser> a_parser, 
                            std::unique_ptr<Generator> a_generator  );
            ~ParsingTree();

            void set_current_node(std::shared_ptr<Node> a_node);

            std::unique_ptr<Parser> parser;
            std::unique_ptr<Generator> generator;
            std::shared_ptr<Node> root_node;
            std::weak_ptr<Node> current_node;

        private:

    };
}

#endif // __PARSINGTREE_H__