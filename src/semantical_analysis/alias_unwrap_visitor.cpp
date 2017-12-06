#include "alias_unwrap_visitor.h"
#include "error.h"

void AliasUnwrapVisitor::visit(Prototype& node) {
    unwrap(node.type);
    for (auto x : node.args) {
        x->accept(*this);
    }
}
void AliasUnwrapVisitor::visit(ArrayDecl& node) {
    unwrap(node.array_type);
}
void AliasUnwrapVisitor::visit(ArrayRef& node) {
    reportError("bug: AliasUnwrapVisitor: visit ArrayRef node");
}
void AliasUnwrapVisitor::visit(Assignment& node) {
}
void AliasUnwrapVisitor::visit(Binary& node) {
    reportError("bug: AliasUnwrapVisitor: visit Binary node");
}
void AliasUnwrapVisitor::visit(Boolean& node) {
    reportError("bug: AliasUnwrapVisitor: visit Boolean node");
}
void AliasUnwrapVisitor::visit(BooleanType& node) {
    reportError("bug: AliasUnwrapVisitor: visit BooleanType node");
}
void AliasUnwrapVisitor::visit(Cast& node) {
    reportError("bug: AliasUnwrapVisitor: visit Cast node");
}
void AliasUnwrapVisitor::visit(For& node) {
    node.body->accept(*this);
}
void AliasUnwrapVisitor::visit(If& node) {
    node.then->accept(*this);
    if (node.else_body) {
        node.else_body->accept(*this);
    }
}
void AliasUnwrapVisitor::visit(Integer& node) {
    reportError("bug: AliasUnwrapVisitor: visit Integer node");
}
void AliasUnwrapVisitor::visit(IntegerType& node) {
    reportError("bug: AliasUnwrapVisitor: visit IntegerType node");
}
void AliasUnwrapVisitor::visit(NamedRef& node) {
    reportError("bug: AliasUnwrapVisitor: visit NamedRef node");
}
void AliasUnwrapVisitor::visit(Program& node) {
    for (auto x : node.program) {
        x->accept(*this);
    }
}
void AliasUnwrapVisitor::visit(Real& node) {
    reportError("bug: AliasUnwrapVisitor: visit Real node");
}
void AliasUnwrapVisitor::visit(RealType& node) {
    reportError("bug: AliasUnwrapVisitor: visit RealType node");
}
void AliasUnwrapVisitor::visit(RecordDecl& node) {
    for (auto x : node.refs) {
        x->accept(*this);
    }
}
void AliasUnwrapVisitor::visit(RecordRef& node) {
    reportError("bug: AliasUnwrapVisitor: visit RecordRef node");
}
void AliasUnwrapVisitor::visit(Return& node) {
}
void AliasUnwrapVisitor::visit(Routine& node) {
    node.proto->accept(*this);
    node.body->accept(*this);
}
void AliasUnwrapVisitor::visit(RoutineCall& node) {
    // bug if inside expression
}
void AliasUnwrapVisitor::visit(Statements& node) {
    for (auto x : node.statements) {
        x->accept(*this);
    }
}
void AliasUnwrapVisitor::visit(TypeDecl& node) {
    unwrap(node.ref_type);
}
void AliasUnwrapVisitor::visit(Unary& node) {
    reportError("bug: AliasUnwrapVisitor: visit Unary node");
}
void AliasUnwrapVisitor::visit(Undefined& node) {
    reportError("bug: AliasUnwrapVisitor: visit Undefined node");
}
void AliasUnwrapVisitor::visit(Var& node) {
    unwrap(node.var_decl.second);
}
void AliasUnwrapVisitor::visit(Void& node) {
    reportError("bug: AliasUnwrapVisitor: visit Void node");
}
void AliasUnwrapVisitor::visit(While& node) {
    node.body->accept(*this);
}
void AliasUnwrapVisitor::unwrap(Type *&type) {
    while (type->type == types::Alias) {
        type = ((TypeDecl*)type)->ref_type;
    }
    if (type->type == types::Array || type->type == types::Record) {
        type->accept(*this);
    }
}
