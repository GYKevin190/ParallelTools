import numpy as np
import time

def prime_factors_sequential(input_array):
    n = len(input_array)
    max_factors = 32  # Maximum number of prime factors we expect

    factors = np.zeros((n, max_factors), dtype=np.int32)

    for i, num in enumerate(input_array):
        index = 0
        for factor in range(2, num + 1):
            while num % factor == 0:
                factors[i, index] = factor
                num //= factor
                index += 1
    return factors

def main():
    input_array = np.random.randint(1, 1001, size=10).astype(np.int32)
    
    sum =0
    start_time = time.time()
    factors = prime_factors_sequential(input_array)
    end_time = time.time()

    elapsed_time = end_time - start_time
    print("Elapsed time (ms):", elapsed_time * 1000)

if __name__ == "__main__":
    main()
