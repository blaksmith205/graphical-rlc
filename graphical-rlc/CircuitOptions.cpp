#include "stdafx.h"
#include "CircuitOptions.h"

CircuitOptions::CircuitOptions(std::shared_ptr<CircuitData> data, QWidget *parent)
	: QWidget(parent), circuitData(data)
{
	ui.setupUi(this);
	buildMap();
	// Limit TextFields to only accept numbers
	double top = 10000; // Top - 1 is the number of characters limit
	QList<QLineEdit*> lineEdits = this->findChildren<QLineEdit*>();
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
	// Update data configuration
	connect(ui.circuitConfigSelection, SIGNAL(currentIndexChanged(int)), this, SLOT(updateCircuitConfig(int)));
}

void CircuitOptions::updateCircuitConfig(int index) {
	switch (index) {
	case 0:
		circuitData->setCircuitConfig(CircuitConfiguration::SERIES);
		break;
	case 1:
		circuitData->setCircuitConfig(CircuitConfiguration::PARALLEL);		
		break;
	}
}

void CircuitOptions::buildMap()
{
	fieldToDataMap.insert({ui.resistorText, CircuitData::Keys::RESISTOR});
	fieldToDataMap.insert({ui.inductorText, CircuitData::Keys::INDUCTOR});
	fieldToDataMap.insert({ui.capacitorText, CircuitData::Keys::CAPACITOR});
	fieldToDataMap.insert({ui.voltageText, CircuitData::Keys::VOLTAGE});
	fieldToDataMap.insert({ui.frequencyText, CircuitData::Keys::FREQUENCY});
	fieldToDataMap.insert({ui.phaseText, CircuitData::Keys::PHASE});
	fieldToDataMap.insert({ui.offsetText, CircuitData::Keys::OFFSET});
}

void CircuitOptions::saveData()
{
	QList<QLineEdit*> lineEdits = this->findChildren<QLineEdit*>();
	for (const auto linedit : lineEdits) {
		//circuitData->setComponentValue()
	}
}

void CircuitOptions::validateTextValue(const QString& text) {
	// Obtain the sending lineEdit
	QLineEdit* lineEdit = (QLineEdit*) QObject::sender();
	QString str = text;
	// Validate the entire double
	int i = 0;
	QDoubleValidator* val = (QDoubleValidator*)lineEdit->validator();
	QValidator::State st = val->validate(str, i);

	if (st == QValidator::Acceptable) {
		// Input is a valid number
	}
}
