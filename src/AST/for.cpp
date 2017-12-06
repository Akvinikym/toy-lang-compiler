#include <string>

#include "node.h"
#include "for.h"

using std::string;

For::For(ASTNode *name,
         ASTNode *start, ASTNode *end,
         Statements *body, bool reverse)
        : name(name), start(start), end(end), body(body), reverse(reverse) {}
        