
#include <string>
#include <vector>

#include "node.h"
#include "expression.h"
#include "var.h"

using std::string;
using std::pair;


Var::Var(const pair<string, Type*> &var_decl, Expression *body):
        var_decl(var_decl), body(body) {}