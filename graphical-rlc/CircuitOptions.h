#pragma once

#include <QWidget>
#include "ui_CircuitOptions.h"
#include "CircuitData.h"

class CircuitOptions : public QWidget {
	Q_OBJECT

public:
	CircuitOptions(std::shared_ptr<CircuitData> data, QWidget* parent = Q_NULLPTR);
	~CircuitOptions();
	// Functions
private:
	void buildMap();
	Circuit::Units extractBaseUnit(const QString& text);
	std::u16string getBestModel();
	inline Circuit::ComponetScale parseForScale(const QString& text);
	void validateFieldData(const QObject* lineEdit, bool isValid);
	// Save the data once the button is pressed 
	void saveAllData();
	void startSimulation();

	//Data
private:
	bool isInductorValid = true, isCapacitorValid = true;
	bool isTransient = true; // TODO: check if transient should be calculated
	// Simulation related
	QFuture<void>* future;
	QFutureWatcher<void>* watcher;
	std::u16string outputName;

	std::shared_ptr<CircuitData> circuitData;
	QList<QLineEdit*> lineEdits;
	Ui::CircuitOptions ui;

	// Maps
	std::map<QObject*, CircuitData::Keys> qobjectToDataMap;
	// Map between ComboBox first char and scale;
	std::map<const QChar, const Circuit::ComponetScale> qcharToScale = { {'n', Circuit::ComponetScale::NANO}, {'m', Circuit::ComponetScale::MILLI},
		{'V', Circuit::ComponetScale::BASE}, {'F', Circuit::ComponetScale::BASE}, {'r', Circuit::ComponetScale::BASE}, {'d', Circuit::ComponetScale::BASE},
		{'k', Circuit::ComponetScale::KILO}, {'M', Circuit::ComponetScale::MEGA}, {'G', Circuit::ComponetScale::GIGA} };
	std::map<const CircuitData::Keys, Circuit::ComponetScale> keyToScale = { {CircuitData::Keys::RESISTOR, Circuit::ComponetScale::BASE},
		{CircuitData::Keys::INDUCTOR, Circuit::ComponetScale::MILLI}, {CircuitData::Keys::CAPACITOR, Circuit::ComponetScale::NANO},
		{CircuitData::Keys::VOLTAGE, Circuit::ComponetScale::BASE}, {CircuitData::Keys::FREQUENCY, Circuit::ComponetScale::BASE},
		{CircuitData::Keys::PHASE, Circuit::ComponetScale::BASE}, {CircuitData::Keys::OFFSET, Circuit::ComponetScale::BASE} };
private slots:
	void displayComponentFields();
	void updateCircuitConfig(int index);
	void updateCircuitScale(const QString& text);
	void updateOutputComponent(const QString& text);
	void updateInputSignal(const QString& text);
	void updateConditionLabels();
	void simulateCircuit();
	void simulationComplete();
	void startSimulationAsync();
	void validateTextValue(const QString& text);

signals:
	void loadingChanged(int visible);
	void simulationOutputChanged(const QString& fileName);
	void transientClicked();
};
