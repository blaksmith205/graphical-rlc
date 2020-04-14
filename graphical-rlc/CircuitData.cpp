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
			emit componentValueChanged(Keys::RESISTOR);
		}
		break;
	case Keys::INDUCTOR:
		if (scaledVal != storedVal)
		{
			inductor = scaledVal;
			emit inductorChanged();
			emit componentValueChanged(Keys::INDUCTOR);
		}
		break;
	case Keys::CAPACITOR:
		if (scaledVal != storedVal)
		{
			capacitor = scaledVal;
			emit capacitorChanged();
			emit componentValueChanged(Keys::CAPACITOR);
		}
		break;
	}
}

void CircuitData::changeVoltage(double val, CircuitComponetScale scale, CircuitUnit unit)
{
	int unitMuliplier = unit == CircuitUnit::VOLTSP_P ? 2 : 1; // Wrong units will just be ignored
	double scaledVal = UnitConverter::scale(val, scale) * unitMuliplier;
	if (voltage != scaledVal)
	{
		voltage = scaledVal;
		emit voltageChanged();
		emit componentValueChanged(Keys::VOLTAGE);
	}
}

void CircuitData::changeFrequency(double val, CircuitComponetScale scale, CircuitUnit unit)
{
	// TODO: Add unique exception to throw
	// TODO: Tell user what unit was provided
	if ((unit != CircuitUnit::HERTZ) && (unit != CircuitUnit::RAD_PER_SEC))
	{
		throw std::exception("Wrong Unit.\nExpected either CircuitUnit::HERTZ or CircuitUnit::RAD_PER_SEC.\n");
	}
	double scaledVal = UnitConverter::scale(val, scale);
	if ((frequency != scaledVal) || (frequencyUnit != unit))
	{
		frequency = scaledVal;
		frequencyUnit = unit;
		emit frequencyChanged();
		emit componentValueChanged(Keys::FREQUENCY);
	}
}

void CircuitData::changePhase(double val, CircuitUnit unit)
{
	if ((unit != CircuitUnit::RADIANS) && (unit != CircuitUnit::DEGREES))
	{
		throw std::exception("Wrong Unit.\nExpected either CircuitUnit::RADIANS or CircuitUnit::DEGREES.\n");
	}
	if ((phase != val) || phaseUnit != unit)
	{
		phase = val;
		phaseUnit = unit;
		emit phaseChanged();
		emit componentValueChanged(Keys::PHASE);
	}
}

double CircuitData::calculateFrequency(bool outputInHz)
{
	if (outputInHz && frequencyUnit != CircuitUnit::HERTZ)
	{
		return UnitConverter::radToHz(frequency);
	}
	else if (!outputInHz && frequencyUnit == CircuitUnit::HERTZ)
	{
		return UnitConverter::hzToRad(frequency);
	}
}

double CircuitData::calculatePhase(bool outputInDegrees) {
	if (outputInDegrees && phaseUnit != CircuitUnit::RADIANS)
	{
		return UnitConverter::radToDeg(phase);
	}
	else if (!outputInDegrees && phaseUnit == CircuitUnit::DEGREES)
	{
		return UnitConverter::degToRad(phase);
	}
}
