#ifndef ILANG_CAST_H
#define ILANG_CAST_H

#include "expression.h"
#include "boolean.h"
#include "integer.h"
#include "real.h"
#include "../semantical_analysis/error.h"

class Cast : public Expression {

public:
    Expression* value;

    Cast(Expression* expression, Type* type);
    void accept(Visitor &v) { v.visit(*this); };
    Expression* eval() {
        if (*type == types::Boolean) {
            if (*value->type == types::Integer) {
                return new Boolean(!!((Integer*)value)->value);
            }
            reportError("error: Cast.eval: bad Boolean cast");
        }
        if (*type == types::Integer) {
            if (*value->type == types::Boolean) {
                return new Integer(+((Boolean*)value)->value);
            }
            if (*value->type == types::Real) {
                return new Integer((int)((Real*)value)->value);
            }
            reportError("error: Cast.eval: bad Integer cast");
        }
        if (*type == types::Real) {
            if (*value->type == types::Boolean) {
                return new Real(((Boolean*)value)->value);
            }
            if (*value->type == types::Integer) {
                return new Real(((Integer*)value)->value);
            }
            reportError("error: Cast.eval: bad Real cast");
        }
        reportError("error: Cast.eval: bad cast");
    }
};

#endif //ILANG_CAST_H
