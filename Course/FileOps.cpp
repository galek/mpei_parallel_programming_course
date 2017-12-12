#include "Pch.h"
#include "FileOps.h"

#include <fstream>

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
			std::cout << " Error on iterator: " << count << std::endl;
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

	uint32_t _readingCounter = 0;

	if (in_file.fail())
	{
		std::cout << "File opening error" << std::endl;
	}
	else
	{
		PFDV size = 0;
		in_file >> size;

		//std::cout << "Reading " << size << " values" << std::endl;

		PFDV * values = new PFDV[size];

		for (int count = 0; count < size; count++)
		{
			in_file >> values[count];

			// debug only
			//std::cout << values[count] << std::endl;

			_readingCounter++;
		}

		data.m_Matrix = values;
		data.m_matrixArraySize = size;
	}


	//std::cout << "Readed " << _readingCounter << " values" << std::endl;

	return data;
}

void SaveFile(MatrixData& data)
{
	if (data.m_Matrix == nullptr)
	{
		std::cout << "error. Invalid pointer on  dataCopy.m_Matrix" << std::endl;
		return;
	}

	WriteToFile(data.m_matrixArraySize, data.m_Matrix);
}