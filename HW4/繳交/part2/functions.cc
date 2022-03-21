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
            printf("%d ", result[index++]);
        }while(index < ll);
		printf("%d\n", result[index++]);
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

// Remember to release your allocated memory
void destruct_matrices(int *a_mat, int *b_mat)
{
    free(a_mat);
    free(b_mat);
}
