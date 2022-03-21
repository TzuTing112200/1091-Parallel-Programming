#include <iostream>
#include <iomanip>
#include <cstdlib>
//#include <ctime>
//#include <algorithm>
//#include <math.h>
#include <pthread.h>
#include <immintrin.h>
#include <limits>
//#include <vector>
//#include <numeric>
using namespace std;


struct xorshift128plus_key_s {
    __m256i part1;
    __m256i part2;
};

typedef struct xorshift128plus_key_s xorshift128plus_key_t;


static void xorshift128plus_onkeys(uint64_t * ps0, uint64_t * ps1) {
	uint64_t s1 = *ps0;
	const uint64_t s0 = *ps1;
	*ps0 = s0;
	s1 ^= s1 << 23; // a
	*ps1 = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5); // b, c
}

/* used by avx_xorshift128plus_init */
static void xorshift128plus_jump_onkeys(uint64_t in1, uint64_t in2,
		uint64_t * output1, uint64_t * output2) {
	/* todo: streamline */
	static const uint64_t JUMP[] = { 0x8a5cd789635d2dff, 0x121fd2155c472f96 };
	uint64_t s0 = 0;
	uint64_t s1 = 0;
	for (unsigned int i = 0; i < sizeof(JUMP) / sizeof(*JUMP); i++)
		for (int b = 0; b < 64; b++) {
			if (JUMP[i] & 1ULL << b) {
				s0 ^= in1;
				s1 ^= in2;
			}
			xorshift128plus_onkeys(&in1, &in2);
		}
	output1[0] = s0;
	output2[0] = s1;
}

void avx_xorshift128plus_init(uint64_t key1, uint64_t key2,
		xorshift128plus_key_t *key) {
	uint64_t S0[4];
	uint64_t S1[4];
	S0[0] = key1;
	S1[0] = key2;
	xorshift128plus_jump_onkeys(*S0, *S1, S0 + 1, S1 + 1);
	xorshift128plus_jump_onkeys(*(S0 + 1), *(S1 + 1), S0 + 2, S1 + 2);
	xorshift128plus_jump_onkeys(*(S0 + 2), *(S1 + 2), S0 + 3, S1 + 3);
	key->part1 = _mm256_loadu_si256((const __m256i *) S0);
	key->part2 = _mm256_loadu_si256((const __m256i *) S1);
}

/*
 Return a 256-bit random "number"
 */
__m256 avx_xorshift128plus(xorshift128plus_key_t *key) {
	__m256i s1 = key->part1;
	const __m256i s0 = key->part2;
	key->part1 = key->part2;
	s1 = _mm256_xor_si256(key->part2, _mm256_slli_epi64(key->part2, 23));
	key->part2 = _mm256_xor_si256(
			_mm256_xor_si256(_mm256_xor_si256(s1, s0),
					_mm256_srli_epi64(s1, 18)), _mm256_srli_epi64(s0, 5));
	return _mm256_cvtepi32_ps(_mm256_add_epi64(key->part2, s0));
}


int number_of_CPU_cores = 4;
long long int number_of_tosses = 100000000;
long long int part_number_of_tosses = 0;


// Estimate PI value
void *getNumberInCircle(void *val)
{
	xorshift128plus_key_t mykey;
	avx_xorshift128plus_init(pthread_self(),1234,&mykey);
	
    //int rand_num;
    //float x, y;
	__m256 _nums_1, _nums_2, _result,
	      _max = _mm256_set1_ps((float) 0xffffffff / 2), 
	       _one = _mm256_set1_ps(1.0);
    //__m256 _x;
    //__m256 _y;
    
    /*
    for(int i = 0; i < 8; i++)
    {
        _x[i] = (double)(rand_num_1 & 0xffff) / 0xffff;
        rand_num_1 = rand_num_1 >> 1;
        _y[i] = (double)(rand_num_1 & 0xffff) / 0xffff;
        rand_num_1 = rand_num_1 >> 1;
    }
    */

    long long int temp_number_in_circle = 0;
	int times = part_number_of_tosses / 8000000;

    for(int all = 0; all < times; all++)
    {
		_result = _mm256_set1_ps(0.0);
		for(int toss = 0; toss < 1000000; toss++)
		{
			_nums_1 = _mm256_div_ps(avx_xorshift128plus(&mykey), _max);
			_nums_1 = _mm256_mul_ps(_nums_1, _nums_1);
			_nums_2 = _mm256_div_ps(avx_xorshift128plus(&mykey), _max);
			_nums_2 = _mm256_mul_ps(_nums_2, _nums_2);
			
			_nums_1 = _mm256_add_ps(_nums_1, _nums_2);
			
			_nums_1 = _mm256_cmp_ps(_nums_1, _one, 2);
			
			_nums_1 = _mm256_and_ps(_one, _nums_1);
			
			_result = _mm256_add_ps(_result, _nums_1);
			
			/*
			rand_num = rand_r(&seed);
			x = (float)(rand_num & 0xffff) / 0xffff;
			rand_num = rand_r(&seed);
			y = (float)(rand_num & 0xffff) / 0xffff;
			*/
			
			//_x = _mm256_mul_ps(_x, _x);
			//_y = _mm256_mul_ps(_y, _y);

			//_x = _mm256_add_ps(_x, _y);
			
			//if((x*x + y*y) < 1.0) temp_number_in_circle++;
			
			/*
			for(int i = 0; i < 8; i ++)
			{
				if(_x[i] < 1)
					temp_number_in_circle++;

				_x[i] = (float)(rand_num_1 & 0xffff) / 0xffff;
				_y[i] = (float)(rand_num_2 & 0xffff) / 0xffff;
				rand_num_1 = rand_num_1 >> 2;
				rand_num_2 = rand_num_2 >> 2;
			}
			*/
		}
		
		temp_number_in_circle += (int)_result[0];
		temp_number_in_circle += (int)_result[1];
		temp_number_in_circle += (int)_result[2];
		temp_number_in_circle += (int)_result[3];
		temp_number_in_circle += (int)_result[4];
		temp_number_in_circle += (int)_result[5];
		temp_number_in_circle += (int)_result[6];
		temp_number_in_circle += (int)_result[7];
	}
	
    pthread_exit((void *) temp_number_in_circle);
}


float getPI()
{
    //int times = number_of_CPU_cores / 5 + 1;
    //vector<long long> number_in_circles;
    long long int number_in_circle = 0;
    part_number_of_tosses = number_of_tosses / number_of_CPU_cores / 5 * 4;
    part_number_of_tosses = part_number_of_tosses / 8000000  * 8000000;
	if(part_number_of_tosses == 0)  part_number_of_tosses = 8000000;

    pthread_t *thread_handles = (pthread_t *)malloc(number_of_CPU_cores * sizeof (pthread_t));
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

    float pi_estimate = 4.0 * number_in_circle / (number_of_CPU_cores * part_number_of_tosses);// + times));
    
    return pi_estimate;
}


int main(int argc, char *argv[])
{
    if(argc == 3)
    {
        number_of_CPU_cores =  strtoll(argv[1], NULL, 10);
        if(number_of_CPU_cores < 1)
            number_of_CPU_cores = 1;

        number_of_tosses =  strtoll(argv[2], NULL, 10);
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

