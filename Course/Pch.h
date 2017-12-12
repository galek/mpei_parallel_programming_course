#pragma once

#ifndef PCH_H
#define PCH_H

#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <math.h>
#include <mpi.h>

#include <string>
#include <array>   
#include <algorithm>

#include <cstdlib>
#include <iostream>
#include <ctime>

#include "CommonDefines.h"
#include "MatrixData.h"

#include "Compute.h"
#include "MergeSort.h"

bool FileExist();

/// <summary>
/// Helper function that clocks and returns run times of a function
/// </summary>
template<typename Func>
const float ClockFunc(Func func)
{
	TimeData time;
	time.StartTime();
	func();
	time.EndTime();

	return time.GetDelta();
}

#endif