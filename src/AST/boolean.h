#ifndef ILANG_BOOLEAN_H
#define ILANG_BOOLEAN_H

#include "expression.h"

class Boolean : public Expression {

public:
    bool value;
    Boolean(bool value);
    void accept(Visitor &v) { v.visit(*this); };
    Expression* eval() { return this; }
};

#endif //ILANG_BOOLEAN_H
