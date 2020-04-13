#include "stdafx.h"
#include "CircuitData.h"
#include "UnitConverter.h"

void CircuitData::setCircuitConfig(CircuitConfiguration config)
{
	if (circuitConfig == config)
		return;
	circuitConfig = config;
	emit configChanged();
}

void CircuitData::setComponent(Keys key, CircuitComponent val)
{
	CircuitComponent cc = *(componentMap[key]);
	if (key == Keys::CIRCUIT_COMPONENTS && val != cc)
	{
		circuitComponents = val;
		emit componentsChanged();
	}
	else if (key == Keys::MEASURE_ACROSS && val != cc) {
		measureAcross = val;
		emit componentsChanged();
	}
}

void CircuitData::setComponentValue(Keys key, double val, CircuitComponetScale scale)
{
	double storedVal = *(componentValueMap[key]);
	double scaledVal = UnitConverter::scale(val, scale);
	switch (key) {
	case Keys::RESISTOR:
		if (scaledVal != storedVal)
		{
			resistor = scaledVal;
			emit resistorChanged();
		}
		break;
	case Keys::INDUCTOR:
		if (scaledVal != storedVal)
		{
			inductor = scaledVal;
			emit inductorChanged();
		}
		break;
	case Keys::CAPACITOR:
		if (scaledVal != storedVal)
		{
			capacitor = scaledVal;
			emit capacitorChanged();
		}
		break;
	}
}

CircuitComponent CircuitData::getComponent(const QString&)
{
	return CircuitComponent();
}

double CircuitData::getComponentValue(const QString&)
{
	return 0.0;
}

void CircuitData::changeVoltage(double val, CircuitComponetScale scale, CircuitUnit unit)
{
	bool changed = false;
	double storedVal = *(componentValueMap[Keys::VOLTAGE]);
	if (voltageUnit == unit && storedVal == val)
	{

	}
}

void CircuitData::changeFrequency(double val, CircuitComponetScale scale, CircuitUnit unit)
{
}

void CircuitData::changePhase(double val, CircuitComponetScale scale, CircuitUnit unit)
{
}
