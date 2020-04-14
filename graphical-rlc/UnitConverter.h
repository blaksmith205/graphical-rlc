#pragma once
#include "CircuitInfo.h"
// Class to convert between units and correctly change scales
namespace UnitConverter {
	double scale(double val, Circuit::ComponetScale scale);
	inline double hzToRad(double frequency) { return 2 * M_PI * frequency; };
	inline double radToHz(double rads_per_sec) { return rads_per_sec / (2 * M_PI); }
	inline double degToRad(double degrees) { return (M_PI / 180) * degrees; };
	inline double radToDeg(double radians) { return (180 / M_PI) * radians; };
};

