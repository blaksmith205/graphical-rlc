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
	connect(ui.simulateButton, SIGNAL(clicked()), this, SLOT(startSimulationAsync()));
	connect(watcher, SIGNAL(finished()), this, SLOT(showImage()));
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

void MainWidget::showImage()
{
	simulationImage = ResourceManager::loadImage(this, "SimulationOutput/vdpSimulation.png");
	simulationImage = simulationImage.scaled(ui.simulationOutput->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.progressBar->setVisible(false);
	ui.simulationOutput->setPixmap(QPixmap::fromImage(simulationImage));
}
