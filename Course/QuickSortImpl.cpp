#include "Pch.h"

#include <vector>
#include <algorithm>
#include <iostream>


MatrixData ReadFromFile();
void SaveFile(MatrixData& data);

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

	uint32_t n = data.m_matrixArraySize;

	/********** Create and populate the array **********/
	PFDV *original_array = data.m_Matrix;

	/********** Divide the array in equal-sized chunks **********/
	auto remainder = n % world_size;
	int *local_counts = new int[world_size];
	int *offsets = new int[world_size + 1];

	int sum = 0;
	for (int i = 0; i < world_size; i++) {
		local_counts[i] = n / world_size;
		if (remainder > 0) {
			local_counts[i] += 1;
			remainder--;
		}
		offsets[i] = sum;
		sum += local_counts[i];
	}
	offsets[world_size] = sum;



	/********** Send each subarray to each process **********/
	auto size = local_counts[world_rank];

	std::cout << "Size: " << size << std::endl;

	PFDV *sub_array = new PFDV[size];

	// сравни параметры
	MPI_Scatterv(original_array, local_counts, offsets, MPI_PFDV, sub_array, local_counts[world_rank], MPI_PFDV, 0, MPI_COMM_WORLD);


	/********** Perform the QuickSort on each process **********/
	std::vector<PFDV> v;
	v.resize(size);

	for (int i = 0; i < size; i++)
	{
		if (sub_array[i] == NULL)
			continue;
		v[i] = sub_array[i];
	}

	std::sort(v.begin(), v.end(), std::less<float>());

	/********** Gather the sorted subarrays into one **********/
	PFDV *sorted = NULL;
	if (world_rank == 0) {
		sorted = new PFDV[n];
	}

	MPI_Gatherv(sub_array, size, MPI_PFDV, sorted, local_counts, offsets, MPI_PFDV, 0, MPI_COMM_WORLD);

	/********** Make the final mergeSort call **********/
	if (world_rank == 0) {

		std::vector<PFDV> res;
		res.resize(n);

		for (int i = 0; i < n; i++) {
			if (sorted[i] == NULL)
				continue;
			res[i] = sorted[i];
		}
		std::sort(res.begin(), res.end(), std::less<float>());


		/********** Display the sorted array **********/
		/*printf("This is the sorted array: ");
		for (auto c = 0; c < n; c++) {

			printf("%f ", res[c]);

		}

		printf("\n");
		printf("\n");*/


		/********** Clean up root **********/
		delete[](sorted);
		res.clear();

	}

	/********** Clean up rest **********/
	delete[](original_array);
	delete[](sub_array);
	v.clear();
}