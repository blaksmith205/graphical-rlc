#include "stdafx.h"
#include <QtConcurrent>
#include "MainWindow.h"
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include "ResourceManager.h"

MainWindow::MainWindow(std::shared_ptr<CircuitData> data, QWidget* parent)
	: QMainWindow(parent), circuitsOptions(new CircuitOptions(data, ui.mainLeft)), outputImages(new ImageDisplay(data, ui.mainRight))
{
	ui.setupUi(this);
	ui.progressBar->setVisible(false);
	future = new QFuture<void>();
	watcher = new QFutureWatcher<void>();
	ui.mainLeft->setLayout(circuitsOptions->layout());
	ui.mainRight->setLayout(outputImages->layout());
	// Connect the events to the proper handlers
	connect(circuitsOptions, SIGNAL(loadingChanged(int)), this, SLOT(updateProgressBar(int)));
	connect(outputImages, SIGNAL(loadingChanged(int)), this, SLOT(updateProgressBar(int)));
	//connect(ui.simulateButton, SIGNAL(clicked()), this, SLOT(startSimulationAsync()));
	//connect(watcher, SIGNAL(finished()), this, SLOT(showOutput()));
}

MainWindow::~MainWindow()
{
	delete future;
	delete watcher;
}

void MainWindow::startSimulationAsync() 
{
	ui.progressBar->setVisible(true);
	*future = QtConcurrent::run(this, &MainWindow::startSimulation);
	watcher->setFuture(*future);
}

void MainWindow::startSimulation()
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

// 0 means not visible
void MainWindow::updateProgressBar(int visible) {
	ui.progressBar->setVisible(visible!=0);
}