    __kernel void sample_kernel(__global float* host_buffer)
    {
        
        int i = get_global_id(0);

    // Megkeressük az i-hez legközelebbi prímszámot
    int closestPrime = i;
    bool prime;
    do {
        prime = true;
        if (closestPrime <= 1)
            prime = false;
        else if (closestPrime <= 3)
            prime = true;
        else if (closestPrime % 2 == 0 || closestPrime % 3 == 0)
            prime = false;
        else {
            for (int j = 5; j * j <= closestPrime; j += 6) {
                if (closestPrime % j == 0 || closestPrime % (j + 2) == 0) {
                    prime = false;
                    break;
                }
            }
        }
        closestPrime--;
    } while (!prime && closestPrime >= 2); // A ciklus addig fut, amíg nem talál prímszámot vagy eléri a 2-t

    closestPrime++; // Az előző ciklusban talált prímszámot adja vissza

    // Az eredményt beírjuk a host_buffer tömbbe
    host_buffer[i] = (float)closestPrime;
    }
