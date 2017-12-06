#include <vector>

#include "node.h"
#include "arraydecl.h"

ArrayDecl::ArrayDecl(Expression *expression, Type *array_type): expression(expression), array_type(array_type){
    type = types::Array;
}
