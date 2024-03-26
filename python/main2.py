# main.py

import pyopencl as cl
import numpy as np

def main():
    # Load kernel code from file
    with open("kernel.cl", "r") as f:
        kernel_code = f.read()

    # Generate random input array
    input_array = np.random.randint(1, 1001, size=10).astype(np.int32)
    n = len(input_array)
    max_factors = 32  # Maximum number of prime factors we expect

    # Initialize arrays for factors
    factors = np.zeros((n, max_factors), dtype=np.int32)

    # Get platforms and select a platform
    platforms = cl.get_platforms()
    platform = platforms[0]

    # Get devices and select a device
    devices = platform.get_devices()
    device = devices[0]

    # Create context and command queue
    context = cl.Context(properties=[(cl.context_properties.PLATFORM, platform)])
    queue = cl.CommandQueue(context, properties=cl.command_queue_properties.PROFILING_ENABLE)

    # Create memory buffers
    input_buffer = cl.Buffer(context, cl.mem_flags.READ_ONLY | cl.mem_flags.COPY_HOST_PTR, hostbuf=input_array)
    factors_buffer = cl.Buffer(context, cl.mem_flags.WRITE_ONLY, factors.nbytes)

    # Build kernel
    program = cl.Program(context, kernel_code).build()

    # Execute kernel and measure time
    event = program.prime_factors(queue, input_array.shape, None, input_buffer, factors_buffer, np.uint32(n))

    # Ensure that all commands have finished
    queue.finish()

    # Get profiling information
    elapsed_time_ns = event.profile.end - event.profile.start

    # Print elapsed time
    print("Elapsed time (ms):", elapsed_time_ns / 1e6)

    # Copy results back to host
    cl.enqueue_copy(queue, factors, factors_buffer).wait()

    # Check results (optional)
    print("Prime factors:")
    for i in range(n):
        print("Input", input_array[i], ":", [factors[i][j] for j in range(max_factors) if factors[i][j] != 0])

if __name__ == "__main__":
    main()
