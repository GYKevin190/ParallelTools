__kernel void vector_addition(__global const int* a, __global const int* b, __global int* result, int n)
{
    int i = get_global_id(0);
    if (i < n) {
        result[i] = a[i] + b[i];
    }
}