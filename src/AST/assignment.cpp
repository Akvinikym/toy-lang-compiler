#include "assignment.h"

Assignment::Assignment(Ref *ref, Expression *value, types lhs_type) : ref(ref), value(value), lhs_type{lhs_type} {}
