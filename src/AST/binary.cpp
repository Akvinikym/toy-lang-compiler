#include "node.h"
#include "binary.h"

Binary::Binary(opchars opchar, Expression *lhs, Expression *rhs):
        opchar(opchar), lhs(lhs), rhs(rhs) {}
        