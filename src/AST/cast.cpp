#include "cast.h"

Cast::Cast(Expression* expression, Type* target_type) : value(expression) {
    type = target_type;
}