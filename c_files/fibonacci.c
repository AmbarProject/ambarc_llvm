#include <stdio.h>

long fibonacci(int n) {
    if (n <= 1) return n;
    
    long a = 0, b = 1, temp;
    
    for (int i = 2; i <= n; i++) {
        temp = a + b;
        a = b;
        b = temp;
    }
    
    return b;
}

int main() {
    int n = 40;
    volatile long result = fibonacci(n);
    return 0;
}
