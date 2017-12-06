#include <string>
#include <vector>

#include "node.h"
#include "typedecl.h"

using std::string;

TypeDecl::TypeDecl(const string &identifier, Type *ref_type):
        identifier(identifier), ref_type(ref_type) {
                type = types::Alias;
        };
