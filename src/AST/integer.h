#ifndef ILANG_INTEGER_H
#define ILANG_INTEGER_H

#include "expression.h"

class Integer : public Expression {

public:
    int value;
    Integer(const int value);
    void accept(Visitor &v) { v.visit(*this); };
    Expression* eval() { return this; }
};

#endif //ILANG_INTEGER_H
