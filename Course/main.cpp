#include "Pch.h"

//http://www2.sscc.ru/Publikacii/Primery_Prll/Primery.htm

//------------------------------------------------------------------------------

static void RunMergeSort(Compute& compute)
{
	printf("\n MERGESORT: \n");
	TimeData time;
	time.StartTime();
	{
		compute.MergeSort();
	}
	time.EndTime();

	if (compute.rank_proc == 0)
	{
		printf("\n FINISH! \n");
		printf("\n\nTime= %f\n", time.GetDelta());
	}
}


static void RunQSort(Compute& compute)
{
	printf("\n QSORT: \n");
	TimeData time;
	time.StartTime();
	{
		compute.QSort();
	}
	time.EndTime();

	if (compute.rank_proc == 0)
	{
		printf("\n FINISH! \n");
		printf("\n\nTime= %f\n", time.GetDelta());
	}
}


int main(int argc, char* argv[])
{
	setvbuf(stdout, 0, _IONBF, 0);

	MPI_Init(&argc, &argv);

	Compute compute;

	MPI_Comm_rank(MPI_COMM_WORLD, &compute.rank_proc);
	MPI_Comm_size(MPI_COMM_WORLD, &compute.g_NumProc);

	compute.Init();

	// Debug
	//PrintfMatrixData(compute.GetMatrixDataCopy());

	RunMergeSort(compute);
	RunQSort(compute);

	// Debug
	//PrintfMatrixDataResult(compute.GetMatrixDataCopy());

	/********** Finalize MPI **********/
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

	system("pause");
}