//
// Created by sebastiend on 07/09/2018.
//

#ifndef IOER_HOMEWORK1_MY_UNIT_TEST_H
#define IOER_HOMEWORK1_MY_UNIT_TEST_H

#include <iostream>
#include <string.h>

extern int number_of_tests;
extern int number_of_pass;
extern bool is_identical_array(int *array1, int *array2, int size1, int size2);

#define TEST_FUNCTION(X,Y) {number_of_tests++; std::cout << "line:" << __LINE__; if (X==Y) \
                                {std::cout << " PASS" << std::endl;number_of_pass++;} \
                            else\
                                {std::cout << \
                                " FAIL EXPECTED: " << \
                                Y << " RECEIVED: " << X << std::endl;} \
                            }
#define TEST_NOT_EQUAL(X,Y) {number_of_tests++; std::cout << "line:" << __LINE__; if (X!=Y) \
                                {std::cout << " PASS" << std::endl;number_of_pass++;} \
                            else\
                                {std::cout << \
                                " FAIL EXPECTED: " << \
                                Y << " RECEIVED: " << X << std::endl;} \
                            }

#define TEST_STRING(X,Y) {number_of_tests++; std::cout << "line:" << __LINE__; if (strcmp(X,Y)==0) \
                                {std::cout << " PASS" << std::endl;number_of_pass++;} \
                            else\
                                {std::cout << \
                                " FAIL EXPECTED: " << \
                                Y << " RECEIVED: " << X << std::endl;} \
                            }


#define PRINT_RESULTS() {std::cout << "You succeeded to pass " << number_of_pass << "/" << number_of_tests;}

bool check_a(double *a);
bool check_b(double *a,int size);
bool check_c(double *a,int size);
bool check_c_result_3(double *a);


#endif //IOER_HOMEWORK1_MY_UNIT_TEST_H

