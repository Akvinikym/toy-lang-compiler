#include <string>
#include <iostream>

extern "C" int sum_two_ints();
extern "C" int sub_two_ints();
extern "C" int mul_two_ints();
extern "C" int div_two_ints();
extern "C" double sum_two_reals();
extern "C" double sub_two_reals();
extern "C" double mul_two_reals();
extern "C" double div_two_reals();
extern "C" bool give_true();
extern "C" bool give_false();
extern "C" bool true_branch();
extern "C" bool false_branch();
extern "C" int for_loop();
extern "C" int inversed_for_loop();
extern "C" int while_loop();
extern "C" int int_array();
extern "C" double float_array();
extern "C" bool boolean_array();
extern "C" int two_dim_array();
extern "C" int give_int_field();

int main() {
    std::string name;
    std::cin >> name;

    if (name == "sum_two_ints") {
        std::cout << sum_two_ints();
        return 0;
    }
    if (name == "sub_two_ints") {
        std::cout << sub_two_ints();
        return 0;
    }
    if (name == "mul_two_ints") {
        std::cout << mul_two_ints();
        return 0;
    }
    if (name == "div_two_ints") {
        std::cout << div_two_ints();
        return 0;
    }
    if (name == "sum_two_reals") {
        std::cout << sum_two_reals();
        return 0;
    }
    if (name == "sub_two_reals") {
        std::cout << sub_two_reals();
        return 0;
    }
    if (name == "mul_two_reals") {
        std::cout << mul_two_reals();
        return 0;
    }
    if (name == "div_two_reals") {
        std::cout << div_two_reals();
        return 0;
    }
    if (name == "give_true") {
        std::cout << give_true();
        return 0;
    }
    if (name == "give_false") {
        std::cout << give_false();
        return 0;
    }
    if (name == "true_branch") {
        std::cout << true_branch();
        return 0;
    }
    if (name == "false_branch") {
        std::cout << false_branch();
        return 0;
    }
    if (name == "for_loop") {
        std::cout << for_loop();
        return 0;
    }
    if (name == "inversed_for_loop") {
        std::cout << inversed_for_loop();
        return 0;
    }
    if (name == "while_loop") {
        std::cout << while_loop();
        return 0;
    }
    if (name == "int_array") {
        std::cout << int_array();
        return 0;
    }
    if (name == "float_array") {
        std::cout << float_array();
        return 0;
    }
    if (name == "boolean_array") {
        std::cout << boolean_array();
        return 0;
    }
    if (name == "two_dim_array") {
        std::cout << two_dim_array();
        return 0;
    }
    if (name == "give_int_field") {
        std::cout << give_int_field();
        return 0;
    }

    if (    ) {
        std::cout << "function input or output is complex";
        return -1;
    }

    std::cout << "function does not exist";
    return -2;
}
