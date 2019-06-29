#include <stdio.h>

int iseven(int x);
int sgn(int x);
int fib(int n);
int area(int a, int b);
int inners(int n);
int tobool(int x);

int main()
{
    printf("iseven(%d)=%d; iseven(%d)=%d; iseven(%d)=%d\n", -1, iseven(-1), 2, iseven(2), -2, iseven(-2));
    printf("sgn(%d)=%d; sgn(%d)=%d; sgn(%d)=%d\n", -1488, sgn(-1488), 0, sgn(0), 228, sgn(228));
    printf("fib(%d)=%d; fib(%d)=%d; fib(%d)=%d\n", -1, fib(-1), 2, fib(2), 6, fib(6));
    printf("area(%d, %d)=%d\n", 2, 28, area(2, 28));
    printf("inners(%d)=%d\n", 5, inners(5));
    printf("tobool(%d)=%d; tobool(%d)=%d; tobool(%d)=%d\n", 0, tobool(0), 1, tobool(1), 5, tobool(5));
    
    return 0;
}
