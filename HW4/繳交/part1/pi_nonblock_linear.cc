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
	double x, y = (double) rand_r(&seed) / RAND_MAX, distance_squared;
	double pi_estimate[world_size - 1];

    if (world_rank > 0)
    {
        // TODO: MPI workers
		for(unsigned int toss = 0; toss < the_tosses; ++toss)
		{
			x = y;
			y = (double) rand_r(&seed) / RAND_MAX;

			distance_squared = x * x + y * y;
			if(distance_squared <= 1)
				number_in_circle++;
		}
	    pi_result = 4 * number_in_circle / ((double) tosses);
		MPI_Send(&pi_result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    else if (world_rank == 0)
    {
        // TODO: non-blocking MPI communication.
        // Use MPI_Irecv, MPI_Wait or MPI_Waitall.
        MPI_Request requests[world_size - 1];
		for (int source = 1; source < world_size; source++ ) 
		{
			MPI_Irecv( &pi_estimate[source - 1], 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &requests[source - 1] );
		}
		
		for(unsigned int toss = 0; toss < the_tosses; ++toss)
		{
			x = y;
			y = (double) rand_r(&seed) / RAND_MAX;

			distance_squared = x * x + y * y;
			if(distance_squared <= 1)
				number_in_circle++;
		}
	    pi_result = 4 * number_in_circle / ((double) tosses);

        MPI_Waitall(world_size - 1, requests, MPI_STATUS_IGNORE);
    }

    if (world_rank == 0)
    {
        // TODO: PI result
		for (int source = 0; source < world_size - 1; source++ ) 
		{
			pi_result += pi_estimate[source];
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
