#include "const_eval_visitor.h"
#include "error.h"

void ConstEvalVisitor::visit(Prototype& node) {
}
void ConstEvalVisitor::visit(ArrayDecl& node) {
    unwrap(node.expression);
}
void ConstEvalVisitor::visit(ArrayRef& node) {
    node.prev->accept(*this);
    unwrap(node.pos);
}
void ConstEvalVisitor::visit(Assignment& node) {
    node.ref->accept(*this);
    unwrap(node.value);
}
void ConstEvalVisitor::visit(Binary& node) {
    unwrap(node.lhs);
    unwrap(node.rhs);
    node.isConst = node.lhs->isConst && node.rhs->isConst;
}
void ConstEvalVisitor::visit(Boolean& node) {
}
void ConstEvalVisitor::visit(BooleanType& node) {
    reportError("bug: ConstEvalVisitor: visit BooleanType node");
}
void ConstEvalVisitor::visit(Cast& node) {
    unwrap(node.value);
    node.isConst = node.value->isConst;
}
void ConstEvalVisitor::visit(For& node) {
    unwrap(node.start);
    unwrap(node.end);
    node.body->accept(*this);
}
void ConstEvalVisitor::visit(If& node) {
    unwrap(node.condition);
    node.then->accept(*this);
    if (node.else_body) {
        node.else_body->accept(*this);
    }
}
void ConstEvalVisitor::visit(Integer& node) {
}
void ConstEvalVisitor::visit(IntegerType& node) {
    reportError("bug: ConstEvalVisitor: visit IntegerType node");
}
void ConstEvalVisitor::visit(NamedRef& node) {
}
void ConstEvalVisitor::visit(Program& node) {
    for (auto x : node.program) {
        x->accept(*this);
    }
}
void ConstEvalVisitor::visit(Real& node) {
}
void ConstEvalVisitor::visit(RealType& node) {
    reportError("bug: ConstEvalVisitor: visit RealType node");
}
void ConstEvalVisitor::visit(RecordDecl& node) {
    for (auto x : node.refs) {
        x->accept(*this);
    }
}
void ConstEvalVisitor::visit(RecordRef& node) {
    node.prev->accept(*this);
}
void ConstEvalVisitor::visit(Return& node) {
    unwrap(node.expression);
}
void ConstEvalVisitor::visit(Routine& node) {
    node.body->accept(*this);
}
void ConstEvalVisitor::visit(RoutineCall& node) {
    for (auto &x : node.args) {
        unwrap(x);
    }
}
void ConstEvalVisitor::visit(Statements& node) {
    for (auto x : node.statements) {
        x->accept(*this);
    }
}
void ConstEvalVisitor::visit(TypeDecl& node) {
}
// TODO: wait: "not" unary operator decoding
// TODO: "not" unary operator logic
void ConstEvalVisitor::visit(Unary& node) {
    unwrap(node.operand);
    node.isConst = node.operand->isConst;
}
void ConstEvalVisitor::visit(Undefined& node) {
    reportError("bug: ConstEvalVisitor: visit Undefined node");
}
void ConstEvalVisitor::visit(Var& node) {
    if (node.body) {
        node.body->accept(*this);
    }
}
void ConstEvalVisitor::visit(Void& node) {
    reportError("bug: ConstEvalVisitor: visit Void node");
}
void ConstEvalVisitor::visit(While& node) {
    unwrap(node.expression);
    node.body->accept(*this);
}
void ConstEvalVisitor::unwrap(ASTNode *&value) {
    unwrap((Expression*&)value);
}
void ConstEvalVisitor::unwrap(Expression *&value) {
    value->accept(*this);
    if (value->isConst) {
        value = value->eval();
    }
}
