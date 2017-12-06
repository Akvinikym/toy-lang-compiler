#ifndef ILANG_STATEMENT_H
#define ILANG_STATEMENT_H

#include <vector>
#include "node.h"

class Statements : public ASTNode {
public:
    std::vector<ASTNode*> statements;
    Statements(const std::vector<ASTNode*> &statements) : statements(statements) {};
    void accept(Visitor &v) { v.visit(*this); };
};

#endif //ILANG_STATEMENT_H
