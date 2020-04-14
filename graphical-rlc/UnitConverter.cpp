#include "stdafx.h"
#include "UnitConverter.h"

static std::map<Circuit::ComponetScale, double> scaleMap = { {Circuit::ComponetScale::NANO, 1e-9}, {Circuit::ComponetScale::MICRO, 1e-6},
	{Circuit::ComponetScale::MILLI, 1e-3}, {Circuit::ComponetScale::BASE, 1}, {Circuit::ComponetScale::KILO, 1e+3}, {Circuit::ComponetScale::MEGA, 1e+6},
	{Circuit::ComponetScale::GIGA, 1e+9} };

double UnitConverter::scale(double val, Circuit::ComponetScale scale)
{
	return val * scaleMap[scale];
}
