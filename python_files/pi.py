import math

ITERATIONS = 100000000

def main():
    pi = 0.0
    sign = 1.0
    
    for i in range(ITERATIONS):
        pi += sign / (2.0 * i + 1.0)
        sign = -sign
    
    pi *= 4.0
    
    return pi

if __name__ == "__main__":
    result = main()
