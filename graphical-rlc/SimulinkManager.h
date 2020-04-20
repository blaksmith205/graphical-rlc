#pragma once
#include "stdafx.h"
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include "CircuitData.h"

class SimulinkManager {
public:
	SimulinkManager() : engine(matlab::engine::connectMATLAB()) {};
	void loadModel(const std::u16string& modelName);
	void loadModelAndSimulate(const std::u16string& modelName);
	void saveResults(const std::u16string& outputName);
	void setVariables(const std::shared_ptr<CircuitData>& data);
	void setSimulationParameters();
	inline void setSimulationParameters(const matlab::data::StructArray& structArray);
	inline void startSimulation();
private:
	std::unique_ptr<matlab::engine::MATLABEngine> engine;
};