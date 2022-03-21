#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---

    // TODO: MPI init
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	long long int the_tosses = tosses / world_size;
	unsigned int number_in_circle = 0;
	unsigned int seed = time(NULL) ^ world_rank ^ 309552017;
	double x, y = (double) rand_r(&seed) / RAND_MAX, pi_estimate, distance_squared;
    double *recv_buf;
	
	if (world_rank == 0)
    {
		recv_buf = (double *) malloc(world_size * sizeof(double));
	}

    // TODO: use MPI_Gather
	for(unsigned int toss = 0; toss < the_tosses; ++toss)
	{
		x = y;
		y = (double) rand_r(&seed) / RAND_MAX;

		distance_squared = x * x + y * y;
		if(distance_squared <= 1)
			number_in_circle++;
	}
	pi_estimate = 4 * number_in_circle / ((double) tosses);
	
	MPI_Gather(&pi_estimate, 1, MPI_DOUBLE, recv_buf, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        // TODO: PI result
		pi_result = recv_buf[0];
		for (int source = 1; source < world_size; source++ ) 
		{
			pi_result += recv_buf[source];
		}

        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }
    
    MPI_Finalize();
    return 0;
}
