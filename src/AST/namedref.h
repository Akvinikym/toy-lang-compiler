#ifndef ILANG_VARIABLE_H
#define ILANG_VARIABLE_H

#include "var.h"
#include "ref.h"

class NamedRef : public Ref {
public:
    Var *var;

    NamedRef(Var *var) : var(var) {
        type = var->var_decl.second;
    }
    void accept(Visitor &v) { v.visit(*this); }
};

#endif //ILANG_VARIABLE_H
