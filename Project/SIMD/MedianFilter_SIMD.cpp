#include <stdio.h>
#include <algorithm>
#include <opencv2/opencv.hpp>

#include <immintrin.h>

using namespace cv;
int main(int argc, char** argv )
{
    Mat img;
    img = imread("./4K_ultra_noise.jpg", IMREAD_COLOR);
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

    __m128i _windows;
    __m128i _tempValue;
    __m128i _gt, _eq;

    short *windows = (short *) &_windows;
    short *tempValue = (short *) &_tempValue;
    short *gt = (short *) &_gt;
    short *eq = (short *) &_eq;

    //double v = 0;

    for(int j = 1 ; j < h - 1 ; j++)
    {
    	for(int i = 1 ; i < w - 1 ; i++)
        {
            for (int c = 0 ; c < 3 ; c++)
            {
                //v = 0;
                //count = 0;

                //for(int t = 0 ; t < total ; t++)
                //{
                    //vs[t] = 0;
                //}

                //for(int _j = -p ; _j < p+1 ; _j++)
                //{
                    //for (int _i = -p; _i < p+1;_i++)
                    //{
                        //vs[count++] = (int)img.at<cv::Vec3b>(j + _j,i + _i)[c];
                    //}
                //}

                _tempValue = _mm_set1_epi16((short)img.at<cv::Vec3b>(j,i)[c]);
                windows[0] = (short)img.at<cv::Vec3b>(j-1,i-1)[c];
                windows[1] = (short)img.at<cv::Vec3b>(j-1,i)[c];
                windows[2] = (short)img.at<cv::Vec3b>(j-1,i+1)[c];
                windows[3] = (short)img.at<cv::Vec3b>(j,i-1)[c];
                windows[4] = (short)img.at<cv::Vec3b>(j,i+1)[c];
                windows[5] = (short)img.at<cv::Vec3b>(j+1,i-1)[c];
                windows[6] = (short)img.at<cv::Vec3b>(j+1,i)[c];
                windows[7] = (short)img.at<cv::Vec3b>(j+1,i+1)[c];

                _gt = _mm_cmpgt_epi16(_tempValue, _windows);
                _eq = _mm_cmpeq_epi16(_tempValue, _windows);

                int __gt = 0, __eq = 0;
                for (int n = 0; n < 8; n++)
                {
                    __gt -= gt[n];
                    __eq -= eq[n];
                }

                if ((__gt) < 5 && ((__gt + __eq) > 3))
                {
                    image.at<cv::Vec3b>(j,i)[c] = (uchar)tempValue[0];
                    continue;
                }

                int m = 0;
                for (; m < 7; m++)
                {
                    short temp = tempValue[0];
                    _tempValue = _mm_set1_epi16(windows[m]);
                    windows[m] = temp;

                    _gt = _mm_cmpgt_epi16(_tempValue, _windows);
                    _eq = _mm_cmpeq_epi16(_tempValue, _windows);

                    int __gt = 0, __eq = 0;
                    for (int n = 0; n < 8; n++)
                    {
                        __gt -= gt[n];
                        __eq -= eq[n];
                    }

                    if ((__gt < 5) && ((__gt + __eq) > 3))
                    {
                        image.at<cv::Vec3b>(j,i)[c] = (uchar)tempValue[m];
                        break;
                    }
                }

                if (m == 7)
                    image.at<cv::Vec3b>(j,i)[c] = (uchar)windows[7];

                //std::sort(vs,vs + total);
                //image.at<cv::Vec3b>(j,i)[c] = (uchar)vs[int(floor(total/2))];
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
