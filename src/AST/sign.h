#ifndef ILANG_SIGN_H
#define ILANG_SIGN_H

class Sign : public ASTNode {
    char symbol;

public:
    Sign(char symbol);
};

#endif //ILANG_SIGN_H
