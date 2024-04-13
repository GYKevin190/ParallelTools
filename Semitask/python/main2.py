# main.py

import pyopencl as cl
import numpy as np

def main():
    
    with open("C:\Egyetem\ParallelTools\Semitask\python\kernel.cl", "r") as f:
        kernel_code = f.read()

   
    input_array = np.random.randint(1024, 1025, size=10).astype(np.int32)
    n = len(input_array)
    max_factors = 32  

    
    factors = np.zeros((n, max_factors), dtype=np.int32)

    
    platforms = cl.get_platforms()
    platform = platforms[0]

    
    devices = platform.get_devices()
    device = devices[0]

    
    context = cl.Context(properties=[(cl.context_properties.PLATFORM, platform)])
    queue = cl.CommandQueue(context, properties=cl.command_queue_properties.PROFILING_ENABLE)

    
    input_buffer = cl.Buffer(context, cl.mem_flags.READ_ONLY | cl.mem_flags.COPY_HOST_PTR, hostbuf=input_array)
    factors_buffer = cl.Buffer(context, cl.mem_flags.WRITE_ONLY, factors.nbytes)

    
    program = cl.Program(context, kernel_code).build()

    
    event = program.prime_factors(queue, input_array.shape, None, input_buffer, factors_buffer, np.uint32(n))

    
    queue.finish()


    elapsed_time_ns = event.profile.end - event.profile.start

    
    print("Elapsed time (ms):", elapsed_time_ns / 1e6)

    
    cl.enqueue_copy(queue, factors, factors_buffer).wait()

    
    print("Prime factors:")
    for i in range(n):
        print("Input", input_array[i], ":", [factors[i][j] for j in range(max_factors) if factors[i][j] != 0])

if __name__ == "__main__":
    main()
