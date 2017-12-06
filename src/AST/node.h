#ifndef ILANG_NODE_H
#define ILANG_NODE_H

#include "../visitor.h"

class ASTNode {
public:
    virtual ~ASTNode() {};
    virtual void accept(Visitor &v) = 0;
};

#endif //ILANG_NODE_H
