#ifndef ILANG_UNDEFINED_H
#define ILANG_UNDEFINED_H

#include "type.h"

class Undefined : public Type {

public:
    Undefined() {
        type = types::Undefined;
    };
    void accept(Visitor &v) { v.visit(*this); };
};

#endif //ILANG_UNDEFINED_H
