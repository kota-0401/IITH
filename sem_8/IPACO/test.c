// WRITE YOUR TEST CASES IN THIS FILE.
// OUTPUT OF EACH FUNCTION MUST BE WRITTEN AT THE END OF THE CORRESPONDING FUNCTION IN CODE COMMENTS.
// OUTPUT MUST BE WRITTEN IN THE PRESCRIBED FORMAT ONLY.

#include <stdio.h>

void private_test1()
{
    // This test case demonstrates that aliasing relationships are not necessarily transitive
    int a = 1, b = 2, c = 3;
    int *r = &c, *q = &b, *p = &a; // r points to c, q points to b, p points to a

    if (a == b)
    {
        // This branch is not executed, but still be considered
        p = q; // p now points to b (same as q)
    }
    else
    {
        q = r; // q now points to c (same as r)
    }

    /*
        Possible aliasing relationships:
        - p points to either a or b
        - q points to either b or c
        - r points to c

        1. Dereferencing to same value
        - p and q may be aliases
        - q and r may be aliases
        - However, p and r are never aliases, showing aliasing is not transitive
    */
}

// Expected aliasing output from LLVM IR:

/*
    Function : private_test1
    r -> {q}
    q -> {r, p}
    p -> {q}
*/

void private_test2(int *x, int *z)
{
    int b = 5;
    int *y = &b;                      // y points to b
    int **k = &x, **l = &y, **m = &z; // k points to x, l points to y, m points to z
    int ***n = &l;                    // n points to l

    if (*x == *y && *y == *z)
    {
        n = &k; // n points to k
    }
    else
    {
        n = &m; // n points to m
    }

    *n = l; // n points to k or m, they should point to what l points to i.e. y (This should not be done, we union y)

    /*
        Possible aliasing relationships:
        - y points to b
        - k points to either x or y (which points to b)
        - l points to y (which points to b)
        - m points to either z or y (which points to b)
        - n points to either k or m

        1. Dereferencing to same value
        - k and x may be aliases
        - k and y may be aliases
        - l and y may be aliases
        - m and y may be aliases
        - m and z may be aliases
        - n and k may be aliases
        - n and x may be aliases
        - n and y may be aliases
        - n and m may be aliases
        - n and z may be aliases

        2. Function Arguments are aliases
        - x and z may be aliases
        - k and z may be aliases
        - m and x may be aliases
    */
}

// Expected aliasing output from LLVM IR:

/*
    Function : private_test2
    x -> {z, k, m, n}
    z -> {x, k, m, n}
    y -> {k, l, m, n}
    k -> {x, z, y, n}
    l -> {y}
    m -> {x, z, y, n}
    n -> {x, z, y, k, m}
*/

void private_test3()
{
    int a = 2, b = 4, c = 6, d = 8, f = 12;
    int *x = &a, *y = &b, *z = &c; // x points to a, y points to b, z points to c
    int *arr[3] = {x, y, z};
    int **p1, **q1, **r1, *p = &d, *r = &f; // p points to d, r points to f

    p1 = &p; // p1 points to p (which points to d)
    q1 = &y; // q1 points to y (which points to b)
    r1 = &r; // r1 points to r (which points to f)

    int flag = 1;

    if (flag < 1)
    {
        *p1 = x; // p1 points to p (which points to what x points to i.e. a)
    }
    else if (flag > 1)
    {
        r1 = &z; // r1 points to z (which points to c)
    }

    /*
        Possible aliasing relationships:
        - x points to a, y points to b, z points to c
        - p points to either a or d
        - r points to f
        - p1 points to p (which points to either a or d)
        - q1 points to y (which points to b)
        - r1 points to either r (which points to f) or z (which points to c)

        1. Dereferencing to same value
        - p and x may be aliases
        - p1 and p may be aliases
        - p1 and x may be aliases
        - q1 and y may be aliases
        - r1 and r may be aliases
        - r1 and z may be aliases

        2. Dereferencing to different indices of arr
        - q1 and r1 may be aliases.
    */
}

// Expected aliasing output from LLVM IR:

/*
    Function : private_test3
    x -> {p1, p}
    y -> {q1}
    z -> {r1}
    p1 -> {x, p}
    q1 -> {y, r1}
    r1 -> {z, q1, r}
    p -> {x, p1}
    r -> {r1}
*/

int main()
{
    int a = 5, c = 5;
    private_test1();
    private_test2(&a, &c);
    private_test3();
    return 0;
}