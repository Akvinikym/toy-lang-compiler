#include <cstdlib>
#include <string>
#include <unordered_map>
#include <iostream>

#include "Lexer.h"

using std::string;
using std::unordered_map;

Lexer::Lexer(std::istream* inputStream) : inputStream(inputStream) {
    op_priority_[(int)Token::OR] = 2;
    op_priority_[(int)Token::XOR] = 2;
    op_priority_[(int)Token::AND] = 2;
    op_priority_[(int)Token::EQUAL] = 10;    
    op_priority_[(int)Token::LESS] = 20;
    op_priority_[(int)Token::HIGH] = 20;
    op_priority_[(int)Token::NOTEQ] = 20;
    op_priority_[(int)Token::LESSEQ] = 20;
    op_priority_[(int)Token::HIGHEQ] = 20;
    op_priority_[(int)Token::PLUS] = 30;
    op_priority_[(int)Token::MINUS] = 30;
    op_priority_[(int)Token::MUL] = 240;
    op_priority_[(int)Token::DIV] = 240;
    op_priority_[(int)Token::MOD] = 240;

    opchar_map_[(int)Token::EQUAL] = opchars::EQUAL;
    opchar_map_[(int)Token::LESS] = opchars::LESS;
    opchar_map_[(int)Token::HIGH] = opchars::HIGH;
    opchar_map_[(int)Token::PLUS] = opchars::PLUS;
    opchar_map_[(int)Token::MINUS] = opchars::MINUS;
    opchar_map_[(int)Token::MUL] = opchars::MUL;
    opchar_map_[(int)Token::DIV] = opchars::DIV;
    opchar_map_[(int)Token::NOTEQ] = opchars::NOTEQ;
    opchar_map_[(int)Token::MOD] = opchars::MOD;
    opchar_map_[(int)Token::HIGHEQ] = opchars::HIGHEQ;
    opchar_map_[(int)Token::LESSEQ] = opchars::LESSEQ;
    opchar_map_[(int)Token::OR] = opchars::OR;
    opchar_map_[(int)Token::XOR] = opchars::XOR;
    opchar_map_[(int)Token::AND] = opchars::AND;


    keyword_map_["var"] = Token::VAR;
    keyword_map_["end"] = Token::END;
    keyword_map_["if"] = Token::IF;
    keyword_map_["then"] = Token::THEN;
    keyword_map_["else"] = Token::ELSE;
    keyword_map_["for"] = Token::FOR;
    keyword_map_["in"] = Token::IN;
    keyword_map_["reverse"] = Token::REVERSE;
    keyword_map_["routine"] = Token::ROUTINE;
    keyword_map_["is"] = Token::IS;
    keyword_map_["type"] = Token::TYPE;
    keyword_map_["record"] = Token::RECORD;
    keyword_map_["array"] = Token::ARRAY;
    keyword_map_["while"] = Token::WHILE;
    keyword_map_["loop"] = Token::LOOP;
    keyword_map_["return"] = Token::RETURN;
    keyword_map_["boolean"] = Token::BOOLEAN_TYPE;
    keyword_map_["integer"] = Token::INTEGER_TYPE;
    keyword_map_["real"] = Token::REAL_TYPE;
    keyword_map_["not"] = Token::NOT;
    keyword_map_["true"] = Token::TRUE_;
    keyword_map_["false"] = Token::FALSE_;
    keyword_map_["and"] = Token::AND;
    keyword_map_["or"] = Token::OR;
    keyword_map_["xor"] = Token::XOR;
    keyword_map_["="] = Token::EQUAL;
    keyword_map_["<"] = Token::LESS;
    keyword_map_[">"] = Token::HIGH;
    keyword_map_["+"] = Token::PLUS;
    keyword_map_["-"] = Token::MINUS;
    keyword_map_["*"] = Token::MUL;
    keyword_map_["/"] = Token::DIV;
    keyword_map_["%"] = Token::MOD;
    keyword_map_["/="] = Token::NOTEQ;
    keyword_map_["<="] = Token::LESSEQ;
    keyword_map_[">="] = Token::HIGHEQ;

}

void Lexer::next() {
    static int last_char = ' ';

    while (isspace(last_char)) {
        last_char = inputStream->get();
    }

    if (isalpha(last_char)) {
        identifier_ = last_char;
        while (isalnum((last_char = inputStream->get())) || last_char == '_') {
            identifier_ += last_char;
        }
        if (keyword_map_.count(identifier_) == 0 ) {
            current_token_ = (int)Token::IDENTIFIER;
            return;
        } else {
            current_token_ = (int)keyword_map_[identifier_];
            return;
        }
    }

    if (isdigit(last_char) ||
        (last_char == '-' && isdigit(inputStream->peek()))) {
        string number_string;
        bool is_double = false;
        do {
            if (last_char == '.' && inputStream->peek() == '.') {
                inputStream->putback(last_char);
                break;
            }
            if (last_char == '.') is_double = true;
            number_string += last_char;
            last_char = inputStream->get();
        } while ( isdigit(last_char) || last_char == '.' );

        if (is_double) {
            real_value_ = strtod(number_string.c_str(), 0);
            current_token_ = (int)Token::REAL;
        }else{
            integer_value_ = (int) strtod(number_string.c_str(), 0);
            current_token_ = (int)Token::INTEGER;
        }

        return;
    }
    if ( last_char == '.' && inputStream->peek() == '.'){
        last_char = inputStream->get();
        current_token_ = (int)Token::RANGE;
        last_char = inputStream->get();
        
        return;
    }

    if ( last_char == '<') {
        if ( inputStream->peek() == '='){
            last_char = inputStream->get();
            current_token_ = (int)Token::LESSEQ;
            last_char = inputStream->get();
            return;
        }
        current_token_ = (int)Token::LESS;
        last_char = inputStream->get();
        return;
    }

    if ( last_char == '>') {
        if ( inputStream->peek() == '='){
            last_char = inputStream->get();
            current_token_ = (int)Token::HIGHEQ;
            last_char = inputStream->get();
            return;
        }

        current_token_ = (int)Token::HIGH;
        last_char = inputStream->get();
        return;
    }

    if ( last_char == '/') {
        if ( inputStream->peek() == '='){
            last_char = inputStream->get();
            current_token_ = (int)Token::NOTEQ;
            last_char = inputStream->get();
            return;
        }
        current_token_ = (int)Token::DIV;
        last_char = inputStream->get();
        return;
    }

    if (last_char == '*') {
        current_token_ = (int)Token::MUL;
        last_char = inputStream->get();
        return;
    }
    if (last_char == '%') {
        current_token_ = (int)Token::MOD;
        last_char = inputStream->get();
        return;
    }
    if (last_char == '+') {
        current_token_ = (int)Token::PLUS;
        last_char = inputStream->get();
        return;
    }
    if (last_char == '-') {
        current_token_ = (int)Token::MINUS;
        last_char = inputStream->get();
        return;
    }
    if (last_char == '=') {
        current_token_ = (int)Token::EQUAL;
        last_char = inputStream->get();
        return;
    }
    if ( last_char == '#' ) {
        do {
            last_char = inputStream->get();
        } while ( last_char != EOF && last_char != '\n' && last_char != '\r' );

        if ( last_char != EOF ) {
            next();
            return;
        }
    }

    if ( last_char == EOF ) {
        current_token_ = (int)Token::EOF_;
        return;
    }

    current_token_ = last_char;
    last_char = inputStream->get();
}


int Lexer::current_token() const {
    return current_token_;
}

opchars Lexer::current_opchar() {
    return opchar_map_[current_token_];
}

string Lexer::identifier() const { return identifier_; }

int Lexer::integer_value() const { return integer_value_; }

double Lexer::real_value() const { return real_value_; }

int Lexer::token_priority() {
    if ( op_priority_.find(current_token_) == op_priority_.end() ){
        return -1; // not in table
    }
    int priority = op_priority_[current_token_];

    if ( priority <= 0 ) {
        return -1;
    }

    return priority;
}


void Lexer::add_op_priority(const char op, const int priority) {
    op_priority_[(int)op] = priority;
}

const unordered_map<int, int> Lexer::op_priority() const {
    return op_priority_;
}
