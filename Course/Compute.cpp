#include "Pch.h"

#include <iterator>
#include <algorithm>

// Count of elements
static const uint32_t gCount = 99999999;

//#define USE_MOC_DATA 1

MatrixData ReadFromFile();


void SetMatrixZero(PFDV*_matrix, uint32_t _size)
{
	auto old = _matrix;
	/*Result vector after sorting*/
	_matrix = new PFDV[_size];
	/*Result matrix*/
	for (uint32_t i = 0; i < _size; i++) {
		_matrix[i] = PFDV_ZERO;
	}

	SAFE_DELETE(old);
}

void Compute::Init()
{
	// Рассылка size
	MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	// Завершили операцию
	MPI_Barrier(MPI_COMM_WORLD);
}

void TimeData::StartTime()
{
	startTime = MPI_Wtime();
}

void TimeData::EndTime()
{
	time_end = MPI_Wtime();
}
