#ifndef ILANG_REALTYPE_H
#define ILANG_REALTYPE_H

#include "type.h"

class RealType : public Type {

public:
    RealType() {
        type = types::Real;
    };
    void accept(Visitor &v) { v.visit(*this); };
};

#endif //ILANG_REALTYPE_H
