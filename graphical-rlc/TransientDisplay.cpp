#include "stdafx.h"
#include <complex>
#include <QtConcurrent>
#include "TransientDisplay.h"
#include "ResourceManager.h"
#include "MatlabManager.h"

TransientDisplay::TransientDisplay(std::shared_ptr<CircuitData> data, QWidget* parent)
	: QWidget(parent), circuitData(data)
{
	ui.setupUi(this);
	future = new QFuture<void>();
	watcher = new QFutureWatcher<void>();
	connect(ui.responseSelection, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(updateResponse(const QString&)));
	connect(circuitData.get(), SIGNAL(configChanged()), this, SLOT(updateCircuitPreview()));
	connect(watcher, SIGNAL(finished()), this, SLOT(calcComplete()));
}

void TransientDisplay::showPreview(const QString& resource)
{
	circuitImage = ResourceManager::loadImage(this, resource);
	circuitImage = circuitImage.scaled(ui.circuitPreview->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.circuitPreview->setPixmap(QPixmap::fromImage(circuitImage));
}

void TransientDisplay::calcComplete()
{
	showOutput(outputName);
}

void TransientDisplay::calcTransientAsync()
{
	emit loadingChanged(1);
	*future = QtConcurrent::run(this, &TransientDisplay::calcTransient);
	watcher->setFuture(*future);
}

void TransientDisplay::calcTransient()
{
	// Create a matlab manager
	MatlabManager manager;
	// TODO: obtain a valid name for the output response
	outputName = "test";
	// TODO: make sure the data is not empty
	matlab::data::StructArray results = manager.calcTransient(circuitData.get(), u"generated/test");

	//std::complex<double> s1 = results[matlab::data::MATLABFieldIdentifier("s1")]
	//std::string complexStr = s1.real << " " << s1.imag;
	//ui.s1Output->setText(QString::fromStdString(complexStr));
}

void TransientDisplay::updateComponents(const QString& text)
{
	Circuit::Components selectedComponents = Circuit::componentMap[text];
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

void TransientDisplay::updateResponse(const QString& text)
{
	Circuit::Response resp = text == "Natural" ? Circuit::Response::NATURAL : Circuit::Response::STEP;
	circuitData->setResponse(resp);
	QString resource(":/Previews/Resources/");
	switch (circuitData->response)
	{
	case Circuit::Response::NATURAL:
		if (circuitData->getConfig() == Circuit::Configuration::PARALLEL)
			resource.append("DC_parallel_natural_response.png");
		else if (circuitData->getConfig() == Circuit::Configuration::SERIES)
			resource.append("DC_series_natural_response.png");
		break;
	case Circuit::Response::STEP:
		if (circuitData->getConfig() == Circuit::Configuration::PARALLEL)
			resource.append("DC_parallel_step_response.png");
		else if (circuitData->getConfig() == Circuit::Configuration::SERIES)
			resource.append("DC_series_step_response.png");
		break;
	}
	showPreview(resource);
}

void TransientDisplay::updateCircuitPreview()
{
	updateResponse(ui.responseSelection->currentText());
}

void TransientDisplay::showOutput(const QString& simulationOutput)
{
	simulationImage = ResourceManager::loadImage(this, "generated/" + simulationOutput + ".png");
	simulationImage = simulationImage.scaled(ui.simulationOutput->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.simulationOutput->setPixmap(QPixmap::fromImage(simulationImage));
	emit loadingChanged(0);
}


