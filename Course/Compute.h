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

	void MergeSort();
private:
	void __GenerateMatrix();

	/*1 processor and 11 elements*/
	void __GenerateMatrix_Moc();

public:
	/*номер процесса world_rank*/
	int rank_proc = 0;
	/*world_size*/
	int g_NumProc = 0;

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

struct TimeData
{
	double  startTime, time_end;

	void StartTime();
	void EndTime();
	inline double GetDelta() {
		return (time_end - startTime);
	}
};