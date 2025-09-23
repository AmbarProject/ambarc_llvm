fn main() {
    const ITERATIONS: i32 = 100000000;
    
    let mut pi = 0.0;
    let mut sign = 1.0;
    
    for i in 0..=ITERATIONS {
        pi += sign / (2.0 * i as f64 + 1.0);
        sign = -sign;
    }
   
    pi *= 4.0;
}
