#include "boolean.h"
#include "booleantype.h"

Boolean::Boolean(bool value) : value(value) {
    type = new BooleanType();
    isConst = true;
}
