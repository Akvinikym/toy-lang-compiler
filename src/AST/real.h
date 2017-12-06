#ifndef ILANG_REAL_H
#define ILANG_REAL_H

#include "expression.h"

class Real : public Expression {
public:
    double value;

    Real();
    Real(double value);
    void accept(Visitor &v) { v.visit(*this); };
    Expression* eval() { return this; }
};

#endif //ILANG_REAL_H
