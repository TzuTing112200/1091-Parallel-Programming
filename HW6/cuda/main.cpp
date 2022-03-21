#include <stdio.h>
#include <algorithm>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "CycleTimer.h"

extern void mandelbrotThread(int filterWidth, float *filter, int imageHeight, int imageWidth, float *inputImage, float *outputImage);

void storeImage(float *imageOut, const char *filename, int rows, int cols,
                const char *refFilename);
				
float *readImage(const char *filename, int *widthOut, int *heightOut);

char *readSource(char *kernelPath);

float *readFilter(const char *filename, int *filterWidth);

void usage(const char *progname)
{
   printf("Usage: %s [options]\n", progname);
   printf("Program Options:\n");
   printf("  -f  --filter  <INT>    Use which filter (0, 1, 2)\n");
   printf("  -?  --help             This message\n");
}

int main(int argc, char **argv)
{

    int imageHeight;
    int imageWidth;
    bool isGPUOnly = false;

    double start_time, end_time;

    const char *inputFile = "input.bmp";
    const char *outputFile = "output.bmp";
    const char *refFile = "ref.bmp";
    char *filterFile = "filter1.csv";

    // parse commandline options ////////////////////////////////////////////
    int opt;
    static struct option long_options[] = {
        {"filter", 1, 0, 'f'},
        {"help", 0, 0, '?'},
        {0, 0, 0, 0}};

    while ((opt = getopt_long(argc, argv, "f:?", long_options, NULL)) != EOF)
    {

       switch (opt)
       {
       case 'f':
       {
           int idx = atoi(optarg);
           if (idx == 2)
               filterFile = "filter2.csv";
           else if (idx == 3)
               filterFile = "filter3.csv";

           break;
       }
       case '?':
       default:
           usage(argv[0]);
           return 1;
       }
    }
    // end parsing of commandline options
	
	// read filter data
	int filterWidth;
	float *filter = readFilter(filterFile, &filterWidth);

	// Homegrown function to read a BMP from file
	float *inputImage = readImage(inputFile, &imageWidth, &imageHeight);
	// Size of the input and output images on the host
	int dataSize = imageHeight * imageWidth * sizeof(float);
	// Output image on the host
	float *outputImage = (float *)malloc(dataSize);

    //
    // Run the threaded version
    //

    double minThread = 0;
    double recordThread[10] = {0};
    for (int i = 0; i < 10; ++i)
    {
        memset(outputImage, 0, dataSize);
        double startTime = CycleTimer::currentSeconds();
        mandelbrotThread(filterWidth, filter, imageHeight, imageWidth, inputImage, outputImage);
        double endTime = CycleTimer::currentSeconds();
        recordThread[i] = endTime - startTime;
    }
    std::sort(recordThread, recordThread + 10);
    for (int i = 3; i < 7; ++i)
    {
        minThread += recordThread[i];
    }
    minThread /= 4;

    printf("[conv cuda]:\t\t[%.3f] ms\n", minThread * 1000);
   storeImage(outputImage, outputFile, imageHeight, imageWidth, inputFile);

    delete[] filter;
    delete[] inputImage;
    delete[] outputImage;

    return 0;
}
typedef unsigned char uchar;

void storeImage(float *imageOut, const char *filename, int rows, int cols,
                const char *refFilename)
{

   FILE *ifp, *ofp;
   unsigned char tmp;
   int offset;
   unsigned char *buffer;
   int i, j;

   int bytes;

   int height, width;

   ifp = fopen(refFilename, "rb");
   if (ifp == NULL)
   {
      perror(filename);
      exit(-1);
   }

   fseek(ifp, 10, SEEK_SET);
   fread(&offset, 4, 1, ifp);

   fseek(ifp, 18, SEEK_SET);
   fread(&width, 4, 1, ifp);
   fread(&height, 4, 1, ifp);

   fseek(ifp, 0, SEEK_SET);

   buffer = (unsigned char *)malloc(offset);
   if (buffer == NULL)
   {
      perror("malloc");
      exit(-1);
   }

   fread(buffer, 1, offset, ifp);

   printf("Writing output image to %s\n", filename);
   ofp = fopen(filename, "wb");
   if (ofp == NULL)
   {
      perror("opening output file");
      exit(-1);
   }
   bytes = fwrite(buffer, 1, offset, ofp);
   if (bytes != offset)
   {
      printf("error writing header!\n");
      exit(-1);
   }

   // NOTE bmp formats store data in reverse raster order (see comment in
   // readImage function), so we need to flip it upside down here.
   int mod = width % 4;
   if (mod != 0)
   {
      mod = 4 - mod;
   }
   //   printf("mod = %d\n", mod);
   for (i = height - 1; i >= 0; i--)
   {
      for (j = 0; j < width; j++)
      {
         tmp = (unsigned char)imageOut[i * cols + j];
         fwrite(&tmp, sizeof(char), 1, ofp);
      }
      // In bmp format, rows must be a multiple of 4-bytes.
      // So if we're not at a multiple of 4, add junk padding.
      for (j = 0; j < mod; j++)
      {
         fwrite(&tmp, sizeof(char), 1, ofp);
      }
   }

   fclose(ofp);
   fclose(ifp);

   free(buffer);
}

/*
 * Read bmp image and convert to byte array. Also output the width and height
 */
float *readImage(const char *filename, int *widthOut, int *heightOut)
{

   uchar *imageData;

   int height, width;
   uchar tmp;
   int offset;
   int i, j;

   printf("Reading input image from %s\n", filename);
   FILE *fp = fopen(filename, "rb");
   if (fp == NULL)
   {
      perror(filename);
      exit(-1);
   }

   fseek(fp, 10, SEEK_SET);
   fread(&offset, 4, 1, fp);

   fseek(fp, 18, SEEK_SET);
   fread(&width, 4, 1, fp);
   fread(&height, 4, 1, fp);

   printf("width = %d\n", width);
   printf("height = %d\n", height);

   *widthOut = width;
   *heightOut = height;

   imageData = (uchar *)malloc(width * height);
   if (imageData == NULL)
   {
      perror("malloc");
      exit(-1);
   }

   fseek(fp, offset, SEEK_SET);
   fflush(NULL);

   int mod = width % 4;
   if (mod != 0)
   {
      mod = 4 - mod;
   }

   // NOTE bitmaps are stored in upside-down raster order.  So we begin
   // reading from the bottom left pixel, then going from left-to-right,
   // read from the bottom to the top of the image.  For image analysis,
   // we want the image to be right-side up, so we'll modify it here.

   // First we read the image in upside-down

   // Read in the actual image
   for (i = 0; i < height; i++)
   {

      // add actual data to the image
      for (j = 0; j < width; j++)
      {
         fread(&tmp, sizeof(char), 1, fp);
         imageData[i * width + j] = tmp;
      }
      // For the bmp format, each row has to be a multiple of 4,
      // so I need to read in the junk data and throw it away
      for (j = 0; j < mod; j++)
      {
         fread(&tmp, sizeof(char), 1, fp);
      }
   }

   // Then we flip it over
   int flipRow;
   for (i = 0; i < height / 2; i++)
   {
      flipRow = height - (i + 1);
      for (j = 0; j < width; j++)
      {
         tmp = imageData[i * width + j];
         imageData[i * width + j] = imageData[flipRow * width + j];
         imageData[flipRow * width + j] = tmp;
      }
   }

   fclose(fp);

   // Input image on the host
   float *floatImage = NULL;
   floatImage = (float *)malloc(sizeof(float) * width * height);
   if (floatImage == NULL)
   {
      perror("malloc");
      exit(-1);
   }

   // Convert the BMP image to float (not required)
   for (i = 0; i < height; i++)
   {
      for (j = 0; j < width; j++)
      {
         floatImage[i * width + j] = (float)imageData[i * width + j];
      }
   }

   free(imageData);
   return floatImage;
}

float *readFilter(const char *filename, int *filterWidth)
{
    printf("Reading filter data from %s\n", filename);

    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Could not open filter file\n");
        exit(-1);
    }

    fscanf(fp, "%d", filterWidth);

    float *filter = (float *)malloc(*filterWidth * *filterWidth * sizeof(float));

    float tmp;
    for (int i = 0; i < *filterWidth * *filterWidth; i++)
    {
        fscanf(fp, "%f", &tmp);
        filter[i] = tmp;
    }

    printf("Filter width: %d\n", *filterWidth);

    fclose(fp);
    return filter;
}
