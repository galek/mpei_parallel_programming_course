#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

//http://www2.sscc.ru/Publikacii/Primery_Prll/Primery.htm

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if(x){delete x; x=nullptr;}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) if(x){delete []x; x=nullptr;}
#endif

struct Compute
{
	~Compute()
	{
		SAFE_DELETE_ARRAY(m_Matrix);
		SAFE_DELETE_ARRAY(m_Vector);
		SAFE_DELETE_ARRAY(m_Result);
	}

	void DestroyThis() { delete this; }

	void Init()
	{
		if (rank_proc == 0)
		{
			_PopulateMatrix();
			printf("\n Timer started");
		}

		MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

		//����������� ������� ����� ������, ������������� �� ���������� ��������

		/*����� �����, ��� �� ������������� �� ���������*/
		int tmp_num_str = size;

		for (int i = 0; i < rank_proc; i++) {
			tmp_num_str -= tmp_num_str / (g_NumProc - i);
		}

		m_CountOfStrings = tmp_num_str / (g_NumProc - rank_proc);

		{
			tmp_matrix = new double[m_CountOfStrings*size];
			tmp_vector = new double[m_CountOfStrings];
			tmp_res = new double[m_CountOfStrings];

			NumsMainLines = new int[size];
			NumsMainLinesIter = new int[m_CountOfStrings];

			_arrayCountOfProcesses = new int[g_NumProc];
			range = new int[g_NumProc];
		}

		for (int i = 0; i < m_CountOfStrings; i++) {
			NumsMainLinesIter[i] = -1;
		}

		/*������������� ������ ����� ����������*/
		_TaskDistribution();
	}

	/**/
	void RunGauss()
	{
		_ForwardGauss();
		_BackGauss();

		//���� ������, �������� �� ���� ������ (�������� c ������ 0)
		MPI_Gatherv(tmp_res, range[rank_proc], MPI_DOUBLE, m_Result, range, _arrayCountOfProcesses, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	}

private:
	/*��������� ������� ������������ ����������*/
	void _PopulateMatrix()
	{
		/*!!������ ���������� �����!!*/
		size = 15000;
		srand(p);

		/**/
		{
			m_Matrix = new double[size*size];
			m_Vector = new double[size];
			m_Result = new double[size];
		}

		/*��������� ���������� ����������*/
		for (int i = 0; i < size*size; i++)
		{
			m_Matrix[i] = rand();
		}

		/*��������� ���������� ����������*/
		for (int i = 0; i < size; i++)
		{
			m_Vector[i] = rand();
			/*�������������� ������ �� 0*/
			m_Result[i] = 0;
		}
	}

	/*��������� ��������� � �������� ������� ������*�� ���������*/
	void _ExceptRaws(int iter_numb, double *glob_str)
	{
		double koef;
		/**/
		for (int i = 0; i < m_CountOfStrings; i++)//��� ������ ������ � ��������
		{
			if (NumsMainLinesIter[i] == -1)
			{
				koef = tmp_matrix[i*size + iter_numb] / glob_str[iter_numb];

				for (int j = iter_numb; j < size; j++)
				{
					tmp_matrix[i*size + j] -= glob_str[j] * koef;
				}

				tmp_vector[i] -= glob_str[size] * koef;
			}
		}
	}

	// ������������� �������� ������ ����� ����������
	void _TaskDistribution()
	{
		/*Index of first element of matrixes what will sended to process*/
		int *index_to_matr;
		/*Count of elements of matrix what will sended to process*/
		int *range_to_proc;

		int _numProcStr;

		int tmp_num_str;

		int prev_sz;
		int prev_indx;

		int portion;

		index_to_matr = new int[g_NumProc];
		range_to_proc = new int[g_NumProc];

		tmp_num_str = size;

		for (int i = 0; i < g_NumProc; i++)  //����������, ������� ��������� ������� ����� �������� ������� ��������
		{
			prev_sz = (i == 0) ? 0 : range_to_proc[i - 1];

			prev_indx = (i == 0) ? 0 : index_to_matr[i - 1];

			//����� �����, �������� ����������� ��������
			portion = (i == 0) ? 0 : _numProcStr;

			tmp_num_str -= portion;

			_numProcStr = tmp_num_str / (g_NumProc - i);

			range_to_proc[i] = _numProcStr*size;

			index_to_matr[i] = prev_indx + prev_sz;
		}

		// �������� ������� ������� ��������
		MPI_Scatterv(m_Matrix, range_to_proc, index_to_matr, MPI_DOUBLE, tmp_matrix, range_to_proc[rank_proc], MPI_DOUBLE, 0, MPI_COMM_WORLD);

		//���������
		tmp_num_str = size;

		for (int i = 0; i < g_NumProc; i++)
		{
			int prev_sz = (i == 0) ? 0 : range[i - 1];
			int prev_indx = (i == 0) ? 0 : _arrayCountOfProcesses[i - 1];
			int portion = (i == 0) ? 0 : range[i - 1];
			tmp_num_str -= portion;
			range[i] = tmp_num_str / (g_NumProc - i);
			_arrayCountOfProcesses[i] = prev_indx + prev_sz;
		}

		{
			//�������� �������
			/*
			�����������:
			-������ � ������������ ������������ �������� �� ������ �������
			-������ � �������� ��������, � ������� ���� �������� �������� �� m_Vector

			�����������:
			���������� ��������� �� ������ �������
			*/
			MPI_Scatterv(m_Vector, range, _arrayCountOfProcesses, MPI_DOUBLE, tmp_vector, range[rank_proc], MPI_DOUBLE, 0, MPI_COMM_WORLD);
			status = 1;
			MPI_Bcast(&status, 1, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Bcast(&m, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		}

		SAFE_DELETE_ARRAY(range_to_proc);
		SAFE_DELETE_ARRAY(index_to_matr);
	}

	void _ForwardGauss()
	{
		// ������ ������� ������ �� ���������� ��������
		int VedIndex;
		//������������ �������+����� ��������, � �������� ��
		struct
		{
			double max_val; int rank_proc;
		} _localMax, _globalMax;

		double* glb_maxst = new double[size + 1]; //�.�. ������ �������+�������� �������

		for (int i = 0; i < size; i++) //������� ���� �������
		{
			// ���������� ������� ������
			double max_val = 0;
			int tmp_index = -1;

			for (int j = 0; j < m_CountOfStrings; j++)
			{
				tmp_index = j;
				if ((NumsMainLinesIter[j] == -1) && (max_val < fabs(tmp_matrix[i + size*j])))
				{
					// ���� ������������ �� ������
					max_val = fabs(tmp_matrix[i + size*j]);
					VedIndex = j;
				}
			}

			_localMax.max_val = max_val;
			_localMax.rank_proc = rank_proc;

			//������ ������� ��������� ���� �������� ������������ ������� �� ���� �������, ��� ������� ��������� ��� ��������� ������������ �������
			MPI_Allreduce(&_localMax, &_globalMax, 1, MPI_DOUBLE_INT, MPI_MAXLOC, MPI_COMM_WORLD);

			//���������� ������� ������ ���� �������
			if (rank_proc == _globalMax.rank_proc)
			{
				if (_globalMax.max_val == 0)
				{
					status = 2;
					MPI_Barrier(MPI_COMM_WORLD);
					MPI_Send(&status, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD);
					NumsMainLinesIter[tmp_index] = i;
					NumsMainLines[i] = _arrayCountOfProcesses[rank_proc] + VedIndex;
					continue;
				}
				else
				{
					NumsMainLinesIter[VedIndex] = i;  // ����� ��������, �� ������� ������ � ��������� ������� �������� ������� ��� ���� �������
					NumsMainLines[i] = _arrayCountOfProcesses[rank_proc] + VedIndex;   //����������� ����� ������� ������ �������
				}
			}
			MPI_Bcast(&NumsMainLines[i], 1, MPI_INT, _globalMax.rank_proc, MPI_COMM_WORLD);
			if (rank_proc == _globalMax.rank_proc)
			{
				for (int j = 0; j < size; j++) {
					glb_maxst[j] = tmp_matrix[VedIndex*size + j];
				}
				glb_maxst[size] = tmp_vector[VedIndex];
			}
			//�������� ������� ������ ���� ���������
			MPI_Bcast(glb_maxst, size + 1, MPI_DOUBLE, _globalMax.rank_proc, MPI_COMM_WORLD);
			//���������� ����������� � ������� � ������� i
			_ExceptRaws(i, glb_maxst);
		};
	}

	/*���������� ���� ��������, ����������� ������� ������� ������, � ����� ���� ������ �� ��������*/
	void Frp(int stringIndex, // ����� ������, ������� ���� ������� �� ����������� ��������
		int &iterationrank_proc, // �������, �� ������� ��� ������
		int &IterationItervedindex) // ��������� ����� ���� ������ (� ������ ������ ��������)
	{
		for (int i = 0; i < g_NumProc - 1; i++) //���������� ���� ��������, ����������� ������ ������
		{
			if ((_arrayCountOfProcesses[i] <= stringIndex) && (stringIndex < _arrayCountOfProcesses[i + 1]))
				iterationrank_proc = i;
		}
		if (stringIndex >= _arrayCountOfProcesses[g_NumProc - 1])
			iterationrank_proc = g_NumProc - 1;
		IterationItervedindex = stringIndex - _arrayCountOfProcesses[iterationrank_proc];
	}

	/*�������� ���*/
	void _BackGauss()
	{
		int Iterrank_proc;  // ���� ��������, ��������� ������� ������� ������
		int IndexVed;    // ��������� �� ����� �������� ����� ������� �������
		double IterRes;   // �������� Xi, ��������� �� ��������
		double val;

		// �������� ����
		for (int i = size - 1; i >= 0; i--)
		{
			// ���������� ���� ��������, ����������� ������� ������� ������, � ����� ���� ������ �� ��������
			Frp(NumsMainLines[i], Iterrank_proc, IndexVed);

			// ��������� �������� �����������
			if (rank_proc == Iterrank_proc)
			{
				if (tmp_matrix[IndexVed*size + i] == 0)
				{
					if (tmp_vector[IndexVed] == 0)
						IterRes = m;
					else
					{
						status = 0;
						MPI_Barrier(MPI_COMM_WORLD);
						MPI_Send(&status, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD);
						break;
					}
				}
				else
					IterRes = tmp_vector[IndexVed] / tmp_matrix[IndexVed*size + i];
				tmp_res[IndexVed] = IterRes; //����� �������� ����������
			}

			MPI_Bcast(&IterRes, 1, MPI_DOUBLE, Iterrank_proc, MPI_COMM_WORLD);
			for (int j = 0; j < m_CountOfStrings; j++) //����������� ��������� ����������
				if (NumsMainLinesIter[j] < i)
				{
					val = tmp_matrix[size*j + i] * IterRes;
					tmp_vector[j] -= val;
				}
		}
	}

public:
	// ���������� ���������
	int g_NumProc;

	// ���������� ��� ��������� �������
	double  startTime, time_end, DeltaTime;
	// ���� ��������
	int rank_proc;
	//status - ������ ������� ������
	int size, p, status;

private:
	int m_CountOfStrings; //m_CountOfStrings-����� ����� �������� �������, ���������� �� ���������� ��������
	double *m_Matrix, *m_Vector, *m_Result;
	/*��������� ����������*/
	/*tmp_matrix, tmp_vector, tmp_res - ����� �������, ������� b � ������� x �� ������������ ��������*/
	double *tmp_matrix, *tmp_vector, *tmp_res, m = 0.0;
	/*
	������ �������� � ���������� ���������. ������ ������� ������ ������ ������� �������� �� ������� ��������� ������, ������� ����������� ��������
	������ � ������������ ������������ �������� �� ������� ��������� ������ �� ������ �������
	*/
	int *_arrayCountOfProcesses, *range;

	//������ ������� ������� ����� �� ������ �������� // i-�� �������� - j-�� ������ �������
	int *NumsMainLines;
	/* ������ ������� ��������, �� ������� ��������������� ������ �������, ������������� �� ��������, �������  �������� */
	int *NumsMainLinesIter;
};

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	setvbuf(stdout, 0, _IONBF, 0);

	MPI_Init(&argc, &argv);

	Compute compute;

	MPI_Comm_rank(MPI_COMM_WORLD, &compute.rank_proc);
	MPI_Comm_size(MPI_COMM_WORLD, &compute.g_NumProc);

	// �������������� �������������
	MPI_Barrier(MPI_COMM_WORLD);

	compute.Init();
	// �������������� �������������
	MPI_Barrier(MPI_COMM_WORLD);

	compute.startTime = MPI_Wtime();

	compute.RunGauss();

	MPI_Barrier(MPI_COMM_WORLD);
	compute.time_end = MPI_Wtime();
	compute.DeltaTime = (compute.time_end - compute.startTime);

	if (compute.rank_proc == 0)
	{
		if (compute.status == 1) // ������� �������
			printf("\nRoots: \n");
		if (compute.status == 0) printf("\nNo roots.\n"); // ������� ���
		if (compute.status == 2) // ������� ���������� �����
			printf("\nContinum. Solution: \n\n");

		printf("\n\nTime= %f\n", compute.DeltaTime);
	}
	if (compute.rank_proc == 0)
	{
		printf("\n FINISH!");
	}

	compute.DestroyThis();
	MPI_Finalize();

}
