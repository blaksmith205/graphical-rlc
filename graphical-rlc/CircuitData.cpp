#include "stdafx.h"
#include "CircuitData.h"
#include "UnitConverter.h"

void CircuitData::setCircuitConfig(Circuit::Configuration config)
{
	if (circuitConfig == config)
		return;
	circuitConfig = config;
	emit configChanged();
}

void CircuitData::setComponent(Keys key, Circuit::Components val)
{
	Circuit::Components cc = *(componentMap[key]);
	if (key == Keys::CIRCUIT_COMPONENTS && val != cc)
	{
		Circuit::Components circuitComponenet= val;
		emit componentsChanged();
	}
	else if (key == Keys::MEASURE_ACROSS && val != cc) {
		measureAcross = val;
		emit componentsChanged();
	}
}

void CircuitData::setComponentValue(Keys key, double val, Circuit::ComponetScale scale)
{
	Circuit::CircuitComponent storedVal = *(componentValueMap[key]);
	double scaledVal = UnitConverter::scale(val, scale);
	switch (key) {
	case Keys::RESISTOR:
		if (scaledVal != storedVal.value)
		{
			resistor.value = scaledVal;
			emit resistorChanged();
			emit componentValueChanged(Keys::RESISTOR);
		}
		break;
	case Keys::INDUCTOR:
		if (scaledVal != storedVal.value)
		{
			inductor.value = scaledVal;
			emit inductorChanged();
			emit componentValueChanged(Keys::INDUCTOR);
		}
		break;
	case Keys::CAPACITOR:
		if (scaledVal != storedVal.value)
		{
			capacitor.value = scaledVal;
			emit capacitorChanged();
			emit componentValueChanged(Keys::CAPACITOR);
		}
		break;
	case Keys::OFFSET:
		if (scaledVal != storedVal.value)
		{
			offset.value = scaledVal;
			emit offsetChanged();
			emit componentValueChanged(Keys::OFFSET);
		}
		break;
	}
}

void CircuitData::changeVoltage(double val, Circuit::ComponetScale scale, Circuit::Units unit)
{
	int unitMuliplier = unit == Circuit::Units::VOLTSP_P ? 2 : 1; // Wrong units will just be ignored
	double scaledVal = UnitConverter::scale(val, scale) * unitMuliplier;
	if (voltage.value != scaledVal)
	{
		voltage.value = scaledVal;
		emit voltageChanged();
		emit componentValueChanged(Keys::VOLTAGE);
	}
}

void CircuitData::changeFrequency(double val, Circuit::ComponetScale scale, Circuit::Units unit)
{
	// TODO: Add unique exception to throw
	// TODO: Tell user what unit was provided
	if ((unit != Circuit::Units::HERTZ) && (unit != Circuit::Units::RAD_PER_SEC))
	{
		throw std::exception("Wrong Unit.\nExpected either Circuit::Units::HERTZ or Circuit::Units::RAD_PER_SEC.\n");
	}
	double scaledVal = UnitConverter::scale(val, scale);
	if ((frequency.value != scaledVal) || (frequency.unit != unit))
	{
		frequency.value = scaledVal;
		frequency.unit = unit;
		emit frequencyChanged();
		emit componentValueChanged(Keys::FREQUENCY);
	}
}

void CircuitData::changePhase(double val, Circuit::Units unit)
{
	if ((unit != Circuit::Units::RADIANS) && (unit != Circuit::Units::DEGREES))
	{
		throw std::exception("Wrong Unit.\nExpected either Circuit::Units::RADIANS or Circuit::Units::DEGREES.\n");
	}
	if ((phase.value != val) || phase.unit != unit)
	{
		phase.value = val;
		phase.unit = unit;
		emit phaseChanged();
		emit componentValueChanged(Keys::PHASE);
	}
}

double CircuitData::calculateFrequency(bool outputInHz)
{
	if (outputInHz && frequency.unit != Circuit::Units::HERTZ)
	{
		return UnitConverter::radToHz(frequency.value);
	}
	else if (!outputInHz && frequency.unit == Circuit::Units::HERTZ)
	{
		return UnitConverter::hzToRad(frequency.value);
	}
	return frequency.value;
}

double CircuitData::calculatePhase(bool outputInDegrees) {
	if (outputInDegrees && phase.unit != Circuit::Units::RADIANS)
	{
		return UnitConverter::radToDeg(phase.value);
	}
	else if (!outputInDegrees && phase.unit == Circuit::Units::DEGREES)
	{
		return UnitConverter::degToRad(phase.value);
	}
	return phase.value;
}
