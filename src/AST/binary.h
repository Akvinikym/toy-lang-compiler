#ifndef ILANG_BINARY_H
#define ILANG_BINARY_H

#include "expression.h"
#include "real.h"
#include "integer.h"
#include "boolean.h"
#include "../parsing/Lexer.h" //Move opchars globally

class BinaryOpcode {
public:
    opchars op;
    bool isMath;
    bool isLogic;
    bool isCmp;
    bool isEq;

    BinaryOpcode(opchars op) : op(op) {
        isMath = op == opchars::DIV || op == opchars::MINUS || op == opchars::MOD || op == opchars::MUL || op == opchars::PLUS;
        isLogic = op == opchars::AND || op == opchars::OR || op == opchars::XOR;
        isCmp = op == opchars::HIGH || op == opchars::HIGHEQ || op == opchars::LESS || op == opchars::LESSEQ;
        isEq = op == opchars::EQUAL || op == opchars::NOTEQ;
    }
};

class Binary : public Expression {
public:
    opchars opchar;
    Expression *lhs, *rhs;

    Binary(opchars opchar, Expression *lhs, Expression *rhs);
    void accept(Visitor &v) { v.visit(*this); };
    Expression* eval() {
        BinaryOpcode op{opchar};
        if (*type == types::Real) {
            return new Real(eval(opchar, ((Real*)lhs)->value, ((Real*)rhs)->value));
        }
        if (*type == types::Integer) {
            return new Integer(eval(opchar, ((Integer*)lhs)->value, ((Integer*)rhs)->value));
        }
        if (*type == types::Boolean) {
            return new Boolean(eval(opchar, ((Boolean*)lhs)->value, ((Boolean*)rhs)->value));
        }
        reportError("error: Binary.eval: bad type");
    }
    static double eval(opchars op, double l, double r) {
        switch (op) {
        case opchars::EQUAL: return l == r;
        case opchars::LESS: return l < r;
        case opchars::HIGH: return l > r;
        case opchars::LESSEQ: return l <= r;
        case opchars::HIGHEQ: return l >= r;
        case opchars::NOTEQ: return l != r;
        case opchars::PLUS: return l + r;
        case opchars::MINUS: return l - r;
        case opchars::MUL: return l * r;
        case opchars::DIV: return l / r;
        }
        reportError("error: Binary.eval<Real>: bad opcode");
    }
    static int eval(opchars op, int l, int r) {
        switch (op) {
        case opchars::EQUAL: return l == r;
        case opchars::LESS: return l < r;
        case opchars::HIGH: return l > r;
        case opchars::LESSEQ: return l <= r;
        case opchars::HIGHEQ: return l >= r;
        case opchars::NOTEQ: return l != r;
        case opchars::PLUS: return l + r;
        case opchars::MINUS: return l - r;
        case opchars::MUL: return l * r;
        case opchars::DIV: return l / r;
        case opchars::MOD: return l % r;
        }
        reportError("error: Binary.eval<Integer>: bad opcode");
    }
    static bool eval(opchars op, bool l, bool r) {
        switch (op) {
        case opchars::EQUAL: return l == r;
        case opchars::XOR:
        case opchars::NOTEQ: return l != r;
        case opchars::AND: return l && r;
        case opchars::OR: return l || r;
        }
        reportError("error: Binary.eval<Boolean>: bad opcode");
    }
};

#endif //ILANG_BINARY_H
