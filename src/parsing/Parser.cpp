#include "../AST/ast.h"
#include "Lexer.h"
#include "Parser.h"
#include "Errors.h"
#include <cstdio>
#include <memory>
#include <iostream>

using std::move;
using std::swap;
using std::string;
using std::vector;
using std::make_pair;
using std::unique_ptr;

Parser::~Parser() {}

Expression * Parser::parse_real() {
    Expression *result = new Real(lexer->real_value());
    lexer->next();
    return result;
}

Expression * Parser::parse_integer() {
    Expression *result = new Integer(lexer->integer_value());
    lexer->next();
    return result;
}

Expression * Parser::parse_boolean(bool value) {
    Expression *result = new Boolean(value);
    lexer->next();
    return result;
}

ASTNode * Parser::parse_return() {
    lexer->next();
    ASTNode *ret = parse_expression();
    return new Return(ret);
}

Expression * Parser::parse_paren() {
    lexer->next();

    Expression *value = parse_expression();
    if ( !value ) {
        return nullptr;
    }
    if ( lexer->current_token() != ')' ) {
        return ErrorE("expected ')'");
    }

    lexer->next();
    return value;
}

ASTNode * Parser::parse_types() {
    ASTNode *type;

    if(lexer->current_token() == (int)Token::ARRAY){
        type = parse_array();
        return type;
    } else if (lexer->current_token() == (int)Token::RECORD) {
        type = parse_record();
        return type;
    }

    if (lexer->current_token() == (int)Token::INTEGER_TYPE) {
        lexer->next();
        return new IntegerType();
    }

    if (lexer->current_token() == (int)Token::REAL_TYPE) {
        lexer->next();
        return new RealType();
    }

    if (lexer->current_token() == (int)Token::BOOLEAN_TYPE) {
        lexer->next();
        return new BooleanType();
    }

    // Check if current_token is in table
    if(lexer->current_token() == (int)Token::IDENTIFIER) {
        ASTNode *type = findDecl(lexer->identifier());
        std::cout << "Find record: " << lexer->identifier() << "\n";
        lexer->next();
        return type;
    }

    return nullptr;
}

Ref * Parser::parse_ref(Var *assignee){
    NamedRef *named = new NamedRef(assignee);
    Ref *latest_ref = named;
    while(1){
        if(lexer->current_token() == '.'){
            lexer->next();
            RecordRef *record_ref = new RecordRef( lexer->identifier(), latest_ref );
            latest_ref->add_next(record_ref);
            latest_ref = record_ref;
            lexer->next();
        } else if(lexer->current_token() == '['){
            lexer->next();
            auto pos = parse_expression();
            ArrayRef *array_ref = new ArrayRef(pos, latest_ref);
            latest_ref->add_next(array_ref);
            latest_ref = array_ref;
            if(lexer->current_token() != ']'){
                Error("Expected ']' in array ref");
            }
            lexer->next();
        }else{
            break;
        }
    }
    return latest_ref;
}

/// Returns Variable or RoutineCall
ASTNode * Parser::parse_identifier_statement() {
    string identifier_name = lexer->identifier();
    lexer->next();
    ASTNode *assignee = findDecl(identifier_name);
    if(!assignee){
        return Error("Undefined Identifier");
    }

    if ( lexer->current_token() != '('){
        Ref *ref = parse_ref((Var*)assignee);
        if ( lexer->current_token() == ':' ){
            lexer->next();
            if (lexer->current_token() == (int)Token::EQUAL) {
                lexer->next();
                Expression *value = parse_expression();
                return new Assignment(ref, value, types::Undefined);
            }
            return Error("Unknown ':' at this possition");
        }
        return Error("No assginement found");
    }

    lexer->next();

    vector<Expression*> args;

    while ( lexer->current_token() != ')' ) {
        auto arg = parse_expression();
        if ( arg ) {
            args.push_back(arg);
        }

        if ( lexer->current_token() == ')' ) {
            break;
        }

        if ( lexer->current_token() != ',' ) {
            return Error("expected ')' or ',' in argument list");
        }

        lexer->next();
    }

    lexer->next();
    ASTNode *callee = findDecl(identifier_name);
    return new RoutineCall((Routine*)callee, args);
}

Expression * Parser::parse_identifier_ref(){
    string identifier_name = lexer->identifier();
    std::cout << "Identifier: " << identifier_name << "\n";
    ASTNode *ref = findDecl(identifier_name);
    lexer->next();
    if (lexer->current_token() != '(') {
        return parse_ref((Var*)ref);
    }

    lexer->next();

    vector<Expression*> args;

    while ( lexer->current_token() != ')' ) {
        auto arg = parse_expression();
        if ( arg ) {
            args.push_back(arg);
        }

        if ( lexer->current_token() == ')' ) {
            break;
        }

        if ( lexer->current_token() != ',' ) {
            return ErrorE("expected ')' or ',' in argument list");
        }

        lexer->next();
    }

    lexer->next();
    ASTNode *callee = findDecl(identifier_name);
    if (!callee) {
        std::cout << "couldn't find " << identifier_name << "\n";
    }
    return new RoutineCall((Routine*)callee, args);
}

ASTNode * Parser::parse_var() {
    lexer->next();

    if ( lexer->current_token() != (int)Token::IDENTIFIER ) {
        return Error("Expected identifier after var");
    }
    string name = lexer->identifier();

    lexer->next();
    Type *type = nullptr;

    if ( lexer->current_token() == ':') {
        lexer->next();
        type = (Type*) parse_types();
    }

    if ( lexer->current_token() == (int)Token::IS) {
        lexer->next();
        auto expression = parse_expression();
        auto var = new Var(make_pair(name, type ? type : new Undefined()), expression);
        addDecl(make_pair(name, var));
        return var;
    }

    if (!type){
        return Error("Expected 'is' or ':' keyword after 'var'", name.c_str());
    }

    ASTNode *var = new Var(make_pair(name, type), nullptr);
    addDecl(make_pair(name, var));
    return var;
}

ASTNode * Parser::parse_type() {
    lexer->next();

    string id_name;

    if ( lexer->current_token() != (int)Token::IDENTIFIER ) {
        return Error("Expected identifier after 'type'");
    }
    id_name = lexer->identifier();
    lexer->next();

    if ( lexer->current_token() != (int)Token::IS ) {
        return Error("Expected 'is' keyword after 'type'");
    }
    lexer->next();

    ASTNode *type = parse_types();

    if (!type) {
        return Error("Expected valid type after 'is'");
    }

    ASTNode *type_decl  = new TypeDecl(id_name, (Type*) type);
    addDecl(make_pair(id_name, type_decl));
    return type_decl;
}

ASTNode * Parser::parse_record() {
    lexer->next();
    vector<ASTNode*> vars_decl;
    openScope();
    while (lexer->current_token() == (int)Token::VAR) {
        vars_decl.push_back(parse_var());
    }

    if (lexer->current_token() != (int)Token::END){
        return Error("Expected 'end' at the end of the 'record' declaration");
    }

    lexer->next();
    closeScope();
    return new RecordDecl(vars_decl);
}

ASTNode * Parser::parse_array() {
    lexer->next();

    if (lexer->current_token() != '['){
        return Error("Expected '[' at the beginning of array expression");
    }
    lexer->next();

    Expression *expression = parse_expression();
    if (!expression) { return nullptr; }

    if (lexer->current_token() != ']'){
        return Error("Expected ']' at the end of the array expression");
    }
    lexer->next();

    ASTNode *type = parse_types();

    if (!type) {
        return Error("Expected valid type after array declaration");
    }

    return new ArrayDecl(expression, (Type*)type);
}

Expression * Parser::parse_primary() {
    switch( lexer->current_token() ) {
        case (int)Token::IDENTIFIER:
            return parse_identifier_ref();
        case (int)Token::INTEGER:
            return parse_integer();
        case (int)Token::FALSE_:
            return parse_boolean(false);
        case (int)Token::TRUE_:
            return parse_boolean(true);
        case (int)Token::REAL:
            return parse_real();
        case '(':
            return parse_paren();
        default:
            std::cout << "Unknown token " << (int)lexer->current_token() << "\n";
            return 0;
    }
}

ASTNode * Parser::parse_statements() {
    std::vector<ASTNode *> statements;
    while (1){
        switch( lexer->current_token() ) {
            case (int)Token::IDENTIFIER:
                statements.push_back(parse_identifier_statement());
                break;
            case (int)Token::IF:
                statements.push_back(parse_if());
                break;
            case (int)Token::FOR:
                statements.push_back(parse_for());
                break;
            case (int)Token::WHILE:
                statements.push_back(parse_while());
                break;
            case (int)Token::VAR:
                statements.push_back(parse_var());
                break;
            case (int)Token::TYPE:
                statements.push_back(parse_type());
                break;
            case (int)Token::RETURN:
                statements.push_back(parse_return());
                break;
            case (int)Token::END:
                return new Statements(statements);
                break;
            case (int)Token::ELSE:
                return new Statements(statements);
                break;
            default:
                std::cout << "Unknown token " << (int)lexer->current_token() << "\n";
                return 0;
        }
    }
}

Expression * Parser::parse_binary_op_rhs(int expression_priority, Expression *LHS) {
    while (1) {
        if (lexer->current_token() == (int)Token::RANGE) {
            return LHS;
        }
        int token_priority = lexer->token_priority();
        if ( token_priority < expression_priority ) {
            return LHS;
        }

        opchars binary_op = lexer->current_opchar();
        lexer->next();

        Expression *RHS = parse_unary();
        if ( !RHS ) { return nullptr; }
        std::cout << "Opchar parsed: " << (int)binary_op << "\n";

        int next_priority = lexer->token_priority();
        if ( token_priority < next_priority ) {
            RHS = parse_binary_op_rhs(token_priority + 1, RHS);
        }

        LHS = new Binary(binary_op, LHS, RHS);
    }
}

Expression * Parser::parse_unary() {
    if ( !isascii(lexer->current_token())
         || lexer->current_token() == '('
         || lexer->current_token() == ')') {
        return parse_primary();
    }
    int unary_op = lexer->current_token();
    lexer->next();

    if ( auto operand = parse_unary() ) {
        return new Unary(unary_op, operand);
    }

    return 0;
}

Expression * Parser::parse_expression() {
    Expression *LHS = parse_unary();
    if ( !LHS ) { return nullptr; }
    std::cout << "Parsing RHS\n";
    return parse_binary_op_rhs(0, LHS);
}

ASTNode * Parser::parse_arg() {

    if ( lexer->current_token() != (int)Token::IDENTIFIER ) {
        return Error("Expected identifier as argument name");
    }

    string arg_name = lexer->identifier();
    lexer->next();

    ASTNode *type = parse_types();

    if (!type){
        return Error("Expected valid type for argument");
    }
    ASTNode *argument = new Var(make_pair(arg_name, (Type*) type), nullptr);
    addDecl(make_pair(arg_name, argument));
    return argument;
}

Prototype * Parser::parse_prototype() {
    string func_name;

    if ( lexer->current_token() == (int)Token::IDENTIFIER ) {
        func_name = lexer->identifier();
        lexer->next();
    }
    if ( lexer->current_token() != '(' ) {
        return ErrorP("Expected '(' in prototype \n");
    }
    openScope();

    vector<Var*> arg_names;

    lexer->next();

    while ( lexer->current_token() == (int)Token::IDENTIFIER) {
        arg_names.push_back((Var*)parse_arg());

        if ( lexer->current_token() != ',') { break; }

        lexer->next();
    }

    if ( lexer->current_token() != ')' ) {
        return ErrorP("Expected ')' in prototype \n");
    }

    lexer->next();

    ASTNode *type;
    if ( lexer->current_token() == ':') {
        lexer->next();
        type = parse_types();
    } else {
        type = new Void();
        ErrorR("Expected : before type declaration in routine");
    }

    Prototype * prot = new Prototype(func_name, arg_names, (Type*)type);

    return prot;
}

Routine * Parser::parse_routine() {
    lexer->next();

    Prototype *proto = parse_prototype();
    if ( !proto ) { return nullptr; }

    if (lexer->current_token() != (int) Token::IS ){
        ErrorR("Expected is before routine body declaration");
    }
    lexer->next();

    ASTNode *expression = parse_statements();

    lexer->next();


    auto r = new Routine(proto, (Statements *) expression);

    closeScope();
    addDecl(make_pair(r->proto->name, r));

    return r;
}

ASTNode * Parser::parse_if() {
    lexer->next();

    auto condition = parse_expression();
    if ( ! condition ) { return nullptr; }
    if ( lexer->current_token() != (int)Token::THEN ) {
        return Error("expected 'then'");
    }

    lexer->next();

    ASTNode *then = parse_statements();
    if ( ! then ) { return nullptr; }

    if ( lexer->current_token() != (int)Token::ELSE ) {
        if (lexer->current_token() != (int)Token::END){
            return Error("expected 'end' at the end of if");
        }
        lexer->next();
        return new If(condition, (Statements*) then);
    }

    lexer->next();

    ASTNode *else_body = parse_statements();
    if ( ! else_body ) { return nullptr; }

    if (lexer->current_token() != (int)Token::END){
        return Error("expected 'end' at the end of if");
    }
    lexer->next();

    return new If(condition, (Statements *) then, (Statements *) else_body);
}

ASTNode * Parser::parse_for() {
    lexer->next();
    if ( lexer->current_token() != (int)Token::IDENTIFIER ) {
        return Error("expected identifier after 'for'");
    }
    openScope();
    ASTNode *id_name = new Var(make_pair(lexer->identifier(), new IntegerType()), nullptr);
    addDecl(make_pair(lexer->identifier(), id_name));
    lexer->next();

    if ( lexer->current_token() != (int)Token::IN) {
        return Error("expected 'in' after 'for'");
    }
    lexer->next();

    bool reverse = (lexer->current_token() == (int)Token::REVERSE);

    if (reverse){
        lexer->next();
    }

    ASTNode *start = parse_expression();
    if ( !start ) {
        std::cout << "Start is null\n";
        return nullptr;
    }
    if ( lexer->current_token() != (int)Token::RANGE){
        return Error("Expected '..' after for start value");
    }

    lexer->next();
    ASTNode *end = parse_expression();
    if ( end == 0 ) { return 0; }

    if (lexer->current_token() != (int)Token::LOOP) {
        return Error("Expected 'loop' after Range in 'for'");
    }

    lexer->next();

    ASTNode *body = parse_statements();
    if ( body == 0 ) { return nullptr; }

    if (lexer->current_token() != (int)Token::END){
        return Error("Expected 'end' at the end of the 'loop' body expression");
    }

    lexer->next();
    closeScope();

    return new For(id_name, start, end, (Statements*) body, reverse);
}

ASTNode * Parser::parse_while() {
    lexer->next();

    ASTNode *condition = parse_expression();
    if ( !condition ) { return nullptr; }

    if (lexer->current_token() != (int)Token::LOOP) {
        return Error("Expected 'loop' after expression in 'while'");
    }

    lexer->next();

    ASTNode *body = parse_statements();
    if ( !body ) { return nullptr; }

    if (lexer->current_token() != (int)Token::END){
        return Error("Expected 'end' at the end of the 'loop' body expression");
    }
    lexer->next();

    return new While(condition, (Statements*) body);
}

Program * Parser::parse() {
    vector<ASTNode*> program_decl;
    lexer->next();
    openScope();
    while (lexer->current_token() != (int) Token::EOF_) {
        switch (lexer->current_token()) {
            case (int)Token::VAR:
                program_decl.push_back(parse_var());
                break;
            case (int)Token::ROUTINE:
                program_decl.push_back(parse_routine());
                break;
            case (int)Token::TYPE:
                program_decl.push_back(parse_type());
                break;
            case (int)Token::FOR:
                program_decl.push_back(parse_for());
                break;
            case (int)Token::WHILE:
                program_decl.push_back(parse_while());
                break;
            case (int)Token::IF:
                program_decl.push_back(parse_if());
                break;
            default:
                std::cout << "Can't handle top level " << (int) lexer->current_token() << "\n";
                return nullptr;
        }
    }
    closeScope();
    return new Program(program_decl);
}

void Parser::addDecl(pair<string, ASTNode*> decl) {
    name_table[scope][decl.first] = decl.second;
}

ASTNode *Parser::findDecl(string name) {
    int local_scope = scope;
    ASTNode *value;

    while (local_scope >= 0){

        if(name_table[local_scope].find(name) != name_table[local_scope].end()) {
            return name_table[local_scope][name];
        }
        local_scope--;
    }

    return Error("Can't find declaration ", name.c_str());
}


void Parser::openScope(){
    scope++;
    if (name_table.size() <= scope){
        name_table.push_back(std::unordered_map <string, ASTNode*>());
    }
}

void Parser::closeScope(){
    name_table[scope].clear();
    scope--;
}
