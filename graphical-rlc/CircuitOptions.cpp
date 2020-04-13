#include "stdafx.h"
#include "CircuitOptions.h"

CircuitOptions::CircuitOptions(std::shared_ptr<CircuitData> data, QWidget *parent)
	: QWidget(parent), circuitData(data)
{
	ui.setupUi(this);
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
