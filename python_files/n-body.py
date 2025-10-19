def main():
    total = 0
    n = 50000000
    
    for i in range(n):
        a = i * 3.14159
        b = a / 2.71828
        c = a + b - i
        
        temp = int(c)
        modulo = temp - (temp // 1000) * 1000
        total += modulo
    return total

if __name__ == "__main__":
    main()
