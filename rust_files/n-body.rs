use std::hint::black_box;

fn main() {
    let mut total: i32 = 0;
    let n: i32 = 50000000;
    
    for i in 0..n {
        let a: f32 = i as f32 * 3.14159;
        let b: f32 = a / 2.71828;
        let c: f32 = a + b - i as f32;
        
        let temp: i32 = c as i32;
        let modulo: i32 = temp - (temp / 1000) * 1000;
        total += modulo;
    }
    
    black_box(total); 
}
