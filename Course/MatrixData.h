#pragma once

struct MatrixData
{
	inline MatrixData()
	{
		m_Matrix = nullptr;
		m_Result = nullptr;
	}

	~MatrixData() {
		//TRACE_FUNCTION;
	}

	// will called manually
	inline void Clean() {
		SAFE_DELETE_ARRAY(m_Matrix);
		SAFE_DELETE_ARRAY(m_Result);
	}

	/*Count of string matrix*/
	uint32_t m_CountOfStrings = 0;
	uint32_t m_ElementsInLine = 0;

	/*Matrix pointers for array. Result matrix*/
	PFDV *m_Matrix = nullptr, *m_Result = nullptr;
	/*Used for computing matrix size of array*/
	uint32_t m_matrixArraySize = 0;

	/*DEPRECATED*/
	//double *tmp_matrix, *tmp_vector, m = 0.0;
	//int *_arrayCountOfProcesses, *range;

	//int *NumsMainLines;
	//int *NumsMainLinesIter;
	void SetMatrixArraySize(uint32_t matrixArraySize);
	uint32_t GetMatrixArraySize();
};

MatrixData GenerateMatrixData(uint32_t _count, uint32_t _countOfNodes);
void PrintfMatrixData(MatrixData _data);