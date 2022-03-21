#include <stdio.h>
#include <stdlib.h>
#include "hostFE.h"
#include "helper.h"

void hostFE(int filterWidth, float *filter, int imageHeight, int imageWidth,
            float *inputImage, float *outputImage, cl_device_id *device,
            cl_context *context, cl_program *program)
{
    cl_int status;
    cl_kernel kernel;
    cl_mem d_inputImage, d_outputImage, d_filter;
    cl_command_queue queue;
    
    int imageSize, filterSize;
    size_t globalSize;

    globalSize = imageHeight * imageWidth;
    imageSize = globalSize * sizeof(float);
    filterSize = filterWidth * filterWidth * sizeof(float);
    filterWidth /= 2;

    kernel = clCreateKernel(*program, "convolution", &status);
    queue = clCreateCommandQueue(*context, *device, CL_QUEUE_PROFILING_ENABLE, &status);

    d_inputImage = clCreateBuffer(*context, CL_MEM_USE_HOST_PTR, imageSize, inputImage, &status);
    d_outputImage = clCreateBuffer(*context, CL_MEM_WRITE_ONLY, imageSize, NULL, &status);
    d_filter = clCreateBuffer(*context, CL_MEM_USE_HOST_PTR, filterSize, filter, &status);

    // clEnqueueWriteBuffer(queue, d_inputImage, CL_TRUE, 0, imageSize, inputImage, 0, NULL, NULL);
    // clEnqueueWriteBuffer(queue, d_filter, CL_TRUE, 0, filterSize, filter, 0, NULL, NULL);

    clSetKernelArg(kernel, 0, sizeof(int), &filterWidth);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_filter);
    clSetKernelArg(kernel, 2, sizeof(int), &imageHeight);
    clSetKernelArg(kernel, 3, sizeof(int), &imageWidth); 
    clSetKernelArg(kernel, 4, sizeof(cl_mem), &d_inputImage);
    clSetKernelArg(kernel, 5, sizeof(cl_mem), &d_outputImage);
    
    globalSize /= 4;
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);

    // clFinish(queue);

    clEnqueueReadBuffer(queue, d_outputImage, CL_TRUE, 0, imageSize, outputImage, 0, NULL, NULL);

    // clReleaseKernel(kernel);
    // clReleaseMemObject(d_inputImage);
    // clReleaseMemObject(d_outputImage);
    // clReleaseMemObject(d_filter);
    // clReleaseCommandQueue(queue);
}