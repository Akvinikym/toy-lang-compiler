#ifndef ILANG_RECORDDECL_H
#define ILANG_RECORDDECL_H

#include <vector>

#include "type.h"

using std::vector;

class RecordDecl : public Type {
public:
    vector<ASTNode*> refs;

    RecordDecl(const vector<ASTNode*> &refs): refs{refs} {type = types::Record;};
    void accept(Visitor &v) { v.visit(*this); };
};

#endif //ILANG_RECORDDECL_H
