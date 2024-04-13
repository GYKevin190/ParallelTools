__kernel void prime_factors(__global const int *input, __global int *factors, const unsigned int n)
{
    int i = get_global_id(0);
    if (i < n) {
        int num = input[i];
        int index = 0;
        for (int factor = 2; factor <= num; ++factor) {
            while (num % factor == 0) {
                factors[i * 32 + index] = factor;
                num /= factor;
                ++index;
            }
        }
    }
}