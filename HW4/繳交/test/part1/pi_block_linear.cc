#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctime>

int main(int argc, char **argv)
{
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---

    // TODO: init MPI
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	long long int the_tosses = tosses / world_size;
	unsigned int number_in_circle = 0;
	unsigned int seed = time(NULL) ^ world_rank ^ 309552017;
	double x, y = (double) rand_r(&seed) / RAND_MAX, pi_estimate, distance_squared;
	

    if (world_rank > 0)
    {
        // TODO: handle workers
		for(unsigned int toss = 0; toss < the_tosses; ++toss)
		{
			x = y;
			y = (double) rand_r(&seed) / RAND_MAX;

			distance_squared = x * x + y * y;
			if(distance_squared <= 1)
				number_in_circle++;
		}
	    pi_estimate = 4 * number_in_circle / ((double) tosses);
		MPI_Send(&pi_estimate, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    else if (world_rank == 0)
    {
        // TODO: master
		for(unsigned int toss = 0; toss < the_tosses; ++toss)
		{
			x = y;
			y = (double) rand_r(&seed) / RAND_MAX;

			distance_squared = x * x + y * y;
			if(distance_squared <= 1)
				number_in_circle++;
		}
		pi_result = 4 * number_in_circle / ((double) tosses);
    }

    if (world_rank == 0)
    {
        // TODO: process PI result
		for (int source = 1; source < world_size; source++ ) 
		{
			MPI_Recv(&pi_estimate, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			pi_result += pi_estimate;
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
