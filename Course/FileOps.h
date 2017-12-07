#pragma once

#include "MatrixData.h"
#include "Compute.h"

void WriteToFile(uint32_t _count, PFDV* matrix);
MatrixData ReadFromFile();
void SaveFile(Compute& compute, bool _loadFromFile);