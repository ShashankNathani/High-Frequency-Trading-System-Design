//
// Created by sebastiend on 07/09/2018.
//

#include "my_unit_test.h"

int number_of_tests = 0;
int number_of_pass = 0;

bool is_identical_array(int *array1, int *array2, int size1, int size2)
{
    // If lengths of array are not equal means
    // array are not equal
    if (size1 != size2)
        return false;
    // Linearly compare elements
    for (int i=0; i<size1; i++)
        if (array1[i] != array2[i])
            return false;
    // If all elements were same.
    return true;
}


bool check_a(double *a)
{
    return (a[0]==1 &&
            a[1]==2 &&
            a[2]==3 &&
            a[3]==1 &&
            a[4]==1 &&
            a[5]==1 &&
            a[6]==1 &&
            a[7]==1 &&
            a[8]==1);
}

bool check_b(double *a,int size)
{
    for (int k=0; k<size; k++)
    {
        if (a[k] != 1) return false;
    }
    return true;
}

bool check_c(double *a,int size)
{
    for (int k=0; k<size; k++)
    {
        if (a[k] != 0) return false;
    }
    return true;
}

bool check_c_result_3(double *a)
{
    return a[0]==4 && a[1]==5 && a[2]==6;
}