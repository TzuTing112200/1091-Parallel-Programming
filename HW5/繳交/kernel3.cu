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

__global__ void mandelKernel(int *img, size_t pitch, float lowerX, float lowerY, float stepX, float stepY, int maxIterations, int resX, int resY) {
    // To avoid error caused by the floating number, use the following pseudo code
    //
    // float x = lowerX + thisX * stepX;
    // float y = lowerY + thisY * stepY;
	
    int thisX;
    int thisY = blockIdx.y * blockDim.y + threadIdx.y;
	
	//int index = blockIdx.x * (pitch / sizeof(int)) + threadIdx.x;
    for(int i = 0; i < 16; i++)
	{
		thisX = blockIdx.x * blockDim.x + i * 256 + threadIdx.x + i;
		if(thisX < resX && thisY < resY)
		{
			float x = lowerX + thisX * stepX;
			float y = lowerY + thisY * stepY;
		
			int idx = thisY * resX + thisX;
			img[idx] = cal(x, y, maxIterations);
		}
	}
}

// Host front-end function that allocates the memory and launches the GPU kernel
void hostFE (float upperX, float upperY, float lowerX, float lowerY, int* img, int resX, int resY, int maxIterations)
{
    int size = resX * resY * sizeof(int);
    int n_thread = 16;
	
    float stepX = (upperX - lowerX) / resX;
    float stepY = (upperY - lowerY) / resY;
    
    int *h_img;
	cudaHostAlloc((void**)&h_img, size,cudaHostAllocDefault);
	
    int *d_img;
    size_t pitch;
    cudaMallocPitch(&d_img, &pitch, resX * sizeof(int), resY);
    
    dim3 blockSize(n_thread, n_thread);
    dim3 numBlock(resX / n_thread / 4, resY / n_thread);

    mandelKernel<<<numBlock, blockSize>>>(d_img, pitch, lowerX, lowerY, stepX, stepY, maxIterations, resX, resY);

    cudaDeviceSynchronize();
	
    cudaMemcpy(h_img, d_img, size, cudaMemcpyDeviceToHost);
    memcpy(img, h_img, size);
	
    cudaFree(d_img);
	cudaFree(h_img);
}
