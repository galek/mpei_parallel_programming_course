#include "Pch.h"

//m_ElementsInLine= count

MatrixData GenerateMatrixData(uint32_t _count, uint32_t _countOfNodes)
{
	MatrixData _newData;

	_newData.m_matrixArraySize = _count;

	std::srand(std::time(0)); //use current time as seed for random generator

	auto matrixArraySize = _count * _countOfNodes;
	_newData.SetMatrixArraySize(matrixArraySize);
	/**/
	{
		_newData.m_Matrix = new PFDV[matrixArraySize];
	}

	/*Fill array*/
	for (uint32_t i = 0; i < matrixArraySize; i++) {
		// Using random
		_newData.m_Matrix[i] = rand();
	}

	return _newData;
}

void PrintfMatrixData(MatrixData _data)
{
	for (uint32_t elIt = 0; elIt < _data.GetMatrixArraySize(); elIt++) {
		printf((ToString(_data.m_Matrix[elIt]) + " ").c_str());
	}

	printf("\n Finished printf \n");
}


void MatrixData::SetMatrixArraySize(uint32_t _matrixArraySize) {
	m_matrixArraySize = _matrixArraySize;
}

uint32_t MatrixData::GetMatrixArraySize() {
	return m_matrixArraySize;
}