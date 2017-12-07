#pragma once

struct MatrixData
{
	inline MatrixData()
	{
		m_Matrix = nullptr;
	}

	~MatrixData() {
		//TRACE_FUNCTION;
	}

	MatrixData(MatrixData &c) :
		m_Matrix(c.m_Matrix),
		m_CountOfStrings(c.m_CountOfStrings),
		m_matrixArraySize(c.m_matrixArraySize)
	{
	}

	MatrixData& operator=(MatrixData c) {

		m_CountOfStrings = (c.m_CountOfStrings);
		
		m_Matrix = (c.m_Matrix);

		m_matrixArraySize = (c.m_matrixArraySize);

		return *this;
	}


	// will called manually
	inline void Clean() {
		/*Count of string matrix*/
		m_CountOfStrings = 0;

		SAFE_DELETE_ARRAY(m_Matrix);

		/*Used for computing matrix size of array*/
		m_matrixArraySize = 0;
	}

	/*Count of string matrix*/
	uint32_t m_CountOfStrings = 0;

	/*Matrix pointers for array. Result matrix*/
	PFDV *m_Matrix = nullptr;
	/*Used for computing matrix size of array*/
	uint32_t m_matrixArraySize = 0;

	void SetMatrixArraySize(uint32_t matrixArraySize);
	uint32_t GetMatrixArraySize();
};

MatrixData GenerateMatrixData(uint32_t _count, uint32_t _countOfNodes);
void PrintfMatrixData(MatrixData _data);