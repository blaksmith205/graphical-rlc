#pragma once
#include "stdafx.h"
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"

class MatlabSimulator {
public:
	MatlabSimulator(const std::u16string &model);
	void loadModel();
	void runSimulation();
	std::u16string saveResult();
	inline std::shared_ptr<matlab::data::StructArray> getParamterStruct() { return parameterStruct; };
private:
	void setupParameters();

	std::unique_ptr<matlab::engine::MATLABEngine> matlabPtr;
	matlab::execution::FutureResult<void> loadFuture;
	matlab::execution::FutureResult<void> simFuture;
	std::shared_ptr<matlab::data::StructArray> parameterStruct;
	std::u16string model;
};

