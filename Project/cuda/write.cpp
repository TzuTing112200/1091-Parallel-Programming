#include <stdio.h>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
using namespace cv;
using namespace std;
int main(int argc, char** argv){
	Mat img;
	img = imread("./4K_ultra_noise.jpg", IMREAD_COLOR);
	if ( !img.data )
    {
        printf("No image data \n");
        return -1;
    }
    fstream file;
    file.open("./4K_ultra_noise.txt", ios::out);
    int h = img.rows, w = img.cols;
    file << h << " "<< w << " "<< 3 <<"\n";
    cout<<h<<" "<<w<<" "<<endl;
    for(int i = 0; i < h; i++){
    	for (int j = 0; j < w; j++){
    		for(int c = 0; c < 3; c++){
    			file<<(int)img.at<cv::Vec3b>(i,j)[c]<<" ";
    		}
    		file<<"\n";
    	}
    }
    file.close();
    
    return 0;
}