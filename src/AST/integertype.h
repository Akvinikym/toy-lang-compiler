#ifndef ILANG_INTEGERTYPE_H
#define ILANG_INTEGERTYPE_H

#include "type.h"

class IntegerType : public Type {

public:
    IntegerType() {
        type = types::Integer;
    };
    void accept(Visitor &v) { v.visit(*this); };
};

#endif //ILANG_INTEGERTYPE_H
