#include "stdafx.h"
#include "ImageDisplay.h"
#include "ResourceManager.h"

ImageDisplay::ImageDisplay(std::shared_ptr<CircuitData> data, QWidget* parent)
	: QWidget(parent), circuitData(data)
{
	ui.setupUi(this);
	buildMap();
	connect(ui.circuitSelection, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(updateComponents(const QString&)));
	connect(circuitData.get(), SIGNAL(configChanged()), this, SLOT(updateCircuitPreview()));
}

void ImageDisplay::buildMap()
{
	stringToComponentMap.insert({ "R", Circuit::Components::R });
	stringToComponentMap.insert({ "RL", Circuit::Components::RL });
	stringToComponentMap.insert({ "RC", Circuit::Components::RC });
	stringToComponentMap.insert({ "RLC", Circuit::Components::RLC });
}

void ImageDisplay::showOutput(const QString& simulationOutput) {
	simulationImage = ResourceManager::loadImage(this, "generated/" + simulationOutput);
	simulationImage = simulationImage.scaled(ui.simulationOutput->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.simulationOutput->setPixmap(QPixmap::fromImage(simulationImage));
	emit updateProgressBar(0);
}

void ImageDisplay::showPreview(const QString& resource) {
	circuitImage = ResourceManager::loadImage(this, resource);
	circuitImage = circuitImage.scaled(ui.circuitPreview->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.circuitPreview->setPixmap(QPixmap::fromImage(circuitImage));
}

void ImageDisplay::updateComponents(const QString& text) {
	Circuit::Components selectedComponents = stringToComponentMap[text];
	circuitData->setComponent(CircuitData::Keys::CIRCUIT_COMPONENTS, selectedComponents);
	QString resource(":/Previews/Resources/");
	switch (selectedComponents) {
	case Circuit::Components::R:
		resource.append("AC_R_circuit.png");
		break;
	case Circuit::Components::RL:
		if (circuitData->getConfig() == Circuit::Configuration::SERIES)
			resource.append("AC_RL_series_circuit.png");
		else if (circuitData->getConfig() == Circuit::Configuration::PARALLEL)
			resource.append("AC_RL_parallel_circuit.png");
		break;
	case Circuit::Components::RC:
		if (circuitData->getConfig() == Circuit::Configuration::SERIES)
			resource.append("AC_RC_series_circuit.png");
		else if (circuitData->getConfig() == Circuit::Configuration::PARALLEL)
			resource.append("AC_RC_parallel_circuit.png");
		break;
	case Circuit::Components::RLC:
		if (circuitData->getConfig() == Circuit::Configuration::SERIES)
			resource.append("AC_RLC_series_circuit.png");
		else if (circuitData->getConfig() == Circuit::Configuration::PARALLEL)
			resource.append("AC_RLC_parallel_circuit.png");
		break;
	}
	showPreview(resource);
}

void ImageDisplay::updateCircuitPreview()
{
	updateComponents(ui.circuitSelection->currentText());
}


