#include "node.h"
#include "unary.h"

Unary::Unary(char opcode, Expression *operand)
        : opcode(opcode), operand(operand) {}

