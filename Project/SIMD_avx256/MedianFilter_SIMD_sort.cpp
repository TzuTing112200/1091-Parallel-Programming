#include <stdio.h>
#include <algorithm>
#include <opencv2/opencv.hpp>

#include <immintrin.h>

using namespace cv;
int main(int argc, char** argv )
{
    Mat img;
    img = imread("./4K_noise.jpg", IMREAD_COLOR);
    if ( !img.data )
    {
        printf("No image data \n");
        return -1;
    }

    int h = img.rows, w = img.cols;
    Mat image = Mat::zeros(h, w, CV_8UC3);

    int kernel_size = 3, count = 0;
    int total = kernel_size*kernel_size;
    int p = floor(kernel_size/2);
    int index = floor(total/2);
    //int *vs = new int[total];

    __m256i _V0, _V1, _V2, _V3, _V4, _V5, _V6, _V7, _V8, _V9;
    int8_t *V0 = (int8_t *) &_V0;
    int8_t *V1 = (int8_t *) &_V1;
    int8_t *V2 = (int8_t *) &_V2;
    int8_t *V3 = (int8_t *) &_V3;
    int8_t *V4 = (int8_t *) &_V4;
    int8_t *V5 = (int8_t *) &_V5;
    int8_t *V6 = (int8_t *) &_V6;
    int8_t *V7 = (int8_t *) &_V7;
    int8_t *V8 = (int8_t *) &_V8;
    int8_t *V9 = (int8_t *) &_V9;

    //double v = 0;

    for(int j = 1 ; j < h - 1 ; j++)
    {
    	for(int i = 1 ; i < w - 1 ; i += 10)
        {
            for(int k = 0 ; k < 10 ; k++)
            {
                V1[k] = img.at<cv::Vec3b>(j,i + k)[0];
                V1[k + 10] = img.at<cv::Vec3b>(j,i + k)[1];
                V1[k + 20] = img.at<cv::Vec3b>(j,i + k)[2];

                V2[k] = img.at<cv::Vec3b>(j - 1,i + k - 1)[0];
                V2[k + 10] = img.at<cv::Vec3b>(j - 1,i + k - 1)[1];
                V2[k + 20] = img.at<cv::Vec3b>(j - 1,i + k - 1)[2];

                V3[k] = img.at<cv::Vec3b>(j - 1,i + k)[0];
                V3[k + 10] = img.at<cv::Vec3b>(j - 1,i + k)[1];
                V3[k + 20] = img.at<cv::Vec3b>(j - 1,i + k)[2];

                V4[k] = img.at<cv::Vec3b>(j - 1,i + k + 1)[0];
                V4[k + 10] = img.at<cv::Vec3b>(j - 1,i + k + 1)[1];
                V4[k + 20] = img.at<cv::Vec3b>(j - 1,i + k + 1)[2];

                V5[k] = img.at<cv::Vec3b>(j,i + k - 1)[0];
                V5[k + 10] = img.at<cv::Vec3b>(j,i + k - 1)[1];
                V5[k + 20] = img.at<cv::Vec3b>(j,i + k - 1)[2];

                V6[k] = img.at<cv::Vec3b>(j,i + k + 1)[0];
                V6[k + 10] = img.at<cv::Vec3b>(j,i + k + 1)[1];
                V6[k + 20] = img.at<cv::Vec3b>(j,i + k + 1)[2];

                V7[k] = img.at<cv::Vec3b>(j + 1,i + k - 1)[0];
                V7[k + 10] = img.at<cv::Vec3b>(j + 1,i + k - 1)[1];
                V7[k + 20] = img.at<cv::Vec3b>(j + 1,i + k - 1)[2];

                V8[k] = img.at<cv::Vec3b>(j + 1,i + k)[0];
                V8[k + 10] = img.at<cv::Vec3b>(j + 1,i + k)[1];
                V8[k + 20] = img.at<cv::Vec3b>(j + 1,i + k)[2];

                V9[k] = img.at<cv::Vec3b>(j + 1,i + k + 1)[0];
                V9[k + 10] = img.at<cv::Vec3b>(j + 1,i + k + 1)[1];
                V9[k + 20] = img.at<cv::Vec3b>(j + 1,i + k + 1)[2];
            }

            _V0 = _mm256_cmpgt_epi8(_V1, _V2);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V2[ii];
                    V2[ii] = V1[ii];
                    V1[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V2, _V3);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V3[ii];
                    V3[ii] = V2[ii];
                    V2[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V3, _V4);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V4[ii];
                    V4[ii] = V3[ii];
                    V3[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V4, _V5);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V5[ii];
                    V5[ii] = V4[ii];
                    V4[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V5, _V6);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V6[ii];
                    V6[ii] = V5[ii];
                    V5[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V6, _V7);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V7[ii];
                    V7[ii] = V6[ii];
                    V6[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V7, _V8);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V8[ii];
                    V8[ii] = V7[ii];
                    V7[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V8, _V9);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V9[ii];
                    V9[ii] = V8[ii];
                    V8[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V1, _V2);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V2[ii];
                    V2[ii] = V1[ii];
                    V1[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V2, _V3);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V3[ii];
                    V3[ii] = V2[ii];
                    V2[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V3, _V4);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V4[ii];
                    V4[ii] = V3[ii];
                    V3[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V4, _V5);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V5[ii];
                    V5[ii] = V4[ii];
                    V4[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V5, _V6);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V6[ii];
                    V6[ii] = V5[ii];
                    V5[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V6, _V7);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V7[ii];
                    V7[ii] = V6[ii];
                    V6[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V7, _V8);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V8[ii];
                    V8[ii] = V7[ii];
                    V7[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V1, _V2);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V2[ii];
                    V2[ii] = V1[ii];
                    V1[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V2, _V3);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V3[ii];
                    V3[ii] = V2[ii];
                    V2[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V3, _V4);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V4[ii];
                    V4[ii] = V3[ii];
                    V3[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V4, _V5);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V5[ii];
                    V5[ii] = V4[ii];
                    V4[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V5, _V6);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V6[ii];
                    V6[ii] = V5[ii];
                    V5[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V6, _V7);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V7[ii];
                    V7[ii] = V6[ii];
                    V6[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V1, _V2);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V2[ii];
                    V2[ii] = V1[ii];
                    V1[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V2, _V3);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V3[ii];
                    V3[ii] = V2[ii];
                    V2[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V3, _V4);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V4[ii];
                    V4[ii] = V3[ii];
                    V3[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V4, _V5);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V5[ii];
                    V5[ii] = V4[ii];
                    V4[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V5, _V6);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V6[ii];
                    V6[ii] = V5[ii];
                    V5[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V1, _V2);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V2[ii];
                    V2[ii] = V1[ii];
                    V1[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V2, _V3);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V3[ii];
                    V3[ii] = V2[ii];
                    V2[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V3, _V4);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V4[ii];
                    V4[ii] = V3[ii];
                    V3[ii] = V0[ii];
                }

            _V0 = _mm256_cmpgt_epi8(_V4, _V5);
            for(int ii = 0; ii < 30; ii++)
                if(V0[ii] == -1)
                {
                    V0[ii] = V5[ii];
                    V5[ii] = V4[ii];
                    V4[ii] = V0[ii];
                }

            for(int k = 0 ; k < 10 ; k++)
            {
                image.at<cv::Vec3b>(j,i + k)[0] = (uchar)V5[k];
                image.at<cv::Vec3b>(j,i + k)[1] = (uchar)V5[k + 10];
                image.at<cv::Vec3b>(j,i + k)[2] = (uchar)V5[k + 20];
            }
  	    }
	}



    for(int j = 1 ; j < h - 1 ; j++)
    {
        image.at<cv::Vec3b>(j,0)[0] = image.at<cv::Vec3b>(j,1)[0];
        image.at<cv::Vec3b>(j,0)[1] = image.at<cv::Vec3b>(j,1)[1];
        image.at<cv::Vec3b>(j,0)[2] = image.at<cv::Vec3b>(j,1)[2];
        image.at<cv::Vec3b>(j,w-1)[0] = image.at<cv::Vec3b>(j,w-2)[0];
        image.at<cv::Vec3b>(j,w-1)[1] = image.at<cv::Vec3b>(j,w-2)[1];
        image.at<cv::Vec3b>(j,w-1)[2] = image.at<cv::Vec3b>(j,w-2)[2];
    }
    for(int i = 1 ; i < w - 1 ; i++)
    {
        image.at<cv::Vec3b>(0,i)[0] = image.at<cv::Vec3b>(1,i)[0];
        image.at<cv::Vec3b>(0,i)[1] = image.at<cv::Vec3b>(1,i)[1];
        image.at<cv::Vec3b>(0,i)[2] = image.at<cv::Vec3b>(1,i)[2];
        image.at<cv::Vec3b>(h-1,i)[0] = image.at<cv::Vec3b>(h-2,i)[0];
        image.at<cv::Vec3b>(h-1,i)[1] = image.at<cv::Vec3b>(h-2,i)[1];
        image.at<cv::Vec3b>(h-1,i)[2] = image.at<cv::Vec3b>(h-2,i)[2];
    }

    image.at<cv::Vec3b>(0,0)[0] = image.at<cv::Vec3b>(1,1)[0];
    image.at<cv::Vec3b>(0,w-1)[0] = image.at<cv::Vec3b>(1,w-2)[0];
    image.at<cv::Vec3b>(h-1,0)[0] = image.at<cv::Vec3b>(h-2,1)[0];
    image.at<cv::Vec3b>(h-1,w-1)[0] = image.at<cv::Vec3b>(h-2,w-2)[0];

    image.at<cv::Vec3b>(0,0)[1] = image.at<cv::Vec3b>(1,1)[1];
    image.at<cv::Vec3b>(0,w-1)[1] = image.at<cv::Vec3b>(1,w-2)[1];
    image.at<cv::Vec3b>(h-1,0)[1] = image.at<cv::Vec3b>(h-2,1)[1];
    image.at<cv::Vec3b>(h-1,w-1)[1] = image.at<cv::Vec3b>(h-2,w-2)[1];

    image.at<cv::Vec3b>(0,0)[2] = image.at<cv::Vec3b>(1,1)[2];
    image.at<cv::Vec3b>(0,w-1)[2] = image.at<cv::Vec3b>(1,w-2)[2];
    image.at<cv::Vec3b>(h-1,0)[2] = image.at<cv::Vec3b>(h-2,1)[2];
    image.at<cv::Vec3b>(h-1,w-1)[2] = image.at<cv::Vec3b>(h-2,w-2)[2];

    imwrite("output.jpg", image);

    return 0;
}
