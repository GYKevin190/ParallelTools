#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

// Ellenőrzi, hogy egy szám prímszám-e
bool is_prime(int number) {
    if (number <= 1) {
        return false;
    }
    if (number <= 3) {
        return true;
    }
    if (number % 2 == 0 || number % 3 == 0) {
        return false;
    }
    for (int i = 5; i * i <= number; i += 6) {
        if (number % i == 0 || number % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

// Megkeresi az adott számhoz legközelebbi prímszámot
int find_nearest_prime(int number) {
    if (is_prime(number)) {
        return number;
    }

    int smaller_prime = number - 1;
    int larger_prime = number + 1;
    
    while (true) {
        if (is_prime(smaller_prime)) {
            return smaller_prime;
        } else if (is_prime(larger_prime)) {
            return larger_prime;
        }
        smaller_prime--;
        larger_prime++;
    }
}

int main() {
    const int SIZE = 100000;
    int primes[SIZE];
    
    clock_t start_time = clock(); // Mérjük az időt előtte
    
    for (int i = 1; i <= SIZE; ++i) {
        primes[i - 1] = find_nearest_prime(i);
    }
    
    clock_t end_time = clock(); // Mérjük az időt utána
    
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC; // Számítsuk ki a teljes időt másodpercekben
    
    // Kiírjuk az első 10 prímszámot a tömbből
    //printf("Az elso 10 primszam a tombben:\n");
    for (int i = 0; i < 10; ++i) {
        //printf("%d ", primes[i]);
    }
    printf("\n");
    
    printf("Elapsed time for the loop: %.6f miliseceond\n", elapsed_time*1000);
    
    return 0;
}
