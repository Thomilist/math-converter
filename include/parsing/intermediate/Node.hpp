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
    class Node : public std::enable_shared_from_this<Node>
    {
        public:
            Node();
            ~Node();

            void add_child_node();
            void delete_child_node(uint64_t a_index);

            NodeType type;
            std::string content;
            std::weak_ptr<Node> parent_node;
            uint64_t child_node_count;
            std::vector<std::shared_ptr<Node>> child_nodes;

        private:

    };
}

#endif // __NODE_H__