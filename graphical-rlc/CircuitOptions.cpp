#include "stdafx.h"
#include <QtConcurrent>
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include "CircuitOptions.h"
#include "UnitConverter.h"

CircuitOptions::CircuitOptions(std::shared_ptr<CircuitData> data, QWidget *parent)
	: QWidget(parent), circuitData(data)
{
	ui.setupUi(this);
	future = new QFuture<void>();
	watcher = new QFutureWatcher<void>();
	buildMap();
	// Limit TextFields to only accept numbers
	lineEdits = this->findChildren<QLineEdit*>();
	double top = 10000; // Top - 1 is the number of characters limit
	for (auto lineEdit : lineEdits)
	{
		QDoubleValidator* dblValidator = new QDoubleValidator(0, top - 1, 8, lineEdit);
		dblValidator->setNotation(QDoubleValidator::StandardNotation);
		dblValidator->setLocale(QLocale::C);

		if (lineEdit == ui.resistorText || lineEdit == ui.inductorText || lineEdit == ui.capacitorText)
		{
			lineEdit->setValidator(dblValidator);
			connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(validateTextValue(const QString&)));
		}
		else {
			dblValidator->setBottom(-(top - 1));
			lineEdit->setValidator(dblValidator);
			connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(validateTextValue(const QString&)));
		}
	}
	// Update the scales per key
	for (auto comboBox : this->findChildren<QComboBox*>()) {
		connect(comboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(updateCircuitScale(const QString&)));
	}
	// Update data configuration
	connect(ui.circuitConfigSelection, SIGNAL(currentIndexChanged(int)), this, SLOT(updateCircuitConfig(int)));
	// Update which component to measure across
	connect(ui.measureAcrossSelection, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(updateOutputComponent(const QString&)));
	// Update the input signal waveform
	connect(ui.signalTypeSelection, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(updateInputSignal(const QString&)));
	// Save the data then simulate
	connect(ui.simulateButton, SIGNAL(clicked()), this, SLOT(simulateCircuit()));
	connect(ui.simulateButton, SIGNAL(clicked()), this, SLOT(startSimulationAsync()));
	connect(watcher, SIGNAL(finished()), this, SLOT(simulationComplete()));
}

void CircuitOptions::updateCircuitConfig(int index) {
	switch (index) {
	case 0:
		circuitData->setCircuitConfig(Circuit::Configuration::SERIES);
		break;
	case 1:
		circuitData->setCircuitConfig(Circuit::Configuration::PARALLEL);		
		break;
	}
}

void CircuitOptions::buildMap()
{
	qobjectToDataMap.insert({ui.resistorText, CircuitData::Keys::RESISTOR});
	qobjectToDataMap.insert({ui.inductorText, CircuitData::Keys::INDUCTOR});
	qobjectToDataMap.insert({ui.capacitorText, CircuitData::Keys::CAPACITOR});
	qobjectToDataMap.insert({ui.voltageText, CircuitData::Keys::VOLTAGE});
	qobjectToDataMap.insert({ui.frequencyText, CircuitData::Keys::FREQUENCY});
	qobjectToDataMap.insert({ui.phaseText, CircuitData::Keys::PHASE});
	qobjectToDataMap.insert({ui.offsetText, CircuitData::Keys::OFFSET});
	qobjectToDataMap.insert({ ui.resistorScale, CircuitData::Keys::RESISTOR });
	qobjectToDataMap.insert({ ui.inductorScale, CircuitData::Keys::INDUCTOR });
	qobjectToDataMap.insert({ ui.capacitorScale, CircuitData::Keys::CAPACITOR });
	qobjectToDataMap.insert({ ui.voltageScale, CircuitData::Keys::VOLTAGE });
	qobjectToDataMap.insert({ ui.frequencyScale, CircuitData::Keys::FREQUENCY });
	qobjectToDataMap.insert({ ui.phaseUnits, CircuitData::Keys::PHASE });
}

Circuit::Units CircuitOptions::extractBaseUnit(const QString& text)
{
	// Check if base is Omega
	QComboBox* sender = (QComboBox*)QObject::sender();
	if (sender == ui.resistorScale) return Circuit::Units::OHM;

	// Check if the whole string is the unit
	auto it = Circuit::unitMap.find(text);
	if (it != Circuit::unitMap.end())
	{
		return it->second;
	}

	// Obtain the unit from the first character
	it = Circuit::unitMap.find(text.at(0));
	if (it != Circuit::unitMap.end())
	{
		return it->second;
	}

	// Remove the scale and extra spaces
	auto substr = text.constData();
	substr = &substr[1];
	
	return Circuit::unitMap[QString(substr).trimmed()];
}

inline Circuit::ComponetScale CircuitOptions::parseForScale(const QString& text)
{
	// Account for Omega and normal bases
	if (text.size() == 1) return Circuit::ComponetScale::BASE;
	// Account for mu
	if (text.size() == 2 && text.at(1) == 'F' && text.at(0) != 'n') return Circuit::ComponetScale::MICRO;
	return text.size() == 1 ? Circuit::ComponetScale::BASE : qcharToScale[text.at(0)];
}

void CircuitOptions::saveAllData()
{
	for (auto lineEdit : lineEdits) {
		CircuitData::Keys key = qobjectToDataMap[lineEdit];
		QString text = lineEdit->text();
		double val = text.toDouble();
		switch (key)
		{
		case CircuitData::Keys::RESISTOR:
		case CircuitData::Keys::INDUCTOR:
		case CircuitData::Keys::CAPACITOR:
			circuitData->setComponentValue(key, val, keyToScale[key]);
			break;
		case CircuitData::Keys::VOLTAGE:
			circuitData->setScaledVoltage(val, keyToScale[key], extractBaseUnit(ui.voltageScale->currentText()));
			break;
		case CircuitData::Keys::FREQUENCY:
			circuitData->setScaledFrequency(val, keyToScale[key], extractBaseUnit(ui.frequencyScale->currentText()));
			break;
		case CircuitData::Keys::PHASE:
			circuitData->setPhase(val, extractBaseUnit(ui.phaseUnits->currentText()));
			break;
		case CircuitData::Keys::OFFSET:
			circuitData->setScaledOffset(val, keyToScale[key]);
			break;
		}
	}
}

void CircuitOptions::startSimulation()
{
	using namespace matlab::engine;

	// Connect to the MATLAB engine
	std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();

	// Create MATLAB data array factory
	matlab::data::ArrayFactory factory;

	// Create struct for simulation parameters
	auto parameterStruct = factory.createStructArray({ 1,4 }, {
		"SaveOutput",
		"OutputSaveName",
		"SaveTime",
		"TimeSaveName" });
	parameterStruct[0]["SaveOutput"] = factory.createCharArray("on");
	parameterStruct[0]["OutputSaveName"] = factory.createCharArray("yOut");
	parameterStruct[0]["SaveTime"] = factory.createCharArray("on");
	parameterStruct[0]["TimeSaveName"] = factory.createCharArray("tOut");

	// Put simulation parameter struct in MATLAB
	matlabPtr->setVariable(u"parameterStruct", parameterStruct);

	// Load vdp Simulink model
	FutureResult<void> loadFuture = matlabPtr->evalAsync(u"load_system('vdp')");
	//std::cout << "Loading Simulink model... " << std::endl;
	std::future_status loadStatus;
	do {
		loadStatus = loadFuture.wait_for(std::chrono::seconds(1));
	} while (loadStatus != std::future_status::ready);
	//std::cout << "vdp model loaded\n";

	// Run simulation
	FutureResult<void> simFuture = matlabPtr->evalAsync(u"simOut = sim('vdp',parameterStruct);");
	//std::cout << "Running simulation... " << std::endl;
	std::future_status simStatus;
	do {
		simStatus = loadFuture.wait_for(std::chrono::seconds(1));
	} while (simStatus != std::future_status::ready);
	//std::cout << "vdp simulation complete\n";

	// Get simulation data and create a graph
	matlabPtr->eval(u"y = simOut.get('yOut');");
	matlabPtr->eval(u"t = simOut.get('tOut');");
	matlabPtr->eval(u"f = figure('visible','off'); plot(t,y);"); // Polt without showing the image
	matlabPtr->eval(u"print('generated/vdpSimulation','-dpng')");
}

void CircuitOptions::updateCircuitScale(const QString& text)
{
	CircuitData::Keys key = qobjectToDataMap[QObject::sender()];
	if (key == CircuitData::Keys::PHASE) return;
	Circuit::ComponetScale newScale = parseForScale(text);
	if (keyToScale[key] == newScale) return;
	keyToScale[key] = newScale;
}

void CircuitOptions::updateOutputComponent(const QString& text)
{
	circuitData->setComponent(CircuitData::Keys::MEASURE_ACROSS, Circuit::componentMap[text]);
}

void CircuitOptions::updateInputSignal(const QString& text)
{
	circuitData->setVoltageWaveform(Circuit::inputSignalMap[text]);
}

void CircuitOptions::simulateCircuit()
{
	emit loadingChanged(1);
	saveAllData();
}

void CircuitOptions::simulationComplete()
{
	// Obtain the name of the created file ...
	emit simulationOutputChanged("vdpSimulation.png");
}

void CircuitOptions::startSimulationAsync()
{
	emit loadingChanged(1);
	*future = QtConcurrent::run(this, &CircuitOptions::startSimulation);
	watcher->setFuture(*future);
}

void CircuitOptions::validateTextValue(const QString& text) {
	// Obtain the sending lineEdit
	QLineEdit* lineEdit = (QLineEdit*) QObject::sender();
	QString str = text;
	// Validate the entire double
	int i = 0;
	QDoubleValidator* val = (QDoubleValidator*)lineEdit->validator();
	QValidator::State st = val->validate(str, i);

	if (st == QValidator::Acceptable)
	{
		// Value is valid
	}
}
