#pragma once
#include "CircuitInfo.h"
// Class to convert between units and correctly change scales
namespace UnitConverter {
	double scale(double val, CircuitComponetScale scale);
};

