#include <stdio.h>

int main() {
    int total = 0;
    int n = 50000000;
    
    for(int i = 0; i < n; i++) {
        float a = i * 3.14159f;
        float b = a / 2.71828f;
        float c = a + b - i;
        
        int temp = (int)c;
        int modulo = temp - (temp / 1000) * 1000;
        total = total + modulo;
    }
    return total;
}
