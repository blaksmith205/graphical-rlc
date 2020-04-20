#include "stdafx.h"
#include "UnitConverter.h"

double UnitConverter::scale(double val, Circuit::ComponetScale scale)
{
	return val * Circuit::scaleMap[scale];
}
