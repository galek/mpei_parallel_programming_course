#include "Pch.h"

//http://www2.sscc.ru/Publikacii/Primery_Prll/Primery.htm

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	setvbuf(stdout, 0, _IONBF, 0);

	MPI_Init(&argc, &argv);

	Compute compute;

	MPI_Comm_rank(MPI_COMM_WORLD, &compute.rank_proc);
	MPI_Comm_size(MPI_COMM_WORLD, &compute.g_NumProc);

	compute.Init();

	// Debug
	PrintfMatrixData(compute.GetMatrixDataCopy());
	//compute.QSort();
	compute.MergeSort();

	//MPI_Barrier(MPI_COMM_WORLD);

	//compute.startTime = MPI_Wtime();

	//compute.RunGauss();

	//MPI_Barrier(MPI_COMM_WORLD); // All threads will wait here until you give thread 0 an input

	//compute.time_end = MPI_Wtime();
	//compute.DeltaTime = (compute.time_end - compute.startTime);

	//if (compute.rank_proc == 0)
	//{
	//	if (compute.status == 1)
	//		printf("\nRoots: \n");
	//	if (compute.status == 0) printf("\nNo roots.\n");
	//	if (compute.status == 2)
	//		printf("\nContinum. Solution: \n\n");

	//	printf("\n\nTime= %f\n", compute.DeltaTime);
	//}

	// Debug
	PrintfMatrixData(compute.GetMatrixDataCopy());

	if (compute.rank_proc == 0)
	{
		printf("\n FINISH! \n");
	}

	/********** Finalize MPI **********/
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

	system("pause");
}