#ifndef ILANG_ARRAY_H
#define ILANG_ARRAY_H

#include <string>
#include "node.h"
#include "type.h"
#include "expression.h"

class ArrayDecl : public Type {

public:
    ASTNode *expression;
    Type *array_type;
    ArrayDecl(Expression *expression, Type *array_type);
    void accept(Visitor &v) { v.visit(*this); };
};

#endif //ILANG_ARRAY_H
