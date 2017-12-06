#ifndef ILANG_VISITOR_H
#define ILANG_VISITOR_H

class Prototype;
class ArrayDecl;
class Assignment;
class Binary;
class Boolean;
class BooleanType;
class Cast;
class For;
class If;
class Integer;
class IntegerType;
class Real;
class RealType;
class RecordDecl;
class Routine;
class RoutineCall;
class TypeDecl;
class Unary;
class Undefined;
class Var;
class NamedRef;
class While;
class Return;
class RecordRef;
class ArrayRef;
class Program;
class Statements;
class Void;

class Visitor
{
public:
    virtual void visit(Prototype& node) = 0;
    virtual void visit(ArrayDecl& node) = 0;
    virtual void visit(Assignment& node) = 0;
    virtual void visit(Binary& node) = 0;
    virtual void visit(Boolean& node) = 0;
    virtual void visit(BooleanType& node) = 0;
    virtual void visit(Cast& node) = 0;
    virtual void visit(For& node) = 0;
    virtual void visit(If& node) = 0;
    virtual void visit(Integer& node) = 0;
    virtual void visit(IntegerType& node) = 0;
    virtual void visit(Real& node) = 0;
    virtual void visit(RealType& node) = 0;
    virtual void visit(RecordDecl& node) = 0;
    virtual void visit(Routine& node) = 0;
    virtual void visit(RoutineCall& node) = 0;
    virtual void visit(TypeDecl& node) = 0;
    virtual void visit(Unary& node) = 0;
    virtual void visit(Undefined& node) = 0;
    virtual void visit(Var& node) = 0;
    virtual void visit(NamedRef& node) = 0;
    virtual void visit(While& node) = 0;
    virtual void visit(Return& node) = 0;
    virtual void visit(RecordRef& node) = 0;
    virtual void visit(ArrayRef& node) = 0;
    virtual void visit(Program& node) = 0;
    virtual void visit(Statements& node) = 0;
    virtual void visit(Void& node) = 0;
};

#endif