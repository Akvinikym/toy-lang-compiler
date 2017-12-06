#include "error.h"
#include <iostream>

void reportError(const std::string &text) {
	std::cerr << text;
    exit(-1);
}
