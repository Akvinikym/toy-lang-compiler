#include "node.h"
#include "while.h"

While::While(ASTNode *expression, Statements *body)
        :  expression(expression),body(body) {}