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

            void AddChildNode();
            void DeleteChildNode(uint64_t a_index);

            NodeType type;
            std::wstring content;
            std::weak_ptr<Node> parent_node;
            uint64_t child_node_count = 0;
            std::vector<std::shared_ptr<Node>> child_nodes;

        private:

    };
}

#endif // __NODE_H__