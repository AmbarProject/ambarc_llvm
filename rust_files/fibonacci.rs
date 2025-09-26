fn fibonacci(n: u64) -> u64 {
    if n <= 1 {
        return n;
    }
    
    let mut a = 0;
    let mut b = 1;
    
    for _ in 2..=n {
        let temp = a + b;
        a = b;
        b = temp;
    }
    
    b
}

fn main() {
    let n = 40;
    let result = fibonacci(n);
    std::hint::black_box(result); // Prevenir otimização
}
