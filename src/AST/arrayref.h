#ifndef ILANG_ARRAYASSIGN_H
#define ILANG_ARRAYASSIGN_H

#include "expression.h"
#include "ref.h"

class ArrayRef : public Ref {
public:
    Expression *pos;

    ArrayRef(Expression *pos, Ref *prev): pos{pos} {
        this->prev = prev;
    }
    void accept(Visitor &v) { v.visit(*this); }
};

#endif //ILANG_ARRAYASSIGN_H
