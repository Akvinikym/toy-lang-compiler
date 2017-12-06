#include "../visitor.h"

#include "../AST/prototype.h"
#include "../AST/arraydecl.h"
#include "../AST/arrayref.h"
#include "../AST/assignment.h"
#include "../AST/binary.h"
#include "../AST/boolean.h"
#include "../AST/booleantype.h"
#include "../AST/cast.h"
#include "../AST/for.h"
#include "../AST/if.h"
#include "../AST/integer.h"
#include "../AST/integertype.h"
#include "../AST/namedref.h"
#include "../AST/real.h"
#include "../AST/realtype.h"
#include "../AST/recorddecl.h"
#include "../AST/recordref.h"
#include "../AST/return.h"
#include "../AST/routine.h"
#include "../AST/routinecall.h"
#include "../AST/program.h"
#include "../AST/statements.h"
#include "../AST/typedecl.h"
#include "../AST/unary.h"
#include "../AST/var.h"
#include "../AST/void.h"
#include "../AST/while.h"

class ConstEvalVisitor : public Visitor
{
public:
    void visit(Prototype& node);
    void visit(ArrayDecl& node);
    void visit(ArrayRef& node);
    void visit(Assignment& node);
    void visit(Binary& node);
    void visit(Boolean& node);
    void visit(BooleanType& node);
    void visit(Cast& node);
    void visit(For& node);
    void visit(If& node);
    void visit(Integer& node);
    void visit(IntegerType& node);
    void visit(NamedRef& node);
    void visit(Program& node);
    void visit(Real& node);
    void visit(RealType& node);
    void visit(RecordDecl& node);
    void visit(RecordRef& node);
    void visit(Return& node);
    void visit(Routine& node);
    void visit(RoutineCall& node);
    void visit(Statements& node);
    void visit(TypeDecl& node);
    void visit(Unary& node);
    void visit(Undefined& node);
    void visit(Var& node);
    void visit(Void& node);
    void visit(While& node);

    void unwrap(ASTNode *&value);
    void unwrap(Expression *&value);
};