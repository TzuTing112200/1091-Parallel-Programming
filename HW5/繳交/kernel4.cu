#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>
__device__ int cal(float x, float y, int maxIterations){
    float z_re = x, z_im = y;
    int i;
    for (i = 0; i < maxIterations; ++i)
    {
        if (z_re * z_re + z_im * z_im > 4.f)
        break;

      float new_re = z_re * z_re - z_im * z_im;
      float new_im = 2.f * z_re * z_im;
      z_re = x + new_re;
      z_im = y + new_im;
    }
    return i;
}

__global__ void mandelKernel(int *img, float lowerX, float lowerY, float stepX, float stepY, int maxIterations, int resX, int resY) {
    // To avoid error caused by the floating number, use the following pseudo code
    //
    // float x = lowerX + thisX * stepX;
    // float y = lowerY + thisY * stepY;
    
    int thisX = blockIdx.x * blockDim.x + threadIdx.x;
    
    if(thisX < resX * resY)
    {
        float x = lowerX + thisX % resX * stepX;
        float y = lowerY + thisX / resX * stepY;
    
        //int idx = thisY * resX + thisX;
        img[thisX] = cal(x, y, maxIterations);
    }
}

// Host front-end function that allocates the memory and launches the GPU kernel 
void hostFE (float upperX, float upperY, float lowerX, float lowerY, int* img, int resX, int resY, int maxIterations)
{
    int size = resX * resY * sizeof(int);
    int n_thread = 256;
    
    float stepX = (upperX - lowerX) / resX;
    float stepY = (upperY - lowerY) / resY;
    
    int *d_img;
    cudaMalloc((void **)&d_img, size);
    
    // dim3 blockSize(n_thread, n_thread);
    // dim3 numBlock(resX / n_thread, resY / n_thread);

    mandelKernel<<<(resX * resY + 255) / n_thread, n_thread>>>(d_img, lowerX, lowerY, stepX, stepY, maxIterations, resX, resY);

    cudaDeviceSynchronize();

    cudaMemcpy(img, d_img, size, cudaMemcpyDeviceToHost);
    
    cudaFree(d_img);
}
