#include "stdafx.h"
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include "MatlabSimulator.h"

using namespace matlab::engine;

MatlabSimulator::MatlabSimulator(const std::u16string& model) : matlabPtr(startMATLAB()), model(model)
{
	setupParameters();
}

void MatlabSimulator::loadModel()
{
	// Load Simulink model
	loadFuture = matlabPtr->evalAsync(u"load_system('" + model + u"')");
	//std::cout << "Loading Simulink model... " << std::endl;
	std::future_status loadStatus;
	do {
		loadStatus = loadFuture.wait_for(std::chrono::seconds(1));
	} while (loadStatus != std::future_status::ready);
	//std::cout << "Model loaded\n";
}

void MatlabSimulator::setupParameters()
{
	// Create MATLAB data array factory
	matlab::data::ArrayFactory factory;

	// Create struct for simulation parameters
	parameterStruct = std::make_shared<matlab::data::StructArray>(factory.createStructArray({ 1,4 }, {
		"SaveOutput",
		"OutputSaveName",
		"SaveTime",
		"TimeSaveName" }));
	(*parameterStruct)[0]["SaveOutput"] = factory.createCharArray("on");
	(*parameterStruct)[0]["OutputSaveName"] = factory.createCharArray("yOut");
	(*parameterStruct)[0]["SaveTime"] = factory.createCharArray("on");
	(*parameterStruct)[0]["TimeSaveName"] = factory.createCharArray("tOut");
}

void MatlabSimulator::runSimulation()
{
	// Run simulation
	simFuture = matlabPtr->evalAsync(u"simOut = sim('" + model + u"',parameterStruct);");
	//std::cout << "Running simulation... " << std::endl;
	std::future_status simStatus;
	do {
		simStatus = loadFuture.wait_for(std::chrono::seconds(1));
	} while (simStatus != std::future_status::ready);
	//std::cout << "Simulation complete\n";
}

std::u16string MatlabSimulator::saveResult()
{
	// Get simulation data and create a graph
	matlabPtr->eval(u"y = simOut.get('yOut');");
	matlabPtr->eval(u"t = simOut.get('tOut');");
	matlabPtr->eval(u"f = figure('visible','off'); plot(t,y);"); // Plot without showing the image
	std::u16string outputName = model + u"-output";
	matlabPtr->eval(u"print('SimulationOutput/" + outputName + u"','-dpng')");
	return outputName;
}
