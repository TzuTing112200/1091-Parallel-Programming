#include <stdio.h>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <pthread.h>
//#include <sys/time.h>
#include <stdlib.h>

using namespace cv;

#define thread_num 4
#define iteration 1
Mat image;
int avgrow;
struct parameter{
    Mat img;
    int id;
    int work_num;
    int h;
    int w;
};

void* medianfilter(void *arg){
    struct parameter *para = (struct parameter *)arg;
    Mat img = para->img;
    //imwrite("./test.jpg", img);
    //int work_num = para->work_num;
    int h = para->h;
    int w = para->w;
    int id = para->id;
    int work_num = para->work_num;
    
    int kernel_size = 3, count = 0;
    int total = kernel_size*kernel_size;
    int p = floor(kernel_size/2);
    int *vs = new int[kernel_size*kernel_size];

    double v = 0;
    
    
        for(int j = id * avgrow ; j < id * avgrow + work_num ; j++){
            for(int i = 0 ; i < w ; i++){
                for (int c = 0 ; c < 3 ; c++){
                    v = 0;
                    count = 0;

                    for(int t = 0 ; t < total ; t++){
                        vs[t] = 0;
                    }

                    for(int _j = -p ; _j < p+1 ; _j++){
                        for (int _i = -p; _i < p+1;_i++){
                            if(((j + _j) >= 0)&&((i + _i) >=0) && ((j + _j) < h) && (i + _i) < w){
                                vs[count++] = (int)img.at<cv::Vec3b>(j + _j,i + _i)[c];
                            }
                        }
                    }

                    std::sort(vs,vs + total);
                    image.at<cv::Vec3b>(j,i)[c] = (uchar)vs[int(floor(total/2))];
                    
                }
            }
        }
        
} 

int main(int argc, char** argv ){
    Mat img;
    img = imread("./4K_noise.jpg", IMREAD_COLOR);
    int h = img.rows, w = img.cols;
    if ( !img.data )
    {
        printf("No image data \n");
        return -1;
    }
    struct timeval start, end;
    //gettimeofday(&start, 0);

    pthread_t thread_id[thread_num];
    struct parameter para[thread_num];
    for(int j = 0; j < iteration; j++){
        image = Mat::zeros(h, w, CV_8UC3);
        for(int i = 0 ; i < thread_num; i++){
            para[i].img = img;
            para[i].id = i;
            para[i].h = h;
            para[i].w = w;
            
            if(i == thread_num -1){
                para[i].work_num = h/thread_num + h%thread_num;
            }
            else{
                para[i].work_num = h/thread_num;
            }
            avgrow = h/thread_num;
            pthread_create(&thread_id[i], NULL, medianfilter, (void *)&para[i]);
        }
        
        for (register int i = 0; i < thread_num; i++){
            pthread_join(thread_id[i], NULL);
        }
        imwrite("./output.jpg", image);
    }    
    
    //gettimeofday(&end, 0); 
    //int sec = abs(end.tv_sec - start.tv_sec);
    //int usec = abs(end.tv_usec - start.tv_usec);  
    //printf("MedianFilter for %d iteration elapsed time %f ms. \n",iteration ,sec * 1000.0f + usec / 1000.0f);
    return 0;
}
