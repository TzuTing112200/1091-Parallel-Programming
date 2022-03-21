#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>

__global__ void mandelKernel(float *outputImage, int filterWidth, float *filter, int imageHeight, int imageWidth, float *inputImage) {
    // To avoid error caused by the floating number, use the following pseudo code
    //
    // float x = lowerX + thisX * stepX;
    // float y = lowerY + thisY * stepY;
    int x, xe, y, l, yl, index, temp_x, temp_f;
    float4 sum, sub_sum;

    // printf("-%f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", inputImage[0], inputImage[1], inputImage[2], inputImage[3], inputImage[4], inputImage[5], inputImage[6], inputImage[7], inputImage[8], inputImage[9]);
    index = (blockIdx.x * blockDim.x + threadIdx.x) * 4;
    // printf("-%d-%d-%d-%f-\n", blockIdx.x, threadIdx.x, index, sum);
    // outputImage[0] = 99;
    // outputImage[1] = 99;
    // outputImage[2] = 99;
    // outputImage[3] = 99;
    // outputImage[4] = 99;
    // outputImage[5] = 99;
    // outputImage[6] = 99;
    // outputImage[7] = 99;
    // outputImage[8] = 99;
    // outputImage[9] = 99;
    // printf("--%f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", outputImage[0], outputImage[1], outputImage[2], outputImage[3], outputImage[4], outputImage[5], outputImage[6], outputImage[7], outputImage[8], outputImage[9]);

    // printf("\n\n-%d-%d-%d-\n\n", blockIdx.x, blockDim.x, threadIdx.x);
    x = index / imageWidth - filterWidth;
    xe = index / imageWidth + filterWidth;
    y = index % imageWidth;
    
    sum = make_float4(0.0f, 0.0f, 0.0f, 0.0f);
    temp_f = 0;
    temp_x = x * imageWidth;

    for (; x <= xe; x++, temp_x += imageWidth)
    {
        for (l = -filterWidth; l <= filterWidth; l++, temp_f++)
        {
            if (filter[temp_f] == 0) continue;
            if (x < 4 || x + 4 >= imageHeight) continue;
            
            yl = y + l;
            if (yl < 4 || yl + 4 >= imageWidth) continue;
            
            yl += temp_x;
            sub_sum = make_float4(inputImage[yl], inputImage[yl + 1], inputImage[yl + 2], inputImage[yl + 3]);
            sub_sum *= filter[temp_f];
            
            sum += sub_sum;
        }
    }
    // printf("-%d-%d-%d-%f-\n", blockIdx.x, threadIdx.x, index, sum);
    outputImage[index++] = sum.x;
    outputImage[index++] = sum.y;
    outputImage[index++] = sum.z;
    outputImage[index] = sum.w;
}

// Host front-end function that allocates the memory and launches the GPU kernel 
void hostFE (int filterWidth, float *filter, int imageHeight, int imageWidth, float *inputImage, float *outputImage)
{
    int size = imageHeight * imageWidth * sizeof(float);
    int n_thread = 256;
    
    float *d_img, *d_filter, *d_inputImage;
    cudaMalloc((void **)&d_img, size);
    
    cudaMalloc((void**)&d_filter, sizeof(float) * filterWidth * filterWidth);
    cudaMemcpy(d_filter, filter, sizeof(float) * filterWidth * filterWidth, cudaMemcpyHostToDevice);
    
    cudaMalloc((void**)&d_inputImage, size);
    cudaMemcpy(d_inputImage, inputImage, size, cudaMemcpyHostToDevice);

    // printf("\n\n-%d-%d-\n\n", (imageHeight * imageWidth + 15) / n_thread, n_thread);
    // dim3 blockSize(n_thread, n_thread);
    // dim3 numBlock(resX / n_thread, resY / n_thread);
    filterWidth /= 2;
    // printf("+%f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", filter[0], filter[1], filter[2], filter[3], filter[4], filter[5], filter[6], filter[7], filter[8], filter[9]);
    // printf("+%f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", inputImage[0], inputImage[1], inputImage[2], inputImage[3], inputImage[4], inputImage[5], inputImage[6], inputImage[7], inputImage[8], inputImage[9]);
    // printf("+%f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", outputImage[0], outputImage[1], outputImage[2], outputImage[3], outputImage[4], outputImage[5], outputImage[6], outputImage[7], outputImage[8], outputImage[9]);
    mandelKernel<<<(imageHeight * imageWidth + 255) / n_thread / 4, n_thread>>>(d_img, filterWidth, d_filter, imageHeight, imageWidth, d_inputImage);

    cudaDeviceSynchronize();
    
    cudaMemcpy(outputImage, d_img, size, cudaMemcpyDeviceToHost);
    // printf("++%f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", inputImage[0], inputImage[1], inputImage[2], inputImage[3], inputImage[4], inputImage[5], inputImage[6], inputImage[7], inputImage[8], inputImage[9]);
    // printf("++%f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", outputImage[0], outputImage[1], outputImage[2], outputImage[3], outputImage[4], outputImage[5], outputImage[6], outputImage[7], outputImage[8], outputImage[9]);
    
    cudaFree(d_img);
}
