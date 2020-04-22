#include "stdafx.h"
#include "MatlabManager.h"
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include <Windows.h>

using namespace matlab::engine;

typedef std::basic_stringbuf<char16_t> StringBuf;

void MatlabManager::call(const std::u16string& func, const std::vector<matlab::data::Array>& args)
{
}

std::vector<matlab::data::Array> MatlabManager::calcTransient(CircuitData* data, const std::u16string& outputName)
{
	auto output = std::make_shared<StringBuf>();
	auto error = std::make_shared<StringBuf>();
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
	
	std::vector<matlab::data::Array> result;
	// call the function
	try
	{
		result = engine->feval(u"calculate_transient", 5, args, output, error);
	}
	catch (matlab::engine::EngineException e)
	{
		qDebug() << e.what() << "\n";
	}
	//String output_ = output.get()->str();
	//String error_ = error.get()->str();
	//qDebug() << convertUTF16StringToUTF8String(output_).c_str() << "\n";
	//qWarning() << convertUTF16StringToUTF8String(error_).c_str() << "\n";
	return result;
}
