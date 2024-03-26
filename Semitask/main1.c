#include "kernel_loader.h"
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

const int SAMPLE_SIZE = 10;

int main(void)
{
    cl_int err;
    int error_code;


    cl_uint n_platforms;
    cl_platform_id platform_id;
    err = clGetPlatformIDs(1, &platform_id, &n_platforms);
    if (err != CL_SUCCESS) {
        printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n", err);
        return 0;
    }


    cl_device_id device_id;
    cl_uint n_devices;
    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &n_devices);
    if (err != CL_SUCCESS) {
        printf("[ERROR] Error calling clGetDeviceIDs. Error code: %d\n", err);
        return 0;
    }

    /*
    char device_name[128];
    err = clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(device_name), &device_name, NULL);
    if (err != CL_SUCCESS) {
       printf("[ERROR] Error getting device name. Error code: %d\n", err);
       return 0;
    }

    printf("GPU device name: %s\n", device_name);

    */

    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    if (err != CL_SUCCESS) {
        printf("[ERROR] Error creating context. Error code: %d\n", err);
        return 0;
    }

    const char* kernel_code = load_kernel_source("kernels/vec.cl", &error_code);
    if (error_code != 0) {
        printf("Source code loading error!\n");
        return 0;
    }
    cl_program program = clCreateProgramWithSource(context, 1, &kernel_code, NULL, &err);
    if (err != CL_SUCCESS) {
        printf("[ERROR] Error creating program. Error code: %d\n", err);
        return 0;
    }
    err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Build error! Code: %d\n", err);
        size_t real_size;
        err = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &real_size);
        char* build_log = (char*)malloc(sizeof(char) * (real_size + 1));
        err = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, real_size + 1, build_log, &real_size);
        printf("Real size : %zu\n", real_size);
        printf("Build log : %s\n", build_log);
        free(build_log);
        return 0;
    }
    cl_kernel kernel = clCreateKernel(program, "sample_kernel", &err);
    if (err != CL_SUCCESS) {
        printf("[ERROR] Error creating kernel. Error code: %d\n", err);
        return 0;
    }


    float* host_buffer_result = (float*)malloc(sizeof(float) * SAMPLE_SIZE);

    float* host_buffer_result2 = (float*)malloc(sizeof(float) * SAMPLE_SIZE);

    


    cl_mem device_buffer_result = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * SAMPLE_SIZE, NULL, &err);

    cl_mem device_buffer_result2 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * SAMPLE_SIZE, NULL, &err);


    if (err != CL_SUCCESS) {
        printf("[ERROR] Error creating buffer. Error code: %d\n", err);
        return 0;
    }

    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&device_buffer_result);
    if (err != CL_SUCCESS) {
        printf("[ERROR] Error setting kernel argument. Error code: %d\n", err);
        return 0;
    }

    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&device_buffer_result2);
    if (err != CL_SUCCESS) {
        printf("[ERROR] Error setting kernel argument. Error code: %d\n", err);
        return 0;
    }

   

    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &err);
    if (err != CL_SUCCESS) {
        printf("[ERROR] Error creating command queue. Error code: %d\n", err);
        return 0;
    }

    size_t local_work_size = 256;
    size_t n_work_groups = (SAMPLE_SIZE + local_work_size - 1) / local_work_size;
    size_t global_work_size = n_work_groups * local_work_size;

    cl_event event;
    err = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_work_size, &local_work_size, 0, NULL, &event);
    if (err != CL_SUCCESS) {
        printf("[ERROR] Error enqueueing kernel. Error code: %d\n", err);
        return 0;
    }
    clFinish(command_queue);

    err = clEnqueueReadBuffer(command_queue, device_buffer_result, CL_TRUE, 0, sizeof(float) * SAMPLE_SIZE, host_buffer_result, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("[ERROR] Error reading buffer. Error code: %d\n", err);
        return 0;
    }

     err = clEnqueueReadBuffer(command_queue, device_buffer_result2, CL_TRUE, 0, sizeof(float) * SAMPLE_SIZE, host_buffer_result2, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("[ERROR] Error reading buffer. Error code: %d\n", err);
        return 0;
    }

    for (int i = 0; i < SAMPLE_SIZE; i++) {
        printf("Result (host1) [%d]: %lf\n", i, host_buffer_result[i]);
    }

    for (int i = 0; i < SAMPLE_SIZE; i++) {
        printf("Result (host2) [%d]: %lf\n", i, host_buffer_result2[i]);
    }

  

    

    cl_ulong start_time, end_time;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start_time, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end_time, NULL);
    double execution_time = (end_time - start_time) * 1.0e-6;

    printf("Kernel execution time: %.6f milliseconds\n", execution_time);

    // Release the resources
    clReleaseEvent(event);
    clReleaseCommandQueue(command_queue);
    clReleaseMemObject(device_buffer_result);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    free(host_buffer_result);

    return 0;
}
