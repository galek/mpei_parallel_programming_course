#include "Pch.h"

#include <fstream>

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



void WriteToFile(uint32_t _count, PFDV* matrix)
{
	std::ofstream out_file;
	out_file.open("numbers.bin", std::ofstream::binary);

	std::cout << " Writing values: " << _count << std::endl;
	out_file << _count << " ";

	for (int count = 0; count < _count; count++)
	{
		if (matrix == nullptr)
		{
			std::cout << " Error on interator: " << count << std::endl;
		}
		else
		{
			out_file << matrix[count] << " ";
		}
	}
	out_file << std::endl;
	out_file.close();
}

MatrixData ReadFromFile()
{
	MatrixData data;

	std::ifstream in_file;
	in_file.open("numbers.bin", std::ofstream::binary);

	if (in_file.fail())
	{
		std::cout << "File opening error" << std::endl;
	}
	else
	{
		PFDV size = 0;
		in_file >> size;

		std::cout << "Reading " << size << " values" << std::endl;

		PFDV * values = new PFDV[size];

		for (int count = 0; count < size; count++)
		{
			in_file >> values[count];

			// debug only
			std::cout << values[count] << std::endl;
		}

		data.m_Matrix = values;
		data.m_matrixArraySize = size;
	}

	return data;
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

	auto dataCopy = compute.GetMatrixDataCopy();

	if (dataCopy.m_Matrix == nullptr)
	{
		std::cout << "error. Invalid pointer on  dataCopy.m_Matrix" << std::endl;
		return;
	}

	WriteToFile(dataCopy.m_matrixArraySize, dataCopy.m_Matrix);

	//system("pause");
	//ReadFromFile();
	//system("pause");
}