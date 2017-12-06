#ifndef ILANG_FOR_H
#define ILANG_FOR_H
#include <string>

#include "node.h"
#include "statements.h"

class For: public ASTNode {
public:
    ASTNode *name, *start, *end;
    bool reverse;
    Statements *body;
    For(ASTNode *name, ASTNode *start, ASTNode *end, Statements *body, bool reverse);
    void accept(Visitor &v) { v.visit(*this); };
};


#endif //ILANG_FOR_H
