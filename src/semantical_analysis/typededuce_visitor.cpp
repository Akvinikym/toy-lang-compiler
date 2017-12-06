#include "typededuce_visitor.h"
#include "error.h"

void TypeDeduceVisitor::visit(Prototype& node) {
    reportError("bug: TypeDeduceVisitor: visit Prototype node");
}
void TypeDeduceVisitor::visit(ArrayDecl& node) {
    node.expression->accept(*this);
}
void TypeDeduceVisitor::visit(ArrayRef& node) {
    node.prev->accept(*this);
    node.type = ((ArrayDecl*)node.prev->type)->array_type;
    node.pos->accept(*this);
}
void TypeDeduceVisitor::visit(Assignment& node) {
    node.ref->accept(*this);
    node.value->accept(*this);
}
void TypeDeduceVisitor::visit(Binary& node) {
    node.lhs->accept(*this);
    node.rhs->accept(*this);
    if (!Type::isPrimitive(*node.lhs->type) || !Type::isPrimitive(*node.rhs->type)) {
        reportError("error: TypeDeduceVisitor: binary with non-primitive");
    }
    BinaryOpcode op{node.opchar};
    if (op.isMath + op.isLogic + op.isCmp + op.isEq != 1) {
        reportError("bug: TypeDeduceVisitor: Binary node bad opcode");
    }
    auto ltype = node.lhs->type;
    auto rtype = node.rhs->type;
    if (op.isMath || op.isCmp || op.isEq) {
        if (*ltype == *rtype) {
            if (!op.isEq && *ltype == types::Boolean) {
                node.lhs = new Cast(node.lhs, new IntegerType());
                node.rhs = new Cast(node.rhs, new IntegerType());
            }
        } else {
            if (*ltype == types::Boolean || *rtype == types::Real) {
                node.lhs = new Cast(node.lhs, rtype);
            }
            if (*rtype == types::Boolean || *ltype == types::Real) {
                node.rhs = new Cast(node.rhs, ltype);
            }
        }
    }
    if (op.isLogic) {
        if (*ltype != types::Boolean) {
            node.lhs = new Cast(node.lhs, new BooleanType());
        }
        if (*rtype != types::Boolean) {
            node.rhs = new Cast(node.rhs, new BooleanType());
        }
    }
    if (*node.lhs->type != *node.rhs->type) {
        reportError("bug: TypeDeduceVisitor: Binary node math cmp non-equal");
    }
    node.type = op.isMath ? node.lhs->type : new BooleanType();
}
void TypeDeduceVisitor::visit(Boolean& node) {
}
void TypeDeduceVisitor::visit(BooleanType& node) {
    reportError("bug: TypeDeduceVisitor: visit BooleanType node");
}
void TypeDeduceVisitor::visit(Cast& node) {
    reportError("bug: TypeDeduceVisitor: visit cast node");
}
void TypeDeduceVisitor::visit(For& node) {
    node.start->accept(*this);
    node.end->accept(*this);
    node.body->accept(*this);
}
void TypeDeduceVisitor::visit(If& node) {
    node.condition->accept(*this);
    node.then->accept(*this);
    if (node.else_body) {
        node.else_body->accept(*this);
    }
}
void TypeDeduceVisitor::visit(Integer& node) {
}
void TypeDeduceVisitor::visit(IntegerType& node) {
    reportError("bug: TypeDeduceVisitor: visit IntegerType node");
}
void TypeDeduceVisitor::visit(NamedRef& node) {
    node.type = node.var->var_decl.second;
}
void TypeDeduceVisitor::visit(Program& node) {
    for (auto x : node.program) {
        x->accept(*this);
    }
}
void TypeDeduceVisitor::visit(Real& node) {
}
void TypeDeduceVisitor::visit(RealType& node) {
    reportError("bug: TypeDeduceVisitor: visit RealType node");
}
void TypeDeduceVisitor::visit(RecordDecl& node) {
}
void TypeDeduceVisitor::visit(RecordRef& node) {
    node.prev->accept(*this);
    for (auto x2 : ((RecordDecl*)node.prev->type)->refs) {
        auto x = (Var*)x2;
        if (x->var_decl.first == node.ref) {
            node.type = x->var_decl.second;
            return;
        }
    }
    reportError("error: TypeDeduceVisitor: RecordRef bad ref `" + node.ref + "`");
}
void TypeDeduceVisitor::visit(Return& node) {
    node.expression->accept(*this);
}
void TypeDeduceVisitor::visit(Routine& node) {
    node.body->accept(*this);
}
void TypeDeduceVisitor::visit(RoutineCall& node) {
    for (auto x : node.args) {
        x->accept(*this);
    }
    node.type = node.callee->proto->type;
}
void TypeDeduceVisitor::visit(Statements& node) {
    for (auto x : node.statements) {
        x->accept(*this);
    }
}
void TypeDeduceVisitor::visit(TypeDecl& node) {
}
// TODO: wait: "not" unary operator decoding
// TODO: "not" unary operator logic
void TypeDeduceVisitor::visit(Unary& node) {
    node.operand->accept(*this);
    if (strchr("+-", node.opcode)) {
        if (*node.operand->type == types::Boolean) {
            node.operand = new Cast(node.operand, new IntegerType());
        }
        node.type = node.operand->type;
        return;
    }
    reportError("bug: TypeDeduceVisitor: unexpected unary opcode");
}
void TypeDeduceVisitor::visit(Undefined& node) {
    reportError("bug: TypeDeduceVisitor: visit Undefined node");
}
void TypeDeduceVisitor::visit(Var& node) {
    if (node.body) {
        node.body->accept(*this);
    }
    if (*node.var_decl.second == types::Undefined) {
        node.var_decl.second = node.body->type;
    }
}
void TypeDeduceVisitor::visit(Void& node) {
    reportError("bug: TypeDeduceVisitor: visit Void node");
}
void TypeDeduceVisitor::visit(While& node) {
    node.expression->accept(*this);
    node.body->accept(*this);
}
