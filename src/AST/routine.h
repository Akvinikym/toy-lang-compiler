#ifndef ILANG_FUNCTION_H
#define ILANG_FUNCTION_H
#include <string>

#include "node.h"
#include "prototype.h"
#include "statements.h"

using std::string;

class Routine : public ASTNode {
public:
    Prototype *proto;
    Statements *body;

    Routine(Prototype *proto, Statements *body);
    void accept(Visitor &v) { v.visit(*this); };
};

#endif //ILANG_FUNCTION_H
