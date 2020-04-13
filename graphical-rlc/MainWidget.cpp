#include "stdafx.h"
#include <QtConcurrent>
#include "mainwidget.h"
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include "ResourceManager.h"

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.progressBar->setVisible(false);
	future = new QFuture<void>();
	watcher = new QFutureWatcher<void>();
	buildComponentMap();
	// Connect the events to the proper handlers
	connect(ui.simulateButton, SIGNAL(clicked()), this, SLOT(startSimulationAsync()));
	connect(watcher, SIGNAL(finished()), this, SLOT(showOutput()));
	connect(ui.circuitConfigSelection,
		SIGNAL(currentIndexChanged(int)), this, SLOT(updateConfiguration(int)));
	connect(ui.circuitToSimulate, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(selectedComponets(const QString&)));
}

MainWidget::~MainWidget()
{
	delete future;
	delete watcher;
}

void MainWidget::startSimulationAsync() 
{
	ui.progressBar->setVisible(true);
	*future = QtConcurrent::run(this, &MainWidget::startSimulation);
	watcher->setFuture(*future);
}

void MainWidget::startSimulation()
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
	matlabPtr->eval(u"print('SimulationOutput/vdpSimulation','-dpng')");
}

void MainWidget::buildComponentMap()
{
	componentMap.insert(std::pair<QString,CircuitComponents>("R", CircuitComponents::R));
	componentMap.insert(std::pair<QString,CircuitComponents>("RL", CircuitComponents::RL));
	componentMap.insert(std::pair<QString,CircuitComponents>("RC", CircuitComponents::RC));
	componentMap.insert(std::pair<QString,CircuitComponents>("RLC", CircuitComponents::RLC));
}

// Todo: Scale Images without the size drift and dynamic sizes
void MainWidget::showCircuit(const QString &resource)
{
	circuitImage = ResourceManager::loadImage(this, resource);
	circuitImage = circuitImage.scaled(390,294, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.circuitsPreview->setPixmap(QPixmap::fromImage(circuitImage));
}

void MainWidget::showOutput()
{
	simulationImage = ResourceManager::loadImage(this, "SimulationOutput/vdpSimulation.png");
	simulationImage = simulationImage.scaled(390, 240, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.progressBar->setVisible(false);
	ui.simulationOutput->setPixmap(QPixmap::fromImage(simulationImage));
}

void MainWidget::selectedComponets(const QString &text)
{
	selectedComponents = componentMap[text];
	QString resource(":/MainWidget/Resources/");
	switch (selectedComponents) {
	case CircuitComponents::R:
		resource.append("AC_R_circuit.png");
		break;
	case CircuitComponents::RL:
		if (selectedConfiguration == CircuitConfiguration::SERIES)
			resource.append("AC_RL_series_circuit.png");
		else if (selectedConfiguration == CircuitConfiguration::PARALLEL)
			resource.append("AC_RL_parallel_circuit.png");
		break;
	case CircuitComponents::RC:
		if (selectedConfiguration == CircuitConfiguration::SERIES)
			resource.append("AC_RC_series_circuit.png");
		else if (selectedConfiguration == CircuitConfiguration::PARALLEL)
			resource.append("AC_RC_parallel_circuit.png");
		break;
	case CircuitComponents::RLC:
		if (selectedConfiguration == CircuitConfiguration::SERIES)
			resource.append("AC_RLC_series_circuit.png");
		else if (selectedConfiguration == CircuitConfiguration::PARALLEL)
			resource.append("AC_RLC_parallel_circuit.png");
		break;
	}
	showCircuit(resource);
}

void MainWidget::updateConfiguration(int index)
{
	switch (index) {
	case 0:
		selectedConfiguration = CircuitConfiguration::SERIES;
		break;
	case 1:
		selectedConfiguration = CircuitConfiguration::PARALLEL;
	}
	// Update the circuit preview to match new selection
	selectedComponets(ui.circuitToSimulate->itemText(ui.circuitToSimulate->currentIndex()));
}
