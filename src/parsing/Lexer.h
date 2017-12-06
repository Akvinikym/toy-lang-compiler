#ifndef ILANG_LEXER_H
#define ILANG_LEXER_H

#include <string>
#include <unordered_map>
#include <sstream>

using std::string;
using std::unordered_map;

enum class Token {
    EOF_ = -1,
    VAR = -2,
    END = -3,
    IF = -4,
    THEN = -5,
    ELSE = -6,
    FOR = -7,
    WHILE = -8,
    LOOP = -9,
    IN = -10,
    REVERSE = -11,
    ROUTINE = -12,
    IS = -13,
    TYPE = -14,
    RECORD = -15,
    RETURN = -16,
    ARRAY = -17,
    IDENTIFIER = -18,
    REAL = -19,
    INTEGER = -20,
    BOOLEAN = -21,
    REAL_TYPE = -22,
    INTEGER_TYPE = -23,
    BOOLEAN_TYPE = -24,
    NOT = -25,
    TRUE_ = -26,
    FALSE_ = -27,
    RANGE = -28,
    EQUAL = -29,
    LESS = -30,
    HIGH = -31,
    LESSEQ = -32,
    HIGHEQ = -33,
    NOTEQ = -34,
    PLUS = -35,
    MINUS = -36,
    MUL = -37,
    DIV = -38,
    MOD = -39,
    AND = -40,
    OR = -41,
    XOR = -42,
};

// TODO: Implement enum also for operation chars
enum class opchars {
    EQUAL,
    LESS,
    HIGH,
    LESSEQ,
    HIGHEQ,
    NOTEQ,
    PLUS,
    MINUS,
    MUL,
    DIV,
    MOD,
    AND,
    OR,
    XOR,
};

class Lexer {
    std::istream* inputStream;

protected:
    int current_token_;
    string identifier_;
    int integer_value_;
    double real_value_;
    unordered_map <int, int> op_priority_;
    unordered_map <string, Token> keyword_map_;
    unordered_map <int, opchars> opchar_map_;

public:

    Lexer(std::istream* inputStream);

    int current_token() const;
    opchars current_opchar();
    string identifier() const;
    int  integer_value() const;
    double  real_value() const;

    void next();

    int token_priority();
    void add_op_priority(const char op, const int priority);
    const unordered_map<int, int> op_priority() const;

};
#endif //ILANG_LEXER_H
