#ifndef ILANG_PROTOTYPE_H
#define ILANG_PROTOTYPE_H
#include <string>
#include <vector>

#include "node.h"
#include "type.h"

using std::string;
using std::vector;

class Prototype : public ASTNode {
public:
    string name;
    Type *type;
    vector<Var*> args;

    Prototype(const string &name, const vector<Var*> &args, Type *type);
    const string &getName() const { return name; }
    const vector<Var*>& getArgs() { return args; };
    void accept(Visitor &v) { v.visit(*this); };
};
#endif //ILANG_PROTOTYPE_H
