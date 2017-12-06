#ifndef ILANG_TYPEDECL_H
#define ILANG_TYPEDECL_H

#include <string>

#include "node.h"
#include "type.h"

class TypeDecl : public Type {

public:
    std::string identifier;
    Type *ref_type;
    TypeDecl(const std::string &identifier, Type *ref_type);
    void accept(Visitor &v) { v.visit(*this); };
};

#endif //ILANG_TYPEDECL_H
