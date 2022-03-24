#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <iostream>
#include <algorithm>
#include <time.h>

using namespace std;
#define iteration 1
#define thread_num 16
#define kernel_size 3

void cuda_err_chk(const cudaError_t& e, const int& cudaError_cnt){
    if(e != cudaSuccess){
        fprintf(stderr, "cudaError in no. %d: %s\n", cudaError_cnt, cudaGetErrorString(e));
        exit(EXIT_FAILURE);
    }
}

__global__ void median_filter(int* img, int* image, int h , int w){
    
    int i = blockIdx.y * blockDim.y + threadIdx.y;
    int j = blockIdx.x * blockDim.x + threadIdx.x;

    int count = 0;
    int total = kernel_size*kernel_size;
    int p = (int)(kernel_size/2);
    int vs_x[kernel_size*kernel_size];
    int vs_y[kernel_size*kernel_size];
    int vs_z[kernel_size*kernel_size];

    if(j >= h || i >= w)return;
    
   
        count = 0;

        for(int t = 0 ; t < total ; t++){
            vs_x[t] = 0;
            vs_y[t] = 0;
            vs_z[t] = 0;
        }
        int now = ((j)*w + i)*3;
        for(int _j = -p ; _j < p+1 ; _j++){
            for (int _i = -p; _i < p+1;_i++){
                if(((j + _j) >= 0)&&((i + _i) >=0) && ((j + _j) < h) && (i + _i) < w){
                    //vs[count++] = (int)cu_img.at<cv::Vec3b>(j + _j,i + _i)[c];
                    int pixel = ((j + _j)*w +(i + _i))*3;
                    vs_x[count] = img[pixel];
                    vs_y[count] = img[pixel + 1];
                    vs_z[count++] = img[pixel + 2];
                }
            }
        }
        
        int temp;
        for(int k = 0; k < total; k++){
            for(int a = 0; a < total; a++){
                if(vs_x[k] > vs_x[a]){
                    temp = vs_x[k];
                    vs_x[k] = vs_x[a];
                    vs_x[a] = temp;
                }
                if(vs_y[k] > vs_y[a]){
                    temp = vs_y[k];
                    vs_y[k] = vs_y[a];
                    vs_y[a] = temp;
                }
                if(vs_z[k] > vs_z[a]){
                    temp = vs_z[k];
                    vs_z[k] = vs_z[a];
                    vs_z[a] = temp;
                }
            }
        }
        image[now] = vs_x[total/2];
        image[now + 1] = vs_y[total/2];

        image[now + 2] = vs_z[total/2];
             
    
}

int main(int argc, char **argv){
    
    clock_t t1, t2;
    t1 = clock();
    
    for(int i = 0; i < iteration; i++){
        int cudaError_cnt = 0;
        int *img_int;
        int *image_int;
        FILE *fp;
        fp = fopen("./4K_ultra_noise.txt", "r");
        cout<<"start"<<endl;
        int h , w, rgb;
        fscanf(fp,"%d %d %d",&h,&w,&rgb);
        img_int = (int *)malloc(h * w * 3 * sizeof(int));
        
        image_int = (int*)malloc(h * w * 3 * sizeof(int));
        cout<<h<<" "<<w<<" "<< rgb <<endl;
        for(int j = 0; j < h; j++){
            for(int k = 0; k < w; k++){
                for(int c = 0; c < 3; c++){
                        //file >> *(img_int + ((j * w) + k) * 3 + c) ;
                        fscanf(fp,"%d", (img_int + ((j * w) + k) * 3 + c) );
                        
                }
            }
            
        }
        
        t1 = clock();
        int *img_cuda;
        int *image_cuda;
        cuda_err_chk(cudaMalloc((void**) &img_cuda, h * w * 3 * sizeof(int)), cudaError_cnt++);
        cuda_err_chk(cudaMalloc((void**) &image_cuda, h * w * 3 * sizeof(int)), cudaError_cnt++);
        cuda_err_chk(cudaMemcpy(img_cuda, img_int, h * w * 3 * sizeof(int), cudaMemcpyHostToDevice), cudaError_cnt++);
        //cuda_err_chk(cudaMemcpy(image_cuda, image_int, h * w * 3 * sizeof(int), cudaMemcpyHostToDevice), cudaError_cnt++);

        
        dim3 threadsPerBlock(thread_num, thread_num);
        dim3 numBlocks((h + thread_num - 1)/thread_num, (w + thread_num - 1)/thread_num);
        median_filter<<<numBlocks, threadsPerBlock>>>(img_cuda,image_cuda, h, w);
        cuda_err_chk(cudaDeviceSynchronize(), cudaError_cnt++);
        
        cuda_err_chk(cudaMemcpy(image_int, image_cuda, h * w * 3 * sizeof(int), cudaMemcpyDeviceToHost), cudaError_cnt++);
        t2 = clock();
        
        FILE *fp2;
        fp2 = fopen("./4k_ultra_cuda.txt", "w");
        fprintf(fp2,"%d %d %d\n" , h, w, rgb);
        for(int j = 0; j < h; j++){
            
            for(int k = 0; k < w; k++){
                for(int c = 0; c < 3; c++){
                        //file2 <<image_int[((j * w) + k) *3 + c]<<" ";
                        fprintf(fp2,"%d " , image_int[((j * w) + k) *3 + c]);
                        
                }
                fprintf(fp2,"\n");
            }
        }
        free(img_int);
        free(image_int);
        cudaFree(img_cuda);
        cudaFree(image_cuda);
        
        printf("%lf \n", (t2-t1)/(double)(CLOCKS_PER_SEC));
        
        fclose ( fp );
        fclose ( fp2 );
        
    }
    
    
    
}