#ifndef TYPE_CHECKING_VSTR
#define TYPE_CHECKING_VSTR

#include <list>
#include "../visitor.h"
#include "error.h"

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
#include "../AST/ref.h"
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

class TypeCheckingVisitor : public Visitor
{
private:
    // Used to evaluate, if routines return values properly
    struct ConditionGraphNode
    {
        std::vector<ConditionGraphNode*> nexts;
        ConditionGraphNode* previous;
        bool hasReturnStatement;
    };
    ConditionGraphNode* currentGraphNode;
    bool allNextsHaveReturns(ConditionGraphNode& node)
    {
        bool hasReturn = true;
        for (ConditionGraphNode* curNode : node.nexts)
            hasReturn = hasReturn && curNode->hasReturnStatement;
        return hasReturn;
    }

public:
    Routine* currentRoutine;

    void visit(Prototype& node);
    void visit(ArrayDecl& node);
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
    void visit(Real& node);
    void visit(RealType& node);
    void visit(RecordDecl& node);
    void visit(Ref& node);
    void visit(Routine& node);
    void visit(RoutineCall& node);
    void visit(TypeDecl& node);
    void visit(Unary& node);
    void visit(Undefined& node);
    void visit(Var& node);
    void visit(While& node);
    void visit(Return& node);
    void visit(RecordRef& node);
    void visit(ArrayRef& node);
    void visit(Program& node);
    void visit(Statements& node);
    void visit(Void& node);
};

#endif
