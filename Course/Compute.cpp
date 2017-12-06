#include "Pch.h"

#include <iterator>
#include <algorithm>

void Compute::Init()
{
	if (rank_proc == 0)
	{
#if 0
		__GenerateMatrix();
#else
		// TODO: USED ONLY FOR TESTS
		__GenerateMatrix_Moc();
#endif
		CopyRawMatrixToResult();
		printf("\n Timer started \n");
	}

	// Рассылка size
	MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	// Завершили операцию
	MPI_Barrier(MPI_COMM_WORLD);
}

void Compute::__GenerateMatrix()
{
	// Generate matrixData
	auto data = GenerateMatrixData(500, g_NumProc);
	SetMatrixData(data);
}

void Compute::__GenerateMatrix_Moc()
{
	// Generate matrixData
	auto data = GenerateMatrixData(11, 1);

	data.m_Matrix[0] = 5;
	data.m_Matrix[1] = 4;
	data.m_Matrix[2] = 3;
	data.m_Matrix[3] = 2;
	data.m_Matrix[4] = 1;
	data.m_Matrix[5] = 0;
	data.m_Matrix[6] = -1;
	data.m_Matrix[7] = -2;
	data.m_Matrix[8] = -3;
	data.m_Matrix[9] = -4;
	data.m_Matrix[10] = -5;

	SetMatrixData(data);
}

void Compute::CopyRawMatrixToResult()
{
	//void * memcpy ( void * destination, const void * source, size_t num );
	// Another size variants not works
	memcpy(_mData.m_Result, _mData.m_Matrix, (sizeof(PFDV)* _mData.m_ElementsInLine));

}

void Compute::QSort()
{
	CopyRawMatrixToResult();
	// sort using the default operator<
	std::sort(_mData.m_Matrix, _mData.m_Matrix + 11);
}

void Compute::MergeSort()
{
	CopyRawMatrixToResult();
	/********** Divide the array in equal-sized chunks **********/
	auto size = _mData.m_ElementsInLine / g_NumProc;

	/********** Send each subarray to each process **********/
	PFDV *sub_array = new PFDV[size];

	MPI_Scatter(_mData.m_Result, size, MPI_FLOAT, sub_array, size, MPI_FLOAT, 0, MPI_COMM_WORLD);

	//
	/********** Perform the mergesort on each process **********/
	PFDV *tmp_array = new PFDV[size];
	mergeSort(sub_array, tmp_array, 0, (size - 1));

	/********** Gather the sorted subarrays into one **********/
	PFDV *sorted = nullptr;

	if (this->rank_proc == 0) {
		sorted = new PFDV[_mData.m_ElementsInLine];
	}

	MPI_Gather(sub_array, size, MPI_FLOAT, sorted, size, MPI_FLOAT, 0, MPI_COMM_WORLD);

	/********** Make the final mergeSort call **********/
	if (this->rank_proc == 0) {

		PFDV *other_array = new PFDV[_mData.m_ElementsInLine];

		mergeSort(sorted, other_array, 0, (_mData.m_ElementsInLine - 1));

		/********** Display the sorted array **********/
		printf("This is the sorted array: ");

		/********** Clean up root **********/
		auto older = _mData.m_Result;

		_mData.m_Result = sorted;

		delete[] older;
		delete[] other_array;

	}
}

void TimeData::StartTime()
{
	startTime = MPI_Wtime();
}

void TimeData::EndTime()
{
	time_end = MPI_Wtime();
}
