#ifndef ILANG_ASSIGNMENT_H
#define ILANG_ASSIGNMENT_H

#include "node.h"
#include "expression.h"
#include "ref.h"

class Assignment : public ASTNode {
public:

    Ref *ref;
    Expression *value;
    types lhs_type;

    Assignment(Ref *ref, Expression *value, types lhs_type);
    void accept(Visitor &v) { v.visit(*this); };
};
#endif //ILANG_ASSIGNMENT_H
