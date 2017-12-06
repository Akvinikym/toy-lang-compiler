#ifndef ILANG_IF_H
#define ILANG_IF_H

#include "node.h"
#include "statements.h"

class If : public ASTNode {
public:
    Expression *condition;
    Statements *then, *else_body;

    If(Expression *condition, Statements *then, Statements *else_body) : condition(condition), then(then), else_body(else_body) {}
    If(Expression *condition, Statements *then) : condition(condition), then(then), else_body(nullptr) {}
    void accept(Visitor &v) { v.visit(*this); }
};


#endif //ILANG_IF_H
