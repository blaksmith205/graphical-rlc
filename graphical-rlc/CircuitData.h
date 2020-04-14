#pragma once
#include "CircuitInfo.h"
#include "qmetatype.h"
class CircuitData : QObject{
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
		OFFSET
	};
	// Keep source public because retrieval will probably not be that common
	InputSignal voltageSource;

	// Functions to properly set data and emit changes
	void setCircuitConfig(CircuitConfiguration config);
	void setComponent(Keys key, CircuitComponent val);
	void setComponentValue(Keys key, double val, CircuitComponetScale scale);

	// Convience functions to set values without the keys
	inline void setScaledResistor(double val, CircuitComponetScale scale) { setComponentValue(Keys::RESISTOR, val, scale); };
	inline void setScaledInductor(double val, CircuitComponetScale scale) { setComponentValue(Keys::INDUCTOR, val, scale); };
	inline void setScaledCapacitor(double val, CircuitComponetScale scale) { setComponentValue(Keys::CAPACITOR, val, scale); };
	inline void setScaledVoltage(double val, CircuitComponetScale scale, CircuitUnit unit) { changeVoltage(val, scale==CircuitComponetScale::MILLI?scale:CircuitComponetScale::BASE, unit); };
	inline void setScaledFrequency(double val, CircuitComponetScale scale, CircuitUnit unit) { changeFrequency(val, scale, unit); };
	inline void setPhase(double val, CircuitUnit unit) { changePhase(val, unit); };

	// Group the types together to return all at once.
	inline std::vector<CircuitComponent> components() { return {circuitComponents, measureAcross}; };
	inline std::vector<double> componentValues() { return { resistor, inductor, capacitor, voltage, frequency, phase, offset }; };

	// Return 1 element at a time using a map
	inline CircuitConfiguration getConfig() { return circuitConfig; };
	inline CircuitComponent getComponent(Keys key) { return *(componentMap[key]); };
	inline double getComponentValue(Keys key) { return *(componentValueMap[key]); };

	// Calculate frequency depending on stored units. If outputInHz is true, the frequency is converted properly based on stored unit
	double calculateFrequency(bool outputInHz);
	// Calculate phase depending on stored units. If outputInDegrees is true, the phase is converted properly based on stored unit
	double calculatePhase(bool outputInDegrees);
private:
	void changeVoltage(double val, CircuitComponetScale scale, CircuitUnit unit);
	void changeFrequency(double val, CircuitComponetScale scale, CircuitUnit unit);
	void changePhase(double val, CircuitUnit unit);
	

	// Data
	CircuitConfiguration circuitConfig;
	CircuitComponent circuitComponents;
	CircuitComponent measureAcross;
	double resistor;
	double inductor;
	double capacitor;
	double voltage;
	double frequency;
	double phase;
	double offset;
	CircuitUnit frequencyUnit = CircuitUnit::HERTZ;
	CircuitUnit phaseUnit = CircuitUnit::DEGREES;

	// Maps for single element retrieval
	std::map<Keys, CircuitComponent*> componentMap = { {Keys::CIRCUIT_COMPONENTS, &circuitComponents}, {Keys::MEASURE_ACROSS, &measureAcross} };
	std::map<Keys, double*> componentValueMap = { {Keys::RESISTOR, &resistor}, {Keys::INDUCTOR, &inductor}, {Keys::CAPACITOR, &capacitor},
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
	
	void voltageSourceChanged();
};
Q_DECLARE_METATYPE(CircuitData::Keys);
