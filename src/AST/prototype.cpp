#include <string>
#include <vector>

#include "prototype.h"

using std::string;
using std::vector;

Prototype::Prototype(const string &name,
                     const vector<Var*> &args,
                     Type *type)
        : name(name), args(args), type(type) {}
