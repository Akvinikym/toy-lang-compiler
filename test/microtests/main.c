#include <stdbool.h>
#include <stdio.h>
#include <math.h>

/*
    Integer math
*/
int sum_two_ints();
int sum_two_ints_res = 10;

int sub_two_ints();
int sub_two_ints_res = 5;

int mul_two_ints();
int mul_two_ints_res = 20;

int div_two_ints();
int div_two_ints_res = 6;

/*
    Real math
*/
double sum_two_reals();
double sum_two_reals_res = 1.2;

double sub_two_reals();
double sub_two_reals_res = 11.4;

double mul_two_reals();
double mul_two_reals_res = 8.478; // 3.14 * 2.7

double div_two_reals();
double div_two_reals_res = 1.5625; // 2.5 / 1.6

/*
    Booleans
*/
bool give_true();
bool give_true_res = true;

bool give_false();
bool give_false_res = false;

/*
    Unaries
*/
// int neg_int();
// int neg_int_res = -3;

// double neg_real();
// double neg_real_res = -3.14;

/*
    Conditions and branches
*/
bool true_branch();
bool true_branch_res = true;

bool false_branch();
bool false_branch_res = false;

/*
    Loops
*/
int for_loop();
int for_loop_res = 5;

int inversed_for_loop();
int inversed_for_loop_res = 10;

int while_loop();
int while_loop_res = 9;

/*
    Arrays
*/
int int_array();
int int_array_res = 2;

double float_array();
double float_array_res = 4.5;

bool boolean_array();
bool boolean_array_res = true;

int two_dim_array();
int two_dim_array_res = 6;

/*
    Records
*/
int give_int_field();
int give_int_field_res = 7;


void run_tests()
{
    double compareEpsilon = 0.0001;
    printf("\nTesting system for Visual Obescal is now running!\n\n");

    if (sum_two_ints() == sum_two_ints_res)
        printf("Sum two ints: passed\n");
    else
        printf("Sum two ints: failed\n");

    if (sub_two_ints() == sub_two_ints_res)
        printf("Sub two ints: passed\n");
    else
        printf("Sub two ints: failed\n");

    if (mul_two_ints() == mul_two_ints_res)
        printf("Mul two ints: passed\n");
    else
        printf("Mul two ints: failed\n");

    if (div_two_ints() == div_two_ints_res)
        printf("Div two ints: passed\n");
    else
        printf("Div two ints: failed\n");


    if (fabs(sum_two_reals() - sum_two_reals_res) < compareEpsilon)
        printf("Sum two reals: passed\n");
    else
        printf("Sum two reals: failed\n");

    if (fabs(sub_two_reals() - sub_two_reals_res) < compareEpsilon)
        printf("Sub two reals: passed\n");
    else
        printf("Sub two reals: failed\n");

    if (fabs(div_two_reals() - div_two_reals_res) < compareEpsilon)
        printf("Div two reals: passed\n");
    else
        printf("Div two reals: failed\n");

    if (fabs(mul_two_reals() - mul_two_reals_res) < compareEpsilon)
        printf("Mul two reals: passed\n");
    else
        printf("Mul two reals: failed\n");

    
    if (give_true() == give_true_res)
        printf("Give true: passed\n");
    else
        printf("Give true: failed\n");

    if (give_false() == give_false_res)
        printf("Give false: passed\n");
    else
        printf("Give false: failed\n");


    // if (neg_int() == neg_int_res)
    //     printf("Int negation: passed\n");
    // else
    //     printf("Int negation: failed\n");

    // if (neg_real() == neg_real_res)
    //     printf("Real negation: passed\n");
    // else
    //     printf("Real negation: failed\n");

    
    if (true_branch() == true_branch_res)
        printf("True branch: passed\n");
    else
        printf("True branch: failed\n");

    if (false_branch() == false_branch_res)
        printf("False branch: passed\n");
    else
        printf("False branch: failed\n");

    
    if (for_loop() == for_loop_res)
        printf("For loop: passed\n");
    else
        printf("For loop: failed\n");

    if (inversed_for_loop() == inversed_for_loop_res)
        printf("Inversed for loop: passed\n");
    else
        printf("Inversed for loop: failed\n");

    if (while_loop() == while_loop_res)
        printf("While loop: passed\n");
    else
        printf("While loop: failed\n");


    if (int_array() == int_array_res)
        printf("Integer array: passed\n");
    else
        printf("Integer array: failed\n");
    
    if (fabs(float_array() - float_array_res) < compareEpsilon)
        printf("Real array: passed\n");
    else
        printf("Real array: failed\n");

    if (boolean_array() == boolean_array_res)
        printf("Boolean array: passed\n");
    else
        printf("Boolean array: failed\n");

    if (two_dim_array() == two_dim_array_res)
        printf("Two-dimensional array: passed\n");
    else
        printf("Two-dimensional array: failed");


    if (give_int_field() == give_int_field_res)
        printf("Field from record: passed\n");
    else
        printf("Field from record: failed\n~");


    printf("\nTests are completed!\n\n");
}

int main()
{
    run_tests();

    return 0;
}