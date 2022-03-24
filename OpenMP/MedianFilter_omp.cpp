#include <stdio.h>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "omp.h"

using namespace cv;
int main(int argc, char** argv ){
    Mat img;
    img = imread("./4K_ultra_noise.jpg", IMREAD_COLOR);
    if ( !img.data )
    {
        printf("No image data \n");
        return -1;
    }
    int h = img.rows, w = img.cols;
    Mat image = Mat::zeros(h, w, CV_8UC3);

    int kernel_size = 3;
    int total = kernel_size*kernel_size;
    int p = floor(kernel_size/2);
    //int *vs = new int[kernel_size*kernel_size];
    int vs[3][total], count[3], i, j;
    //double v = 0;
    
    #pragma omp parallel for private(i, j, count, vs) collapse(2)
    for(j = 0 ; j < h ; j++){
    	for(i = 0 ; i < w ; i++){
            count[0] = 0;
    		count[1] = 0;
    		count[2] = 0;
            for(int t = 0 ; t < total ; t++){
                vs[0][t] = 0;
    		    vs[1][t] = 0;
    		    vs[2][t] = 0;
            }
            for(int _j = -p ; _j < p+1 ; _j++){
                for (int _i = -p; _i < p+1;_i++){
                    if(((j + _j) >= 0) && ((i + _i) >=0) && ((j + _j) < h) && ((i + _i) < w)){
        				vs[0][count[0]++] = (int)img.at<cv::Vec3b>(j + _j,i + _i)[0];
        				vs[1][count[1]++] = (int)img.at<cv::Vec3b>(j + _j,i + _i)[1];
        				vs[2][count[2]++] = (int)img.at<cv::Vec3b>(j + _j,i + _i)[2];
                    }
                }
            }
            std::sort(vs[0],vs[0] + total);
    		std::sort(vs[1],vs[1] + total);
    		std::sort(vs[2],vs[2] + total);
            image.at<cv::Vec3b>(j,i)[0] = (uchar)vs[0][int(floor(total/2))];
    		image.at<cv::Vec3b>(j,i)[1] = (uchar)vs[1][int(floor(total/2))];
    		image.at<cv::Vec3b>(j,i)[2] = (uchar)vs[2][int(floor(total/2))];
    	}
	}

    imwrite("output.jpg", image);

    return 0;
}

