__kernel void sample_kernel(__global int* host_buffer, __global int* input_array, int n)
{
    int i = get_global_id(0);
    if (i < n) {
        host_buffer[i] = input_array[i] * 2;
    }
}
