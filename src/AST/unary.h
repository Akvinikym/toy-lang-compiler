#ifndef ILANG_UNARY_H
#define ILANG_UNARY_H

#include "expression.h"
#include "integer.h"
#include "real.h"
#include "../semantical_analysis/error.h"

class Unary: public Expression {
public:
    char opcode;
	Expression *operand;

    Unary(char opcode, Expression *operand);
    void accept(Visitor &v) { v.visit(*this); };
    // TODO: wait: "not" unary operator decoding
    // TODO: "not" unary operator logic
    Expression* eval() {
        if (opcode == '+') {
            return operand;
        }
        if (opcode == '-') {
            if (*operand->type == types::Integer) {
                return new Integer(-((Integer*)operand)->value);
            }
            if (*operand->type == types::Real) {
                return new Real(-((Real*)operand)->value);
            }
            reportError("error: Unary.eval: bad - operand");
        }
        reportError("error: Unary.eval: bad opcode");
    }
};

#endif //ILANG_UNARY_H
