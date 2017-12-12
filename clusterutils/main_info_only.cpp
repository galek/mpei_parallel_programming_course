#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <math.h>
#include <mpi.h>

#include <string>
#include <array>   
#include <algorithm>

#include <cstdlib>
#include <iostream>
#include <ctime>

static int main_info(int argc, char* argv[])
{
	setvbuf(stdout, 0, _IONBF, 0);

	MPI_Init(&argc, &argv);

	int rank_proc = 0;
	int g_NumProc = 0;
	int size=0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank_proc);
	MPI_Comm_size(MPI_COMM_WORLD, &g_NumProc);

	// Рассылка size
	MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	// Завершили операцию
	MPI_Barrier(MPI_COMM_WORLD);

	if (rank_proc == 0) {
		printf("\n\n CLUSTER INFO: rank_proc:%i  g_NumProc:%i\n", rank_proc, g_NumProc);
	}

	/********** Finalize MPI **********/
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
}


int main(int argc, char* argv[])
{
	main_info(argc, argv);
}