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
	stringToComponentMap.insert({ "R", CircuitComponent::R });
	stringToComponentMap.insert({ "RL", CircuitComponent::RL });
	stringToComponentMap.insert({ "RC", CircuitComponent::RC });
	stringToComponentMap.insert({ "RLC", CircuitComponent::RLC });
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
	CircuitComponent selectedComponents = stringToComponentMap[text];
	circuitData->setComponent(CircuitData::Keys::CIRCUIT_COMPONENTS, selectedComponents);
	QString resource(":/Previews/Resources/");
	switch (selectedComponents) {
	case CircuitComponent::R:
		resource.append("AC_R_circuit.png");
		break;
	case CircuitComponent::RL:
		if (circuitData->getConfig() == CircuitConfiguration::SERIES)
			resource.append("AC_RL_series_circuit.png");
		else if (circuitData->getConfig() == CircuitConfiguration::PARALLEL)
			resource.append("AC_RL_parallel_circuit.png");
		break;
	case CircuitComponent::RC:
		if (circuitData->getConfig() == CircuitConfiguration::SERIES)
			resource.append("AC_RC_series_circuit.png");
		else if (circuitData->getConfig() == CircuitConfiguration::PARALLEL)
			resource.append("AC_RC_parallel_circuit.png");
		break;
	case CircuitComponent::RLC:
		if (circuitData->getConfig() == CircuitConfiguration::SERIES)
			resource.append("AC_RLC_series_circuit.png");
		else if (circuitData->getConfig() == CircuitConfiguration::PARALLEL)
			resource.append("AC_RLC_parallel_circuit.png");
		break;
	}
	showPreview(resource);
}

void ImageDisplay::updateCircuitPreview()
{
	updateComponents(ui.circuitSelection->currentText());
}


