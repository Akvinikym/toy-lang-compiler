#ifndef ILANG_RETURN_H
#define ILANG_RETURN_H

#include "node.h"
#include "expression.h"

class Return : public ASTNode {
public:
    ASTNode *expression;
    Return(ASTNode *expression);
    void accept(Visitor &v) { v.visit(*this); };
};
#endif //ILANG_RETURN_H
