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
    ifstream file;
    file.open("./4K_ultra_cuda.txt");
    int h , w, c;
    file >> h;
    file >> w;
    file >> c;
    cout<<h<<" "<<w<<" "<<c<<endl;
    Mat image = Mat::zeros(h, w, CV_8UC3);
    int u = 0;
    int temp;
    for(int i = 0; i < h; i++){
    	for (int j = 0; j < w; j++){
    		for(int k = 0; k < c; k++){
    			file >> temp;
                image.at<cv::Vec3b>(i, j)[k] = (uchar)temp;
                if(u == 0)cout<<image.at<cv::Vec3b>(i, j)[k]<<" ";

    		}
            if(u == 0)
            cout<<endl;
            u++;
    	}
    }
    file.close();
    imwrite("write.jpg", image);
    return 0;
}
