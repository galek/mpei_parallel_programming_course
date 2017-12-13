#include "Pch.h"


void merge(PFDV *a, PFDV *b, int l, int m, int r)
{
	int h, i, j, k;
	h = l;
	i = l;
	j = m + 1;

	while ((h <= m) && (j <= r)) {
		if (a[h] <= a[j]) {
			b[i] = a[h];
			h++;
		}

		else {
			b[i] = a[j];
			j++;
		}

		i++;
	}

	if (m < h) {
		for (k = j; k <= r; k++) {
			b[i] = a[k];
			i++;
		}
	}

	else {
		for (k = h; k <= m; k++) {
			b[i] = a[k];
			i++;
		}
	}

	for (k = l; k <= r; k++) {
		a[k] = b[k];
	}
}

void mergeSort(PFDV *a, PFDV *b, int l, int r)
{
	int m;

	if (l < r) {
		m = (l + r) / 2;

		mergeSort(a, b, l, m);
		mergeSort(a, b, (m + 1), r);
		merge(a, b, l, m, r);
	}
}

MatrixData ReadFromFile();
void SaveFile(MatrixData& data);

void MergeSortImpl(int world_rank, int world_size)
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
	PFDV *sub_array = new PFDV[size];

	// сравни параметры
	MPI_Scatterv(original_array, local_counts, offsets, MPI_PFDV, sub_array, local_counts[world_rank], MPI_PFDV, 0, MPI_COMM_WORLD);


	/********** Perform the mergesort on each process **********/
	PFDV *tmp_array = new PFDV[size];
	mergeSort(sub_array, tmp_array, 0, (size - 1));

	/********** Gather the sorted subarrays into one **********/
	PFDV *sorted = NULL;
	if (world_rank == 0) {
		sorted = new PFDV[n];
	}

	MPI_Gatherv(sub_array, size, MPI_PFDV, sorted, local_counts, offsets, MPI_PFDV, 0, MPI_COMM_WORLD);

	/********** Make the final mergeSort call **********/
	if (world_rank == 0) {

		PFDV *other_array = new PFDV[n];
		mergeSort(sorted, other_array, 0, (n - 1));

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
		delete[](other_array);

	}

	/********** Clean up rest **********/
	delete[](original_array);
	delete[](sub_array);
	delete[](tmp_array);
}
