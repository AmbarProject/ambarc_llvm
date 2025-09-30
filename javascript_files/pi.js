const ITERATIONS = 100_000_000;
let pi = 0.0;
let sign = 1.0;

for (let i = 0; i < ITERATIONS; i++) {
    pi += sign / (2.0 * i + 1.0);
    sign = -sign;
}

pi *= 4.0;

// Guardar globalmente para prevenir otimização
globalThis.result = pi;

