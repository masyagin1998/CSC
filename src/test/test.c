int kek() {
    int a = 1;
    int b = 4;
    int c = 8;
    int d = 8;
    return (a * b + c * d - 228 - 1488 * 322 - (228 - 322 - 1488)) < (8841 - (228 - 822));
}

int iseven(int x)
{
    if (x % 2 == 0) {
        return 1;
    } else {
        return 0;
    }
}

int sgn(int x)
{
    int res = 0;
    if (x < 0) {
        res = -1;
    } else if (x > 0) {
        res = 1;
    }
    return res;
}

int fib(int n)
{
    int a = 1;
    int b = 1;
    int c = 1;
    int i;
    if (n <= 0) {
        return -1;
    }
    if (n <= 2) {
        return 1;
    }
    i = 3;
    while (i <= n) {
        c = a + b;
        a = b;
        b = c;
        i = i + 1;
    }
    return b;
}

int area(int a, int b)
{
    return a * b;
}

int inners(int n)
{
    int i = 0;
    int sum = 0;
    int j = 0;
    while (i <= n) {
        j = 0;
        while (j <= i) {
            if (j % 2 == 0) {
                sum = sum + j;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return sum;
}

int tobool(int x)
{
    return !(!x);
}

int heh(int x)
{
    if (1) {
        x = 1;
    } else {
        x = 2;
    }
    return x;
}
