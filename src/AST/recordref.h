#ifndef ILANG_RECORDREF_H
#define ILANG_RECORDREF_H

#include <string>
#include "ref.h"
#include "expression.h"

class RecordRef : public Ref {
public:
    std::string ref;

    RecordRef (std::string ref, Ref *prev): ref{ref} {
        this->prev = prev;
    }
    void accept(Visitor &v) { v.visit(*this); }
};

#endif //ILANG_RECORDREF_H
