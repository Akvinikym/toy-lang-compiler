#ifndef ILANG_WHILE_H
#define ILANG_WHILE_H

#include "node.h"
#include "statements.h"

class While : public ASTNode {
public:
    ASTNode *expression;
    Statements *body;

    While(ASTNode *expression, Statements *body);
    void accept(Visitor &v) { v.visit(*this); };
};


#endif //ILANG_WHILE_H
