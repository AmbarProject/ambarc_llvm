#include <stdio.h>
#include <math.h>

#define ITERATIONS 100000000

int main() {
    double pi = 0.0;
    double sign = 1.0;
    
    for (long i = 0; i < ITERATIONS; i++) {
        pi += sign / (2.0 * i + 1.0);
        sign = -sign;
    }
    
    pi *= 4.0;
    volatile double result = pi; // Prevenir otimização
    
    return 0;
}
