#include "stdafx.h"
#include "MatlabManager.h"
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include <Windows.h>

using namespace matlab::engine;

void MatlabManager::call(const std::u16string& func, const std::vector<matlab::data::Array>& args)
{
}

matlab::data::StructArray MatlabManager::calcTransient(CircuitData* data, const std::u16string& outputName)
{
	// Add the models to the path
	matlab::data::ArrayFactory factory;
	auto path = factory.createCharArray("models/transient");
	engine->feval(u"addpath", path);

	auto dataVals = data->componentValues();
	auto config = data->getConfig();
	auto conditions = data->initialConditions();
	// Setup the args
	std::vector<matlab::data::Array> args = {
		factory.createScalar<double>(dataVals[0].value), // R
		factory.createScalar<double>(dataVals[1].value), // L
		factory.createScalar<double>(dataVals[2].value), // C
		factory.createScalar<double>(conditions[0]), // y_init
		factory.createScalar<double>(conditions[1]), // dy_init
		factory.createScalar<double>(conditions[2]), // finalVal
		factory.createScalar<bool>(config == Circuit::Configuration::SERIES), // isSeries = false if config is parallel
		factory.createScalar<bool>(data->response == Circuit::Response::STEP), // isStep is false when response is natural
		factory.createCharArray(outputName)
	};
	
	// call the function
	matlab::data::StructArray results = engine->feval(u"calculate_transient", args);
	return results;
}
