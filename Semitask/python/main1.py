import numpy as np
import time
import matplotlib.pyplot as plt

def prime_factors_sequential(input_array):
    n = len(input_array)
    max_factors = 32  

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
    step = 100000
    max_size = 250000

    sizes = range(50000, max_size + 1, step)
    times = []

    for size in sizes:
        input_array = np.random.randint(1, 1025, size=size).astype(np.int32)
        print("Calculating for size:", size)
        
        start_time = time.time()
        prime_factors_sequential(input_array)
        end_time = time.time()

        elapsed_time = end_time - start_time
        times.append(elapsed_time)
        print("Elapsed time (ms):", elapsed_time * 1000)
    
    plt.plot(sizes, times)
    plt.xlabel('Array Size')
    plt.ylabel('Time (s)')
    plt.title('Execution Time vs. Array Size')
    plt.show()

if __name__ == "__main__":
    main()
