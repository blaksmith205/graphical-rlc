#pragma once
#include "stdafx.h"
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include "CircuitData.h"
class MatlabManager {
public:
	MatlabManager() : engine(matlab::engine::connectMATLAB()) {};
	void call(const std::u16string& func, const std::vector<matlab::data::Array>& args);
	matlab::data::StructArray calcTransient(CircuitData* data, const std::u16string& outputName);
private:
	std::unique_ptr<matlab::engine::MATLABEngine> engine;
};

