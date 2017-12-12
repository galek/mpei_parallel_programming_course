#include "Pch.h"


#include <ppl.h>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "ppl_extras.h"


MatrixData ReadFromFile();
void SaveFile(MatrixData& data);

void QuickSort(PFDV *a, PFDV *out, int l, int r)
{
	VAR_UNUSED(out);
	VAR_UNUSED(l);
	// sort using the default operator<
	std::sort(a, a + r);
}

void QuickSortImpl(int world_rank, int world_size)
{
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
	else {
		data = ReadFromFile();
	}

#if 0
	uint32_t n = data.m_matrixArraySize;

	/********** Create and populate the array **********/
	PFDV *original_array = data.m_Matrix;

	/********** Divide the array in equal-sized chunks **********/
	uint32_t size = n / world_size;

	/********** Send each subarray to each process **********/
	PFDV *sub_array = new PFDV[size];
	MPI_Scatter(original_array, size, MPI_PFDV, sub_array, size, MPI_PFDV, 0, MPI_COMM_WORLD);

	/********** Perform the QuickSort on each process **********/
	//PFDV *tmp_array = new PFDV[size];
	QuickSort(sub_array, /*tmp_array*/nullptr, 0, (size - 1));

	/********** Gather the sorted subarrays into one **********/
	PFDV *sorted = NULL;
	if (world_rank == 0) {
		sorted = new PFDV[n];
	}

	MPI_Gather(sub_array, size, MPI_PFDV, sorted, size, MPI_PFDV, 0, MPI_COMM_WORLD);

	/********** Make the final mergeSort call **********/
	if (world_rank == 0) {

		//PFDV *other_array = new PFDV[n];
		QuickSort(sorted, /*other_array*/nullptr, 0, (n - 1));

		//#if DEBUG_SORT
				/********** Display the sorted array **********/
		printf("This is the sorted array: ");
		for (auto c = 0; c < n; c++) {

			printf("%f ", sorted[c]);

		}

		printf("\n");
		printf("\n");
		//#endif // endif

				/********** Clean up root **********/
		delete[](sorted);
		//delete[](other_array);

	}

	/********** Clean up rest **********/
	delete[](original_array);
	delete[](sub_array);
	//delete[](tmp_array);
#else
	//Parallel Sort
	{
		std::vector<PFDV> v;
		v.resize(data.m_matrixArraySize);

		uint32_t i = 0;
		for (auto&it : v) {
			it = data.m_Matrix[i];
			i++;
		}

		if (world_rank == 0)
		{
			std::cout << "Parallel Sort: " << ClockFunc([&v]() { Concurrency::samples::parallel_sort(v.begin(), v.end()); }) << std::endl;
		}
	}

#endif
}