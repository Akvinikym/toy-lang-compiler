#ifndef ILANG_EXPRESSION_H
#define ILANG_EXPRESSION_H


#include "node.h"
#include "type.h"
#include "undefined.h"
#include "../semantical_analysis/error.h"

class Expression : public ASTNode {
public:
    Type *type;
    bool isConst;
    Expression() : type(new Undefined()), isConst(false) {};
    virtual Expression* eval() {
        reportError("bug: Expression.eval: virtual");
        return nullptr;
    }
};

#endif //ILANG_EXPRESSION_H
