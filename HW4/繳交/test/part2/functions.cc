#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int world_rank, world_size;

// Read size of matrix_a and matrix_b (n, m, l) and whole data of matrixes from stdin
//
// n_ptr:     pointer to n
// m_ptr:     pointer to m
// l_ptr:     pointer to l
// a_mat_ptr: pointer to matrix a (a should be a continuous memory space for placing n * m elements of int)
// b_mat_ptr: pointer to matrix b (b should be a continuous memory space for placing m * l elements of int)
void construct_matrices(int *n_ptr, int *m_ptr, int *l_ptr,
                        int **a_mat_ptr, int **b_mat_ptr)
{
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int a_size, b_size;

    if(world_rank == 0)
    {
        scanf("%d %d %d", n_ptr, m_ptr, l_ptr);

        a_size = (*n_ptr) * (*m_ptr);
        b_size = (*m_ptr) * (*l_ptr);

        *a_mat_ptr = (int *)malloc(a_size * sizeof(int));
        *b_mat_ptr = (int *)malloc(b_size * sizeof(int));

        int i, j;

        for(i = 0; i < a_size; i++)
            scanf("%d", *a_mat_ptr + i);

        for(i = 0; i < (*m_ptr); i++)
            for(j = i; j < b_size; j += (*m_ptr))
                scanf("%d", *b_mat_ptr + j);

        // for(int i = 0; i < (*m_ptr); i++)
            // for(int j = 0; j < (*l_ptr); j++)
                // scanf("%d", *b_mat_ptr + j * (*m_ptr) + i);
    }
	
	// if(world_size == 5 && world_rank < 2)
	// {
		// if(world_rank == 0)
		// {
			// MPI_Send(n_ptr, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			// MPI_Send(m_ptr, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
			// MPI_Send(l_ptr, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);

			// MPI_Send(&a_size, 1, MPI_INT, 1, 3, MPI_COMM_WORLD);
			// MPI_Send(&b_size, 1, MPI_INT, 1, 4, MPI_COMM_WORLD);

			// MPI_Send(*a_mat_ptr, a_size, MPI_INT, 1, 5, MPI_COMM_WORLD);
			// MPI_Send(*b_mat_ptr, b_size, MPI_INT, 1, 6, MPI_COMM_WORLD);
		// }
		// else if(world_rank == 1)
		// {
			// MPI_Recv(n_ptr, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// MPI_Recv(m_ptr, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// MPI_Recv(l_ptr, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			// MPI_Recv(&a_size, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// MPI_Recv(&b_size, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			
			// *a_mat_ptr = (int *)malloc(a_size * sizeof(int));
			// *b_mat_ptr = (int *)malloc(b_size * sizeof(int));

			// MPI_Recv(*a_mat_ptr, a_size, MPI_INT, 0, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// MPI_Recv(*b_mat_ptr, b_size, MPI_INT, 0, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// }
	// }
	
	if(world_size == 5)
	{
		MPI_Bcast(n_ptr, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(m_ptr, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(l_ptr, 1, MPI_INT, 0, MPI_COMM_WORLD);

		MPI_Bcast(&a_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&b_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

		if(world_rank != 0)
		{
			*a_mat_ptr = (int *)malloc(a_size * sizeof(int));
			*b_mat_ptr = (int *)malloc(b_size * sizeof(int));
		}

		MPI_Bcast(*a_mat_ptr, a_size, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(*b_mat_ptr, b_size, MPI_INT, 0, MPI_COMM_WORLD);
	}
}

// Just matrix multiplication (your should output the result in this function)
//
// n:     row number of matrix a
// m:     col number of matrix a / row number of matrix b
// l:     col number of matrix b
// a_mat: a continuous memory placing n * m elements of int
// b_mat: a continuous memory placing m * l elements of int
void matrix_multiply(const int n, const int m, const int l,
                     const int *a_mat, const int *b_mat)
{
    if(world_size == 4 && world_rank == 0)
    {
        int index, rbase, base, row, rows, rem, size, ll, sum;
        rows = 0;
        //rem = n % world_size;
        size = l * n;
        ll = l - 1;
        
        int *result;//, *all_result;
        const int *a_temp, *b_temp;
        result = (int *)malloc(size * sizeof(int));
        //all_result = (int *)malloc(size * sizeof(int));

        //if(world_rank == 0)
            //row = 0;
        //else if(world_rank <= rem)
            //row = world_rank * (++rows) - 1;
        //else
            //row = rem + world_rank * rows;

        int r, i, j;

        base = 0;
        rbase = 0;
        for(r = 0; r < n; r++, base += m)
        {
            index = 0;
            a_temp = &a_mat[base];
            for(i = 0; i < l; i++, index += m)
            {
                sum = 0;
                b_temp = &b_mat[index];
                for(j = 0; j < m; j++)
                    sum += a_temp[j] * b_temp[j];
                result[rbase++] = sum;
            }
        }

        // if(world_rank == 0)
        // {
            // index = 0;
            // do
            // {
                // printf("%d ", result[index++]);
            // }while(index < ll);
            // printf("%d\n", result[index++]);
        // }

        //MPI_Reduce(result, all_result, size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        index = 0;
        do
        {
            if(index % l == ll)
                printf("%d\n", result[index++]);
            else
                printf("%d ", result[index++]);
        }while(index < size);
        
        free(result);
    }
	// else if(world_size == 5 && world_rank < 2)
	// {
		// int index, rbase, base, rows, size, ll, sum;
		// rows = n / 2;

		// if(world_rank == 0)
			// base = 0;
		// else
			// base = rows * m;

		// if(n % 2 == 1 && world_rank == 1)
			// rows++;
		
		// size = l * rows;
		
		// ll = l - 1;
		
		// const int *a_temp, *b_temp;
		// int *result;//, *all_result;
		// result = (int *)malloc(size * sizeof(int));
		// all_result = (int *)malloc(size * sizeof(int));
		
		// int r, i, j;
		
		// rbase = 0;
		// for(r = 0; r < rows; r++, base += m)
		// {
			// index = 0;
			// a_temp = &a_mat[base];
			// for(i = 0; i < l; i++, index += m)
			// {
				// sum = 0;
				// b_temp = &b_mat[index];
				// for(j = 0; j < m; j++)
					// sum += a_temp[j] * b_temp[j];
				// result[rbase++] = sum;
			// }
		// }
		
		// if(world_rank == 0)
		// {
			// index = 0;
			// do
			// {
				// if(index % l == ll)
					// printf("%d\n", result[index++]);
				// else
					// printf("%d ", result[index++]);
			// }while(index < size);
			
			// free(result);
			// MPI_Send(&sum, 1, MPI_INT, 1, 10, MPI_COMM_WORLD);
		// }
	
		// if(world_rank == 1)
		// {
			// MPI_Recv(&sum, 1, MPI_INT, 0, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// index = 0;
			// do
			// {
				// if(index % l == ll)
					// printf("%d\n", result[index++]);
				// else
					// printf("%d ", result[index++]);
			// }while(index < size);
			
			// free(result);
		// }
	// }
	else if(world_size == 5)
	{
		int index, rbase, base, row, rows, rem, size, ll, sum;
		rows = n / world_size;
		rem = n % world_size;
		size = l * n;
		ll = l - 1;
		
		int *result, *all_result;
		const int *a_temp, *b_temp;
		result = (int *)malloc(size * sizeof(int));
		all_result = (int *)malloc(size * sizeof(int));

		if(world_rank == 0)
			row = 0;
		else if(world_rank <= rem)
			row = world_rank * (++rows) - 1;
		else
			row = rem + world_rank * rows;

		int r, i, j;

		base = row * m;
		rbase = row * l;
		for(r = 0; r < rows; r++, base += m)
		{
			index = 0;
			a_temp = &a_mat[base];
			for(i = 0; i < l; i++, index += m)
			{
				sum = 0;
				b_temp = &b_mat[index];
				for(j = 0; j < m; j++)
					sum += a_temp[j] * b_temp[j];
				result[rbase++] = sum;
			}
		}

		if(world_rank == 0)
		{
			index = 0;
			do
			{
				if(index % l == ll)
					printf("%d\n", result[index++]);
				else
					printf("%d ", result[index++]);
			}while(index < rbase);
		}

		MPI_Reduce(result, all_result, size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

		if(world_rank == 0)
		{
			do
			{
				if(index % l == ll)
					printf("%d\n", all_result[index++]);
				else
					printf("%d ", all_result[index++]);
			}while(index < size);
		}

		free(result);
		free(all_result);
	}

    //free(all_result);
}

// Remember to release your allocated memory
void destruct_matrices(int *a_mat, int *b_mat)
{
    if(world_size == 4 && world_rank == 0)
    {
        free(a_mat);
        free(b_mat);
    }
    else if(world_size == 5)
    {
        free(a_mat);
        free(b_mat);
    }
}
