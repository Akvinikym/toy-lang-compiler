#include "typechecking_visitor.h"
#include <iostream>

/*
    In routine's prototype we can do nothing
*/
void TypeCheckingVisitor::visit(Prototype& node) 
{
    std::cout << "Type checking Prototype\n";
    return;
}

/*
    Check that array's size is an integer constant
*/
void TypeCheckingVisitor::visit(ArrayDecl& node)
{
    std::cout << "Type checking ArrayDecl\n";

    auto sizeExpression = (Expression*) node.expression;
    if (sizeExpression->type->type != types::Integer)
    {
        reportError("Size of array is not compile-time integer constanst\n");
    }
}

/*
    Check that both sides of the assignment are of the same or
    compatible types. If second variant, cast it
*/
void TypeCheckingVisitor::visit(Assignment& node)
{
    std::cout << "Type checking Assignment\n";

    auto left_type = node.ref->type->type;
    auto right = (Expression*) node.value;
    auto right_type = right->type->type;

    node.ref->accept(*this);
    right->accept(*this);

    if (left_type == types::Integer && right_type == types::Integer)
    {
        return;
    }
    else if (left_type == types::Integer && right_type == types::Real)
    {
        auto cast_node = new Cast { right, new IntegerType() };
        node.value = cast_node;
    }
    else if (left_type == types::Integer && right_type == types::Boolean)
    {
        auto cast_node = new Cast { right, new IntegerType() };
        node.value = cast_node;
    }
    else if (left_type == types::Real && right_type == types::Integer)
    {
        auto cast_node = new Cast { right, new RealType() };
        node.value = cast_node;
    }
    else if (left_type == types::Real && right_type == types::Real)
    {
        return;
    }
    else if (left_type == types::Real && right_type == types::Boolean)
    {
        auto cast_node = new Cast { right, new RealType() };
        node.value = cast_node;
    }
    else if (left_type == types::Boolean && right_type == types::Integer)
    {
        auto cast_node = new Cast { right, new BooleanType() };
        node.value = cast_node;
    }
    else if (left_type == types::Boolean && right_type == types::Boolean)
    {
        return;
    }
    else if (left_type == types::Array && right_type == types::Array)
    {
        if (*node.ref->type != *right->type)
        {
            reportError("Trying to assign arrays of different types\n");
        }
        return;
    }
    else if (left_type == types::Record && right_type == types::Record)
    {
        if (*node.ref->type != *right->type)
        {
            reportError("Trying to assign records of different types\n");
        }
        return;
    }
    else
    {
        reportError("Trying to assign incompatible types, " + std::to_string((int) node.lhs_type) + " and " +
            std::to_string((int) right_type) + "\n");
    }
}

/*
    Everything was checked during the previous pass
*/
void TypeCheckingVisitor::visit(Binary& node) 
{
    std::cout << "Type checking Binary\n";

    return;
}

/*
    Can do nothing
*/
void TypeCheckingVisitor::visit(Boolean& node)
{
    std::cout << "Type checking Boolean\n";

    return;
}

/*
    Can do nothing
*/
void TypeCheckingVisitor::visit(BooleanType& node)
{
    std::cout << "Type checking BooleanType\n";

    return;
}

/*
    Can do nothing
*/
void TypeCheckingVisitor::visit(Cast& node)
{
    std::cout << "Type checking Cast\n";

    return;
}

/*
    Check that expression in FOR is of integer type
    Go through statements in FOR
*/
void TypeCheckingVisitor::visit(For& node) 
{
    std::cout << "Type checking For\n";

    auto from = (Expression*) node.start;
    auto until = (Expression*) node.end;
    if (from->type->type != types::Integer || until->type->type != types::Integer)
        reportError("Variable in FOR loop is not of integer type!\n");

    node.body->accept(*this);
}

/*
    Check that expression in IF is of boolean type
    Go through all statements in both branches
*/
void TypeCheckingVisitor::visit(If& node) 
{
    std::cout << "Type checking If\n";

    auto expr = (Expression*) node.condition;
    if (expr->type->type != types::Boolean)
        reportError("Type of expression in if is not boolean!\n");

    auto ifElseGraphNode = new ConditionGraphNode;
    ifElseGraphNode->hasReturnStatement = false;
    ifElseGraphNode->previous = currentGraphNode;
    currentGraphNode->nexts.push_back(ifElseGraphNode);
    currentGraphNode = ifElseGraphNode;
    
    auto ifGraphNode = new ConditionGraphNode;
    ifGraphNode->hasReturnStatement = false;
    ifGraphNode->previous = ifElseGraphNode;
    ifElseGraphNode->nexts.push_back(ifGraphNode);
    currentGraphNode = ifGraphNode;

    node.then->accept(*this);
    if (node.else_body)
    {
        auto elseGraphNode = new ConditionGraphNode;
        elseGraphNode->hasReturnStatement = false;
        elseGraphNode->previous = ifElseGraphNode;
        ifElseGraphNode->nexts.push_back(elseGraphNode);
        currentGraphNode = elseGraphNode;

        node.else_body->accept(*this);

        ifElseGraphNode->hasReturnStatement = allNextsHaveReturns(*ifElseGraphNode);
    }

    currentGraphNode = ifElseGraphNode->previous;
}

/*
    Can do nothing
*/
void TypeCheckingVisitor::visit(Integer& node) 
{
    std::cout << "Type checking Integer\n";

    return;
}

/*
    Can do nothing
*/
void TypeCheckingVisitor::visit(IntegerType& node)
{
    std::cout << "Type checking IntegerType\n";

    return;
}

/*
    Visit var in the ref
*/
void TypeCheckingVisitor::visit(NamedRef& node)
{
    std::cout << "Type checking NamedRef\n";

    node.var->accept(*this);
}

void TypeCheckingVisitor::visit(Real& node)
{
    std::cout << "Type checking Real\n";

    return;
}
/*
    Can do nothing
*/
void TypeCheckingVisitor::visit(RealType& node)
{
    std::cout << "Type checking RealType\n";

    return;
}

/*
    Go through all declarations in the record
*/
void TypeCheckingVisitor::visit(RecordDecl& node)
{
    std::cout << "Type checking RecordDecl\n";

    for (int i = 0; i < node.refs.size(); i++)
        node.refs[i]->accept(*this);
}

/*
    Visit all chained references
*/
void TypeCheckingVisitor::visit(Ref& node)
{
    std::cout << "Type checking Ref\n";

    if (node.next)
        node.next->accept(*this);
    if (node.prev)
        node.prev->accept(*this);
}

/*
    Go through all routine's statements
*/
void TypeCheckingVisitor::visit(Routine& node)
{
    std::cout << "Type checking Routine\n";

    // Preparations for valid return checkment
    currentRoutine = &node;
    currentGraphNode = new ConditionGraphNode;
    currentGraphNode->hasReturnStatement = false;

    // Go through all statements in body
    node.body->accept(*this);

    std::cout << "Here\n" << currentGraphNode << "\n";

    // Check, if function returns smth in all cases
    if (!allNextsHaveReturns(*currentGraphNode) && node.proto->type->type != types::Void)
        reportError("Routine " + currentRoutine->proto->name + " does not return " +
            "a value in all branches!\n");
}

/*
    Check that arguments are exactly as in the routine declaration
*/
void TypeCheckingVisitor::visit(RoutineCall& node)
{
    std::cout << "Type checking RoutineCall\n";

    auto actualArguments = node.args;
    auto expectedArguments = node.callee->proto->args;
    auto routineName = node.callee->proto->name;
    
    // Check arguments
    auto argsLength = expectedArguments.size();
    if (actualArguments.size() != argsLength)
        reportError("Arguments of function " + routineName + " are incorrect! Expected " +
            std::to_string(argsLength) + ", got " + std::to_string(actualArguments.size()) + "\n");

    for (int i = 0; i < argsLength; i++)
    {
        if (*actualArguments[i]->type != *(((Var*)expectedArguments[i])->var_decl.second))
            reportError("Provided arguments for function " + routineName +
                " are incorrect! Expected another type on " + std::to_string(i) + "\n");
    }
}

/*
    Can do nothing
*/
void TypeCheckingVisitor::visit(TypeDecl& node)
{
    std::cout << "Type checking TypeDecl\n";

    return;
}

/*
    Can do nothing
*/
void TypeCheckingVisitor::visit(Unary& node)
{
    std::cout << "Type checking Unary\n";

    return;
}

/*
    We cannot reach the undefined in normal case
*/
void TypeCheckingVisitor::visit(Undefined& node)
{
    std::cout << "Type checking Undefined\n";

    reportError("Undefined reached during the type checking!\n");
}

/*
    Check that initial value is of declared type, if exists
*/
void TypeCheckingVisitor::visit(Var& node)
{
    std::cout << "Type checking Var\n";

    auto declaredType = node.var_decl.second;
    declaredType->accept(*this);
    if (node.body) 
    {
        node.body->accept(*this);
        auto actualType = node.body->type;
        if (*declaredType != *actualType) {
            reportError("Initial value of variable " + node.var_decl.first + " is not of the declared type!\n");
        }
    }
}

/*
    Check expression in WHILE for boolean
    Go through statements in loop
*/
void TypeCheckingVisitor::visit(While& node)
{
    std::cout << "Type checking While\n";

    auto expr = (Expression*) node.expression;
    if (expr->type->type != types::Boolean)
        reportError("Expression in WHILE is not of boolean type!\n");
    node.body->accept(*this);
}

/*
    Check that routine's return value is the same as declared
*/
void TypeCheckingVisitor::visit(Return& node)
{
    std::cout << "Type checking Return\n";

    if (*((Expression*)node.expression)->type != *currentRoutine->proto->type)
        reportError("Routine " + currentRoutine->proto->name + " has incorrect " +
        "return value!\n");
    currentGraphNode->hasReturnStatement = true;
}

/*
    Visit the previous reference
*/
void TypeCheckingVisitor::visit(RecordRef& node)
{
    std::cout << "Type checking RecordRef\n";

    if (node.prev)
        node.prev->accept(*this);
}

/*
    Check that called position is integer
    Visit the previous reference
*/
void TypeCheckingVisitor::visit(ArrayRef& node)
{
    std::cout << "Type checking ArrayRef\n";

    auto position = (Expression*) node.pos;
    if (position->type->type != types::Integer)
        reportError("Trying to call array with not-integer type!\n");
    if (node.prev)
        node.prev->accept(*this);
}

/*
    Entry point
*/
void TypeCheckingVisitor::visit(Program& node)
{
    std::cout << "Type checking Program\n";

    for (auto statement: node.program)
    {
        statement->accept(*this);
    }
}

/*
    Visit every statement
*/
void TypeCheckingVisitor::visit(Statements& node)
{
    std::cout << "Type checking Statements\n";

    for (auto statement : node.statements)
        statement->accept(*this);
}

/*
    Can do nothing
*/
void TypeCheckingVisitor::visit(Void& node)
{
    std::cout << "Type checking Void\n";

    return;
}

