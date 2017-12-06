#include "real.h" 
#include "realtype.h"

Real::Real() : value(0.0) {} 
Real::Real(double value) : value(value) {
    type = new RealType();
    isConst = true;
}