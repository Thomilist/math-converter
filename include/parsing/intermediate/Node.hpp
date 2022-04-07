#ifndef __NODE_H__
#define __NODE_H__

// Standard libraries
#include <string>
#include <vector>
#include <memory>

// Custom headers
#include "NodeTypes.hpp"

namespace mcon
{
    class Node
    {
        public:
            Node();
            ~Node();

            void append_node(std::unique_ptr<Node> a_node);
            void delete_node(uint64_t a_index);

            NodeType type;
            std::string content;
            uint64_t child_node_count;
            std::vector<std::unique_ptr<Node>> child_nodes;

        private:

    };
}

#endif // __NODE_H__