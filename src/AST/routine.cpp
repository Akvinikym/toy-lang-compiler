#include "node.h"
#include "prototype.h"
#include "routine.h"

Routine::Routine(Prototype *proto, Statements *body): proto(proto), body(body) {}