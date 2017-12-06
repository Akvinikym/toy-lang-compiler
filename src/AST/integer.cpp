#include "integer.h"
#include "integertype.h"

Integer::Integer(int value) : value(value) {
    type = new IntegerType();
    isConst = true;
}