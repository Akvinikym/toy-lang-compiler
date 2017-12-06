//
// Created by feross on 11/3/17.
//

#ifndef ILANG_PARSER_H
#define ILANG_PARSER_H

#include "../AST/node.h"
#include "../AST/prototype.h"
#include "../AST/routine.h"
#include "../AST/expression.h"

#include "Lexer.h"

#include <memory>
#include <utility>
#include <unordered_map>

using std::pair;

class Parser {
    Lexer* const lexer;
    int scope = -1; //First scope will be at 0 pos in array
    vector<std::unordered_map <string, ASTNode*>> name_table;

    Expression *parse_expression();
    Expression *parse_primary();
    ASTNode *parse_if();
    ASTNode *parse_for();
    ASTNode *parse_while();
    Expression *parse_unary();
    Expression *parse_real();
    Expression *parse_integer();
    Expression *parse_boolean(bool val);
    Expression *parse_paren();
    ASTNode *parse_identifier_statement();
    Expression *parse_identifier_ref();
    ASTNode *parse_var();
    ASTNode *parse_array();
    ASTNode *parse_record();
    ASTNode *parse_arg();
    ASTNode *parse_type();
    ASTNode *parse_types();
    ASTNode *parse_return();
    ASTNode *parse_statements();
    Ref *parse_ref(Var *a);
    ASTNode *parse_assignment(string identifier_name);
    Expression *parse_binary_op_rhs(int priority, Expression *lhs);
    Prototype *parse_prototype();
// parse - on numbers
// Statements
public:
    Parser(Lexer* const lexer) : lexer(lexer) { }
    ~Parser();

    Prototype *parse_extern();
    Routine *parse_routine();
    Routine *parse_top_level_expression();
    Program *parse();

    void addDecl(pair<string, ASTNode*> name_pair);
    ASTNode *findDecl(string name);
    void openScope();
    void closeScope();
};

#endif //ILANG_PARSER_H
