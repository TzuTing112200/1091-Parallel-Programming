#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include "device_launch_parameters.h"

__global__ void test(float* devPtr, size_t pitch, int width, int height)
{
    printf("%d\t%d\t%d\t%d\n", blockIdx.x, threadIdx.x, blockIdx.y, threadIdx.y);
    float* row = (float*)((char*)devPtr + blockIdx.x * pitch); 

    row[0] = blockIdx.x + 1000000; 
}

int main() {
    int n_thread = 16;
    int width=150;
    int height=20;
    float *myMatrix = new float[width * height];

    //cudaHostAlloc((void**)&myMatrix, width * height * sizeof(float),cudaHostAllocDefault);

    memset(myMatrix, 0, width * height * sizeof(float));

    for (int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            myMatrix[i + j * width]=j * 1000 + i;
            printf("%.0f\t",myMatrix[i + j * width]);
        }
        printf("\n");
    }
    printf("\n");

    float *dev_mat;
    size_t pitch;

    printf("%d\n",pitch);
    cudaMallocPitch(&dev_mat,&pitch,width*sizeof(float),height);
    printf("%d\n%d\n\n", pitch, pitch / sizeof(float));

    cudaMemcpy(dev_mat,myMatrix,width*height*sizeof(float),cudaMemcpyHostToDevice);
    
    dim3 blockSize(n_thread, n_thread);
    dim3 numBlock(width / n_thread, height / n_thread);

    test<<<numBlock, blockSize>>>(dev_mat,pitch,width,height);

    cudaDeviceSynchronize();

    cudaMemcpy(myMatrix,dev_mat,width*height*sizeof(float),cudaMemcpyDeviceToHost);

    cudaFree(dev_mat);

    for (int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            printf("%.0f\t",myMatrix[i + j * width]);
        }
        printf("\n");
    }

    return 0;
}
