#include <stdlib.h>
#include <stdio.h>

int main() {
    const int n = 1000;
    
    // Alocação única com verificação de erro
    int* fib = malloc(n * sizeof(int));
    if (fib == NULL) {
        return 1; // Falha na alocação
    }
    
    // Gerar sequência Fibonacci otimizada
    fib[0] = 0;
    fib[1] = 1;
    
    // Loop otimizado - evitando dependências desnecessárias
    for (int i = 2; i < n; i++) {
        fib[i] = fib[i-1] + fib[i-2];
    }
    
    // Usar o resultado para evitar otimização do compilador
    volatile int final_result = fib[n-1];
    
    // Liberação imediata da memória
    free(fib);
    fib = NULL; // Boa prática para evitar dangling pointers
    
    return 0;
}
