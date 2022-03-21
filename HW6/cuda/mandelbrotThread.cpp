#include "kernel.h"

//
// MandelbrotThread --
//
// Multi-threaded implementation of mandelbrot set image generation.
// Threads of execution are created by using CUDA
void mandelbrotThread(int filterWidth, float *filter, int imageHeight, int imageWidth, float *inputImage, float *outputImage)
{
    hostFE(filterWidth, filter, imageHeight, imageWidth, inputImage, outputImage);
}
