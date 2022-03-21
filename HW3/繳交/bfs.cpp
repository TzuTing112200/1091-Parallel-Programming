#include "bfs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstddef>
#include <omp.h>
#include <vector>
using namespace std;
//#include <ctime>

#include "../common/CycleTimer.h"
#include "../common/graph.h"

#define ROOT_NODE_ID 0
#define NOT_VISITED_MARKER -1

void vertex_set_clear(vertex_set *list)
{
    list->count = 0;
}

void vertex_set_init(vertex_set *list, int count)
{
    list->max_vertices = count;
    list->vertices = (int *)malloc(sizeof(int) * list->max_vertices);
    vertex_set_clear(list);
}

// Take one step of "top-down" BFS.  For each vertex on the frontier,
// follow all outgoing edges, and add all neighboring vertices to the
// new_frontier.
void top_down_step(
    Graph g,
    vertex_set *frontier,
    vertex_set *new_frontier,
    int *distances)
{
	//int count = 0;
	//int current, next;
	#pragma omp parallel //firstprivate(count) private(current, next)
	{
		vector<int> temp;
		temp.reserve(g->num_nodes);
		
		#pragma omp for schedule(static, 128)
		for (int i = 0; i < frontier->count; i++)
		{
			int node = frontier->vertices[i];

			int start_edge = g->outgoing_starts[node];
			int end_edge = (node == g->num_nodes - 1)
							   ? g->num_edges
							   : g->outgoing_starts[node + 1];

			// attempt to add all neighbors to the new frontier
			for (int neighbor = start_edge; neighbor < end_edge; neighbor++)
			{
				int outgoing = g->outgoing_edges[neighbor];

				//if (__sync_bool_compare_and_swap(&distances[outgoing], NOT_VISITED_MARKER, distances[node] + 1))
				if (distances[outgoing] == NOT_VISITED_MARKER)
				{
					// int index;
					// #pragma omp critical
						// index = new_frontier->count++;
					// new_frontier->vertices[index] = outgoing;
					
					// do
					// {
						// current = count;
						// next = current + 1;
					// } while (!__sync_bool_compare_and_swap(&count, current, next));
					
					distances[outgoing] = distances[node] + 1;
					temp.push_back(outgoing);
				}
			}
		}
		
		#pragma omp critical
		{
			// int index = new_frontier->count;
			// for (int i = 0; i < temp.size(); i++)
				// new_frontier->vertices[index + i] = temp[i];
			copy(temp.begin(), temp.end(), new_frontier->vertices + new_frontier->count);
            new_frontier->count += temp.size();
		}
		
		// #pragma omp for
		// for (int i = 0; i < temp.size(); i++)
			// new_frontier->vertices[oldcount + i] = temp[i];
	}
}

// Implements top-down BFS.
//
// Result of execution is that, for each node in the graph, the
// distance to the root is stored in sol.distances.
void bfs_top_down(Graph graph, solution *sol)
{

    vertex_set list1;
    vertex_set list2;
    vertex_set_init(&list1, graph->num_nodes);
    vertex_set_init(&list2, graph->num_nodes);

    vertex_set *frontier = &list1;
    vertex_set *new_frontier = &list2;

    // initialize all nodes to NOT_VISITED
    #pragma omp parallel for
    for (int i = 0; i < graph->num_nodes; i++)
        sol->distances[i] = NOT_VISITED_MARKER;

    // setup frontier with the root node
    frontier->vertices[frontier->count++] = ROOT_NODE_ID;
    sol->distances[ROOT_NODE_ID] = 0;

    while (frontier->count != 0)
    {

#ifdef VERBOSE
        double start_time = CycleTimer::currentSeconds();
#endif

		vertex_set_clear(new_frontier);
		
        top_down_step(graph, frontier, new_frontier, sol->distances);

#ifdef VERBOSE
        double end_time = CycleTimer::currentSeconds();
        printf("frontier=%-10d %.4f sec\n", frontier->count, end_time - start_time);
#endif

        // swap pointers
        vertex_set *tmp = frontier;
        frontier = new_frontier;
        new_frontier = tmp;
    }
}

/*
bool bottom_up_step(Graph g, int* distances, int index)
{
	const int next = index + 1;
	bool result = false;
	#pragma omp parallel
	{
		bool r = false;
		
		#pragma omp for schedule(dynamic, 512)
		for (int v = 0; v < g->num_nodes; v++)
		{
			if (distances[v] == NOT_VISITED_MARKER)
			{
				const Vertex* start = incoming_begin(g, v);
				const Vertex* end = incoming_end(g, v);
				
				for (const Vertex *u = start; u != end; u++)
				{
					if (distances[(*u)] == index)
					{
						distances[v] = next;
						r = true;
						break;
					}
				}
			}
		}
		
		if (!result && r)
			result = r;
	}
    return result;
}
//*/

bool bottom_up_step(Graph g, vertex_set *frontier, int* distances, int index)
{
	const int next = index + 1;
	bool result = false;
	#pragma omp parallel
	{
		bool r = false;
		vector<int> temp;
		temp.reserve(g->num_nodes);
		
		//for(each vertex v in graph)
		#pragma omp for schedule(dynamic, 512)
		for (int v = 0; v < g->num_nodes; v++)
		{
			//if(v has not been visited  
			if (distances[v] == NOT_VISITED_MARKER)
			{
				const Vertex* start = incoming_begin(g, v);
				const Vertex* end = incoming_end(g, v);
				
				for (const Vertex *u = start; u != end; u++)
				{
					//&& v shares an incoming edge with a vertex u on the frontier)
					if (distances[(*u)] == index)
					{
						//add vertex v to frontier;
						distances[v] = next;
						temp.push_back(v);
						
						r = true;
						break;
					}
				}
			}
		}
		
		#pragma omp critical
		{
			copy(temp.begin(), temp.end(), frontier->vertices + frontier->count);
            frontier->count += temp.size();
		}
		
		if (!result && r)
			result = r;
	}
    return result;
}

void bfs_bottom_up(Graph graph, solution *sol)
{
    // For PP students:
    //
    // You will need to implement the "bottom up" BFS here as
    // described in the handout.
    //
    // As a result of your code's execution, sol.distances should be
    // correctly populated for all nodes in the graph.
    //
    // As was done in the top-down case, you may wish to organize your
    // code by creating subroutine bottom_up_step() that is called in
    // each step of the BFS process.
	
    vertex_set list1;
    vertex_set_init(&list1, graph->num_nodes);

    vertex_set *frontier = &list1;
	
    #pragma omp parallel for
    for (int i = 0; i < graph->num_nodes; i++)
        sol->distances[i] = NOT_VISITED_MARKER;

    frontier->vertices[frontier->count++] = ROOT_NODE_ID;
    sol->distances[ROOT_NODE_ID] = 0;
	
	int index = -1;

    do
    {
		index++;
		vertex_set_clear(frontier);
    } while(bottom_up_step(graph, frontier, sol->distances, index));
}

void bfs_hybrid(Graph graph, solution *sol)
{
    // For PP students:
    //
    // You will need to implement the "hybrid" BFS here as
    // described in the handout.

    vertex_set list1;
    vertex_set list2;
    vertex_set_init(&list1, graph->num_nodes);
    vertex_set_init(&list2, graph->num_nodes);

    vertex_set *frontier = &list1;
    vertex_set *new_frontier = &list2;

    #pragma omp parallel for
    for (int i = 0; i < graph->num_nodes; i++)
        sol->distances[i] = NOT_VISITED_MARKER;

    frontier->vertices[frontier->count++] = ROOT_NODE_ID;
    sol->distances[ROOT_NODE_ID] = 0;
	
	int index = -1;
	
	do
    {
		while(frontier->count > graph->num_nodes / 8)
		{
		    index++;
			vertex_set_clear(frontier);
			if(!bottom_up_step(graph, frontier, sol->distances, index))
				return;
		}
		
		index++;
		vertex_set_clear(new_frontier);
		top_down_step(graph, frontier, new_frontier, sol->distances);

        vertex_set *tmp = frontier;
        frontier = new_frontier;
        new_frontier = tmp;
    } while (frontier->count != 0);	
		
}
