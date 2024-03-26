    __kernel void sample_kernel(__global float* host_buffer, __global float* host_buffer2 )
    {
        
    int i = get_global_id(0);




    



    int x = i;
    
    host_buffer[i] = i;
    
    
    host_buffer2[0] = x;
    host_buffer2[1] = x;
    host_buffer2[2] = x;
    host_buffer2[3] = x;
    host_buffer2[4] = x;
    host_buffer2[5] = x;
    host_buffer2[6] = x;
    host_buffer2[7] = x;
    host_buffer2[8] = x;
    host_buffer2[9] = x;

    }