#pragma once
#include "CircuitInfo.h"
#include "qmetatype.h"
class CircuitData : public QObject {
	Q_OBJECT
public:
	enum class Keys {
		CIRCUIT_COMPONENTS,
		MEASURE_ACROSS,
		RESISTOR,
		INDUCTOR,
		CAPACITOR,
		VOLTAGE,
		FREQUENCY,
		PHASE,
		OFFSET, 
		INITIAL_CONDITION,
		INITIAL_CONDITION_PRIME
	};
	// Keep source public because retrieval will probably not be that common
	Circuit::InputSignal voltageSource = Circuit::InputSignal::DC;

	// Call this to emit a voltageChanged signal
	void setVoltageWaveform(Circuit::InputSignal signalWaveform);

	// Functions to properly set data and emit changes
	void setCircuitConfig(Circuit::Configuration config);
	void setComponent(Keys key, Circuit::Components val);
	void setComponentValue(Keys key, double val, Circuit::ComponetScale scale);
	void setInitialCondition(Keys key, double val);

	// Convience functions to set values without the keys
	inline void setScaledResistor(double val, Circuit::ComponetScale scale) { setComponentValue(Keys::RESISTOR, val, scale); };
	inline void setScaledInductor(double val, Circuit::ComponetScale scale) { setComponentValue(Keys::INDUCTOR, val, scale); };
	inline void setScaledCapacitor(double val, Circuit::ComponetScale scale) { setComponentValue(Keys::CAPACITOR, val, scale); };
	inline void setScaledVoltage(double val, Circuit::ComponetScale scale, Circuit::Units unit) { changeVoltage(val, scale == Circuit::ComponetScale::MILLI ? scale : Circuit::ComponetScale::BASE, unit == Circuit::Units::VOLTSP_P ? unit : Circuit::Units::VOLTS); };
	inline void setScaledFrequency(double val, Circuit::ComponetScale scale, Circuit::Units unit) { changeFrequency(val, scale, unit); };
	inline void setPhase(double val, Circuit::Units unit) { changePhase(val, unit); };
	inline void setScaledOffset(double val, Circuit::ComponetScale scale) { setComponentValue(Keys::OFFSET, val, scale); };

	// Group the types together to return all at once.
	const inline std::vector<Circuit::Components> components() { return { circuitComponents, measureAcross }; };
	// Returns values for resistor, inductor, capacitor, voltage, frequency, phase, offset in that order
	const inline std::vector<Circuit::CircuitComponent> componentValues() { return { resistor, inductor, capacitor, voltage, frequency, phase, offset }; };
	// Returns values for initialCondition, initialConditionPrime
	const inline std::vector<double> initialConditions() { return { initialCondition, initialConditionPrime }; };
	
	// Return 1 element at a time using a map
	const inline Circuit::Configuration getConfig() { return circuitConfig; };
	const inline Circuit::Components getComponent(Keys key) { return *(componentMap[key]); };
	const inline Circuit::CircuitComponent getComponentValue(Keys key) { return *(componentValueMap[key]); };
	const inline double getInitialCondition(Keys key) { return *(conditionMap[key]); };

	// Calculate frequency depending on stored units. If outputInHz is true, the frequency is converted properly based on stored unit
	double calculateFrequency(bool outputInHz);
	// Calculate phase depending on stored units. If outputInDegrees is true, the phase is converted properly based on stored unit
	double calculatePhase(bool outputInDegrees);
private:
	void changeVoltage(double val, Circuit::ComponetScale scale, Circuit::Units unit);
	void changeFrequency(double val, Circuit::ComponetScale scale, Circuit::Units unit);
	void changePhase(double val, Circuit::Units unit);

	// Data
	Circuit::Configuration circuitConfig = Circuit::Configuration::SERIES;
	Circuit::Components circuitComponents = Circuit::Components::RLC;
	Circuit::Components measureAcross = Circuit::Components::R;
	Circuit::CircuitComponent resistor{ Circuit::Components::R, Circuit::Units::OHM };
	Circuit::CircuitComponent inductor{ Circuit::Components::L, Circuit::Units::HENRY };
	Circuit::CircuitComponent capacitor{ Circuit::Components::C, Circuit::Units::FARARDS };
	Circuit::CircuitComponent voltage{ Circuit::Components::NONE, Circuit::Units::VOLTS };
	Circuit::CircuitComponent frequency{ Circuit::Components::NONE, Circuit::Units::HERTZ };
	Circuit::CircuitComponent phase{ Circuit::Components::NONE, Circuit::Units::DEGREES };
	Circuit::CircuitComponent offset{ Circuit::Components::NONE, Circuit::Units::VOLTS };
	double initialCondition, initialConditionPrime;
	// Maps for single element retrieval
	std::map<Keys, const double*> conditionMap = { {Keys::INITIAL_CONDITION, &initialCondition}, {Keys::INITIAL_CONDITION_PRIME, &initialConditionPrime } };
	std::map<Keys, const Circuit::Components*> componentMap = { {Keys::CIRCUIT_COMPONENTS, &circuitComponents}, {Keys::MEASURE_ACROSS, &measureAcross} };
	std::map<Keys, const Circuit::CircuitComponent*> componentValueMap = { {Keys::RESISTOR, &resistor}, {Keys::INDUCTOR, &inductor}, {Keys::CAPACITOR, &capacitor},
		{Keys::VOLTAGE, &voltage}, {Keys::FREQUENCY, &frequency}, {Keys::PHASE, &phase}, {Keys::OFFSET, &offset} };
signals:
	void configChanged();
	void componentsChanged();
	void measuredOutputChanged();

	// General component value changed signal
	void componentValueChanged(const Keys&);

	// Specific component value changed signal
	void resistorChanged();
	void inductorChanged();
	void capacitorChanged();
	void voltageChanged();
	void frequencyChanged();
	void phaseChanged();
	void offsetChanged();

	void voltageSignalChanged();

	// Initial Conditions
	void initialConditionChanged();
	void initialConditionPrimeChanged();
};
Q_DECLARE_METATYPE(CircuitData::Keys);
