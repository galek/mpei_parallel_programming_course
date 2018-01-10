#include "Pch.h"

#include <fstream>
#include <sys/stat.h>

//http://www2.sscc.ru/Publikacii/Primery_Prll/Primery.htm

bool generateFile = true;
bool saveFile = true;
uint32_t countOfEl = 50000000; // 50 000 000

//------------------------------------------------------------------------------

void MergeSortImpl(int world_rank, int world_size);
void QuickSortImpl(int world_rank, int world_size);


bool FileExist()
{
	struct stat buffer;
	return (stat("numbers.bin", &buffer) == 0);
}

static void RunMergeSort(Compute& compute)
{
	//printf("\n MERGESORT: \n");
	TimeData time;
	time.StartTime();
	{
		MergeSortImpl(compute.rank_proc, compute.g_NumProc);
	}
	time.EndTime();

	if (compute.rank_proc == 0)
	{
		//printf("\n FINISH! \n");
		printf("\n\n Merge sort = %f\n", time.GetDelta());
	}
}


static void RunQSort(Compute& compute)
{
	//printf("\n MERGESORT: \n");
	TimeData time;
	time.StartTime();
	{
		QuickSortImpl(compute.rank_proc, compute.g_NumProc);
	}
	time.EndTime();

	if (compute.rank_proc == 0)
	{
		//printf("\n FINISH! \n");
		printf("\n\n quick sort = %f\n", time.GetDelta());
	}
}


static int main_info(int argc, char* argv[])
{
	setvbuf(stdout, 0, _IONBF, 0);

	MPI_Init(&argc, &argv);

	Compute compute;

	MPI_Comm_rank(MPI_COMM_WORLD, &compute.rank_proc);
	MPI_Comm_size(MPI_COMM_WORLD, &compute.g_NumProc);

	compute.Init();

	if (compute.rank_proc == 0) {
		printf("\n\n CLUSTER INFO: rank_proc:%i  g_NumProc:%i\n", compute.rank_proc, compute.g_NumProc);
	}

	/********** Finalize MPI **********/
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}

#include "FileOps.h"

static int main_run(int argc, char* argv[])
{
	setvbuf(stdout, 0, _IONBF, 0);

	MPI_Init(&argc, &argv);

	Compute compute;

	MPI_Comm_rank(MPI_COMM_WORLD, &compute.rank_proc);
	MPI_Comm_size(MPI_COMM_WORLD, &compute.g_NumProc);

	compute.Init();

	if (compute.rank_proc == 0) {
		printf("\n\n CLUSTER INFO: rank_proc:%i  g_NumProc:%i\n", compute.rank_proc, compute.g_NumProc);
	}


#if 0 // Generation
	MatrixData data;

	if (generateFile && FileExist() == false)
	{
		MatrixData _p;
		_p.m_matrixArraySize = countOfEl;
		_p.m_Matrix = new PFDV[_p.m_matrixArraySize];

		srand(time(NULL));
		for (auto c = 0; c < countOfEl; c++) {
			_p.m_Matrix[c] = rand();
			//printf("%f ", original_array[c]);
		}

		data = _p;

		if (saveFile)
			SaveFile(data);
	}

#else
	RunMergeSort(compute);
	RunQSort(compute);
#endif

	// Debug
	//PrintfMatrixDataResult(compute.GetMatrixDataCopy());

	/********** Finalize MPI **********/
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();


	//system("pause");
	return 0;
}


int main(int argc, char* argv[])
{
	main_run(argc, argv);
}