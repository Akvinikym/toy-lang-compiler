#ifndef ILANG_TYPE_H
#define ILANG_TYPE_H

#include "node.h"
enum class types {
    Integer,
    Real,
    Boolean,
    Void,
    Undefined,
    Record,
    Array,
    Alias,
};

class Type : public ASTNode {
public:
    types type;

	bool operator==(types other) const {
		return other != types::Array && other != types::Record && this->type == other;
	}
    bool operator!=(types other) const {
        return !(*this == other);
    }
	virtual bool operator==(const Type &other) const {
		if (this->type != other.type) {
			return false;
		}
		if (this->type == types::Record) {
			return this == &other;
		}
		return true;
	}
    bool operator!=(const Type &other) const {
        return !(*this == other);
    }
	static bool isPrimitive(const Type &type) {
		return type == types::Boolean || type == types::Integer || type == types::Real;
	}
};

#endif //ILANG_TYPE_H
