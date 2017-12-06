#ifndef ILANG_VOID_H
#define ILANG_VOID_H

#include "type.h"

class Void : public Type {

public:
    Void() {
        type = types::Void;
    };
    void accept(Visitor &v) { v.visit(*this); };
};

#endif //ILANG_VOID_H
