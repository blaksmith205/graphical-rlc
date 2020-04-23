#include "stdafx.h"
#include <iostream>
#include <complex>
#include <QtConcurrent>
#include "TransientDisplay.h"
#include "ResourceManager.h"
#include "MatlabManager.h"

typedef std::basic_stringbuf<char16_t> StringBuf;

TransientDisplay::TransientDisplay(std::shared_ptr<CircuitData> data, QWidget* parent)
	: QWidget(parent), circuitData(data), matlabOutput(std::make_shared<StringBuf>()), matlabError(std::make_shared<StringBuf>())
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
	// Display the results
	QString s1Str, s2Str;
	// Format S1
	if (s1.imag() == 0)
	{
		s1Str = QString::number(s1.real());
	}
	else
	{
		s1Str = complexFormat.arg(QString::number(s1.real()), s1.imag() < 0 ? QString("-") : QString("+"), QString::number(abs(s1.imag())));
	}
	if (s2.imag() == 0)
	{
		s2Str = QString::number(s2.real());
	}
	else
	{
		s2Str = complexFormat.arg(QString::number(s2.real()), s2.imag() < 0 ? QString("-") : QString("+"), QString::number(abs(s2.imag())));
	}
	ui.s1Output->setText(s1Str);
	ui.s2Output->setText(s2Str);
	ui.wdOutput->setText(QString::number(wd));
	ui.responseType->setText(response);
	ui.equationRHS->setText(equation);
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
	bool isSeries = circuitData->getConfig() == Circuit::Configuration::SERIES;
	bool isStep = circuitData->response == Circuit::Response::STEP;

	// Create a matlab manager
	MatlabManager manager;
	outputName = ResourceManager::validTransientOutputName(isSeries, isStep);
	// TODO: make sure the data is not empty
	std::vector<matlab::data::Array> results = manager.calcTransient(circuitData.get(), outputName.toStdU16String(), matlabOutput, matlabError);

	// Obtain the data
	if (results.size() < 5)
	{
		response = "";
		s1 = 0;
		s2 = 0;
		wd = 0;
		equation = "";
		return;
	}

	matlab::data::CharArray _response = results[3];
	response = QString::fromStdString(_response.toAscii());
	if (response == QString("Under"))
	{
		matlab::data::TypedArray<std::complex<double>> _s1 = results[0];
		matlab::data::TypedArray<std::complex<double>> _s2 = results[1];
		s1 = _s1[0];
		s2 = _s2[0];
	}
	else
	{
		matlab::data::TypedArray<double> _s1 = results[0];
		matlab::data::TypedArray<double> _s2 = results[1];
		s1 = std::complex<double>(_s1[0], 0);
		s2 = std::complex<double>(_s2[0], 0);
	}

	matlab::data::TypedArray<double> _wd = results[2];
	matlab::data::CharArray _equation = results[4];

	// save the common data
	wd = _wd[0];
	equation = QString::fromStdString(_equation.toAscii());
	equation = equation.replace("exp", "e^");
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

void TransientDisplay::showOutput(const QString& fullRelativeName)
{
	simulationImage = ResourceManager::loadImage(this, fullRelativeName);
	simulationImage = simulationImage.scaled(ui.simulationOutput->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.simulationOutput->setPixmap(QPixmap::fromImage(simulationImage));
	emit loadingChanged(0);
}


