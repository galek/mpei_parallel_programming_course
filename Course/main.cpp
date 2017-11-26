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

	TimeData time;
	time.StartTime();
	{
		compute.MergeSort();
	}
	time.EndTime();

	// Debug
	PrintfMatrixData(compute.GetMatrixDataCopy());

	if (compute.rank_proc == 0)
	{
		printf("\n FINISH! \n");
		printf("\n\nTime= %f\n", time.GetDelta());
	}

	/********** Finalize MPI **********/
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

	system("pause");
}