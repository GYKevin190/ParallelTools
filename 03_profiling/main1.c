    #include "kernel_loader.h"

    #define CL_TARGET_OPENCL_VERSION 220
    #include <CL/cl.h>

    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/time.h>

    const int SAMPLE_SIZE = 1000;

    int main(void)
    {
        int i;
        cl_int err;
        int error_code;

        // Get platform
        cl_uint n_platforms;
        cl_platform_id platform_id;
        err = clGetPlatformIDs(1, &platform_id, &n_platforms);
        if (err != CL_SUCCESS) {
            printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n", err);
            return 0;
        }

        // Get device
        cl_device_id device_id;
        cl_uint n_devices;
        err = clGetDeviceIDs(
            platform_id,
            CL_DEVICE_TYPE_GPU,
            1,
            &device_id,
            &n_devices
        );
        if (err != CL_SUCCESS) {
            printf("[ERROR] Error calling clGetDeviceIDs. Error code: %d\n", err);
            return 0;
        }

        // Create OpenCL context
        cl_context context = clCreateContext(NULL, n_devices, &device_id, NULL, NULL, NULL);

        // Build the program
        const char* kernel_code = load_kernel_source("kernels/vec.cl", &error_code);
        if (error_code != 0) {
            printf("Source code loading error!\n");
            return 0;
        }
        cl_program program = clCreateProgramWithSource(context , 1, &kernel_code, NULL, NULL);
        const char options[] = "";
        err = clBuildProgram(
            program,
            1,
            &device_id,
            options,
            NULL,
            NULL
        );
        if (err != CL_SUCCESS) {
            printf("Build error! Code: %d\n", err);
            size_t real_size;
            err = clGetProgramBuildInfo(
                program,
                device_id,
                CL_PROGRAM_BUILD_LOG,
                0,
                NULL,
                &real_size
            );
            char* build_log = (char*)malloc(sizeof(char) * (real_size + 1));
            err = clGetProgramBuildInfo(
                program,
                device_id,
                CL_PROGRAM_BUILD_LOG,
                real_size + 1,
                build_log,
                &real_size
            );
            printf("Real size : %d\n", real_size);
            printf("Build log : %s\n", build_log);
            free(build_log);
            return 0;
        }
        cl_kernel kernel = clCreateKernel(program, "vector_addition", NULL);

        // Create the host buffers and initialize them
        int* host_buffer_result = (int*)malloc(sizeof(int));


        // Create the device buffers
        cl_mem device_buffer_result = clCreateBuffer(context, CL_MEM_WRITE_ONLY,sizeof(int), NULL, NULL);

        // Set kernel arguments
        clSetKernelArg(kernel, 0, sizeof(cl_mem), (int*)&device_buffer_result);

        

        // Create the command queue
        cl_command_queue command_queue = clCreateCommandQueue(
            context, device_id, CL_QUEUE_PROFILING_ENABLE, NULL);

        // Apply the kernel on the range
        size_t local_work_size = 256;
        size_t n_work_groups = (SAMPLE_SIZE + local_work_size - 1) / local_work_size;
        size_t global_work_size = n_work_groups * local_work_size;

        cl_event event;
        clEnqueueNDRangeKernel(
            command_queue,
            kernel,
            1,
            NULL,
            &global_work_size,
            &local_work_size,
            0,
            NULL,
            &event
        );
        clFinish(command_queue);


        // Host buffer <- Device buffer
        clEnqueueReadBuffer(
        command_queue,
        device_buffer_result,
        CL_TRUE,
        0,
        sizeof(int),
        host_buffer_result,
        0,
        NULL,
        NULL
        );

        printf(" A result %d", host_buffer_result[0]);


        // Release the resources
        clReleaseKernel(kernel);
        clReleaseProgram(program);
        clReleaseContext(context);
        clReleaseMemObject(device_buffer_result);
        clReleaseDevice(device_id);
        
        free(host_buffer_result);

        return 0;
    }
