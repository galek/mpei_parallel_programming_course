#include "Pch.h"



void Compute::Init()
{
	if (rank_proc == 0)
	{
		// TODO: USED ONLY FOR TESTS
		//__GenerateMatrix();
		__GenerateMatrix_Moc();


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

void Compute::QSort()
{
	// sort using the default operator<
	std::sort(_mData.m_Matrix, _mData.m_Matrix + 11);
}