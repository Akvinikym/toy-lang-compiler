#ifndef ILANG_ERRORS_H
#define ILANG_ERRORS_H

#include "../AST/node.h"
#include "../AST/prototype.h"
#include "../AST/routine.h"
#include "../AST/expression.h"


ASTNode *Error(const char *message);
ASTNode *Error(const char *message, const char *name);
Prototype *ErrorP(const char *message);
Routine *ErrorR(const char *message);
Expression *ErrorE(const char *message);

#endif //ILANG_ERRORS_H
