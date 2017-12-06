#ifndef ILANG_REF_H
#define ILANG_REF_H

#include <string>

#include "expression.h"

using std::string;

class Ref : public Expression {
public:
    Ref *next;
    Ref *prev;

    Ref() : next(nullptr), prev(nullptr) {}
    void add_next(Ref *node){
        next = node;
    }
    void add_prev(Ref *node){
        prev = node;   
    }
};

#endif //ILANG_REF_H
