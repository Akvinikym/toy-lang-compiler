#ifndef ILANG_ROUTINECALL_H
#define ILANG_ROUTINECALL_H

#include <vector>

#include "expression.h"

using std::vector;

class RoutineCall : public Expression {
public:
    Routine *callee;
    vector<Expression*> args;

    RoutineCall(Routine *callee, vector<Expression*> &args);
    void accept(Visitor &v) { v.visit(*this); };
};

#endif //ILANG_ROUTINECALL_H
