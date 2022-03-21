#include <iostream>
#include <iomanip>
#include <cstdlib>
//#include <ctime>
#include <algorithm>
#include <math.h>
#include <pthread.h>
//#include <immintrin.h>
#include <vector>
#include <numeric>
using namespace std;


int number_of_CPU_cores = 4;
long long int number_of_tosses = 100000000;
long long int part_number_of_tosses = 0;


// Estimate PI value
void *getNumberInCircle(void *val)
{
    unsigned int seed = time(NULL) + pthread_self();
    int rand_num;
	double x, y;

    //__m256 _x;
    //__m256 _y;
	
    /*
	for(int i = 0; i < 8; i++)
    {
        _x[i] = (double)(rand_num_1 & 0xffff) / 0xffff;
        rand_num_1 >> 1;
        _y[i] = (double)(rand_num_1 & 0xffff) / 0xffff;
        rand_num_1 >> 1;
    }
	*/

    long long int temp_number_in_circle = 0;

    for(long long int toss = 0; toss < part_number_of_tosses; toss += 1)
    {
        rand_num = rand_r(&seed);
		x = (double)(rand_num & 0xffff) / 0xffff;
        rand_num = rand_r(&seed);
		y = (double)(rand_num & 0xffff) / 0xffff;
		
        //_x = _mm256_mul_ps(_x, _x);
        //_y = _mm256_mul_ps(_y, _y);

        //_x = _mm256_add_ps(_x, _y);
		if((x*x + y*y) < 1.0) temp_number_in_circle++;
		/*
        for(int i = 0; i < 8; i ++)
        {
            if(_x[i] < 1)
                temp_number_in_circle++;

            _x[i] = (float)(rand_num_1 & 0xffff) / 0xffff;
            _y[i] = (float)(rand_num_2 & 0xffff) / 0xffff;
            rand_num_1 >> 2;
            rand_num_2 >> 2;
        }
		*/
    }

    pthread_exit((void *) temp_number_in_circle);
}


float getPI()
{
    //int times = number_of_CPU_cores / 5 + 1;
    //vector<long long> number_in_circles;
    long long int number_in_circle = 0;
    part_number_of_tosses = number_of_tosses / number_of_CPU_cores / 8;
	if(number_of_CPU_cores > 3)  part_number_of_tosses *= 1.25;
	else if(number_of_CPU_cores > 4)  part_number_of_tosses *= 5;

    pthread_t thread_handles[32];
    for (int thread = 0; thread < number_of_CPU_cores; thread++)
        pthread_create(&thread_handles[thread], NULL, getNumberInCircle, NULL);

    void *returnValue;
    for (int thread = 0; thread < number_of_CPU_cores; thread++)
    {
        pthread_join(thread_handles[thread], &returnValue);
        number_in_circle += (long long int)returnValue;
        //number_in_circles.push_back((long long int)returnValue);
    }

    //int n = number_in_circles.size()/2;
    //nth_element(number_in_circles.begin(), number_in_circles.begin() + n, number_in_circles.end());
    //long long int m = number_in_circles[n];
    //vector<long long int>::iterator itOfMedian = std::find(number_in_circles.begin(), number_in_circles.end(), m);
    //n = itOfMedian - number_in_circles.begin();
    //for(int i = 0; i < times; i ++)
    //    number_in_circles.push_back(number_in_circles[n]);
    //long long int number_in_circle = accumulate(number_in_circles.begin(), number_in_circles.end(), 0);

    float pi_estimate = 4.0 * number_in_circle / (part_number_of_tosses * (number_of_CPU_cores));// + times));
    
    return pi_estimate;
}


int main(int argc, char *argv[])
{
    if(argc == 3)
    {
        number_of_CPU_cores =  strtol(argv[1], NULL, 10);
        if(number_of_CPU_cores < 1)
            number_of_CPU_cores = 1;
        else if(number_of_CPU_cores > 32)
        {
            number_of_CPU_cores = 32;
            cout << "number_of_CPU_cores 不得大於 32，已將 number_of_CPU_cores 設為 32。" << endl;
        }

        number_of_tosses =  strtol(argv[2], NULL, 10);
        if(number_of_tosses < 100000000)
        {
            number_of_tosses = 100000000;
            cout << "number_of_tosses 不得小於 1e8，已將 number_of_tosses 設為 1e8。" << endl;
        }
    }
    else if(argc != 1)
    {
        cout << "參數數量錯誤，請輸入 the number of CPU cores(int) 和 the number of tosses(long long int)。" << endl;
        return 1;
    }

    float ans = getPI();

    cout << setprecision(8) << ans << endl;

    return 0;
}

