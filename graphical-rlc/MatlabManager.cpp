#include "stdafx.h"
#include "MatlabManager.h"
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
using namespace matlab::engine;

static std::map<int, std::string> variableNames = { {0, "R"}, {1, "L"}, {2, "C"}, {3, "A"}, {4, "F"}, {5, "phase"}, {6, "offset"} };

void MatlabManager::loadModel(std::unique_ptr<matlab::engine::MATLABEngine>& engine, const std::u16string& modelName)
{
	// Set the model name in matlab's workspace
	matlab::data::ArrayFactory factory;
	auto name = factory.createCharArray(modelName);
	engine->setVariable("modelName", name);
	auto loadArgs = factory.createCharArray(modelName);
	// Load the model and wait for simulation
	engine->feval("load_system", loadArgs);
}

void MatlabManager::loadModelAndSimulate(std::unique_ptr<matlab::engine::MATLABEngine>& engine, const std::u16string& modelName)
{
	loadModel(engine, modelName);
	startSimulation(engine);
}

void MatlabManager::saveResults(std::unique_ptr<matlab::engine::MATLABEngine>& engine, const std::u16string& outputName)
{
	// Get simulation data and create a graph
	engine->eval(u"y = simOut.get('yOut');");
	engine->eval(u"t = simOut.get('tOut');");
	engine->eval(u"f = figure('visible','off'); plot(t,y);"); // Plot without showing the image
	std::u16string printCall = u"print('generated/" + outputName + u"','-dpng')";
	engine->eval(printCall);
}

void MatlabManager::setVariables(std::unique_ptr<MATLABEngine>& engine, const std::shared_ptr<CircuitData>& data)
{
	auto componentValues = data->componentValues();
	matlab::data::ArrayFactory factory;
	for (int i = 0; i < componentValues.size(); i++)
	{
		auto scalar = factory.createScalar<double>(componentValues[i].value);
		engine->setVariable(variableNames[i], scalar);
	}
}

void MatlabManager::setSimulationParameters(std::unique_ptr<matlab::engine::MATLABEngine>& engine)
{
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
	engine->setVariable(u"parameterStruct", parameterStruct);
}

void MatlabManager::setSimulationParameters(std::unique_ptr<matlab::engine::MATLABEngine>& engine, const matlab::data::StructArray& parameterStruct)
{
	// Put simulation parameter struct in MATLAB
	engine->setVariable(u"parameterStruct", parameterStruct);
}

void MatlabManager::startSimulation(std::unique_ptr<matlab::engine::MATLABEngine>& engine)
{
	engine->eval(u"simOut = sim(modelName, parameterStruct);");
}
