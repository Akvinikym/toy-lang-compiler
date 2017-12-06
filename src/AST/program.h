#ifndef ILANG_PROGRAM_H
#define ILANG_PROGRAM_H

#include <vector>
#include "node.h"

class Program : public ASTNode {
public:
    std::vector<ASTNode*> program;
    Program(const std::vector<ASTNode*> &program) : program(program) {};
    void accept(Visitor &v) { v.visit(*this); };
};

#endif //ILANG_PROGRAM_H
