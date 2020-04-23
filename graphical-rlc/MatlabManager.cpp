#include "stdafx.h"
#include "MatlabManager.h"
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include <Windows.h>

using namespace matlab::engine;

void MatlabManager::call(const std::u16string& func, const std::vector<matlab::data::Array>& args)
{
}

std::vector<matlab::data::Array> MatlabManager::calcTransient(CircuitData* data, const std::u16string& outputName, 
	const std::shared_ptr<std::basic_stringbuf<char16_t>>& output, const std::shared_ptr<std::basic_stringbuf<char16_t>>& error)
{
	// Add the models to the path
	matlab::data::ArrayFactory factory;
	auto path = factory.createCharArray("models/transient");
	engine->feval(u"addpath", path);
	try
	{
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
		return engine->feval(u"calculate_transient", 5, args, output, error);
	}
	catch (matlab::engine::EngineException e)
	{
		qDebug() << e.what() << "\n";
	}
	return {};
}
