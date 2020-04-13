#include "stdafx.h"
#include "UnitConverter.h"

static std::map<CircuitComponetScale, double> scaleMap = { {CircuitComponetScale::NANO, 1e-9}, {CircuitComponetScale::MICRO, 1e-6},
	{CircuitComponetScale::MILLI, 1e-3}, {CircuitComponetScale::BASE, 1}, {CircuitComponetScale::KILO, 1e+3}, {CircuitComponetScale::MEGA, 1e+6},
	{CircuitComponetScale::GIGA, 1e+9} };

double UnitConverter::scale(double val, CircuitComponetScale scale)
{
	return val * scaleMap[scale];
}
