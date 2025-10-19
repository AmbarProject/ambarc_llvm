function main() {
    let total = 0;
    const n = 50000000;
    
    for(let i = 0; i < n; i++) {
        const a = i * 3.14159;
        const b = a / 2.71828;
        const c = a + b - i;
        
        const temp = Math.floor(c);
        const modulo = temp - Math.floor(temp / 1000) * 1000;
        total += modulo;
    }
    
    return total;
}

main();
