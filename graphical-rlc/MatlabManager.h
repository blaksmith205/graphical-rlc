#pragma once
#include "stdafx.h"
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include "CircuitData.h"
class MatlabManager {
public:
	MatlabManager() : engine(matlab::engine::connectMATLAB()) {};
	void call(const std::u16string& func, const std::vector<matlab::data::Array>& args);
	std::vector<matlab::data::Array> calcTransient(CircuitData* data, const std::u16string& outputName);
	inline matlab::data::Array getProperty(const matlab::data::Array& objectArray, size_t index, const matlab::engine::String& propertyName) { return engine->getProperty(objectArray, index, propertyName); };
	inline matlab::data::Array getProperty(const matlab::data::Array& objectArray, const std::string& propertyName) { return engine->getProperty(objectArray, propertyName); };
private:
	std::unique_ptr<matlab::engine::MATLABEngine> engine;
};

