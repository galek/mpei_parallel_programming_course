#pragma once



struct Compute
{
	inline Compute() {}

	~Compute() {
		_mData.Clean();
	}

	/**/
	void RunGauss()
	{
		/*	_ForwardGauss();
		_BackGauss();

		MPI_Gatherv(tmp_res, range[rank_proc], MPI_DOUBLE, m_Result, range, _arrayCountOfProcesses, MPI_DOUBLE, 0, MPI_COMM_WORLD);*/
	}

	void Init();
	void QSort();

private:
	void __GenerateMatrix();

	/*1 processor and 11 elements*/
	void __GenerateMatrix_Moc();

public:
	int g_NumProc;

	double  startTime, time_end, DeltaTime;
	/*номер процесса*/
	int rank_proc = 0;
	/*число процессов*/
	int size = 0, status;


	void SetMatrixData(MatrixData _data) {
		_mData.Clean();
		_mData = _data;
	}

	inline MatrixData GetMatrixDataCopy() {
		return _mData;
	}

private:
	MatrixData _mData;
};

