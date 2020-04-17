#pragma once
#include "stdafx.h"
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include "CircuitData.h"

class MatlabManager {
public:
	MatlabManager() : engine(matlab::engine::connectMATLAB()) {};
	inline void loadModel(const std::u16string& modelName) { loadModel(engine, modelName); };
	inline void loadModelAndSimulate(const std::u16string& modelName) { loadModelAndSimulate(engine, modelName); };
	inline void saveResults(const std::u16string& outputName) { saveResults(engine, outputName); };
	inline void setVariables(const std::shared_ptr<CircuitData>& data) { setVariables(engine, data); };
	inline void setSimulationParameters() { setSimulationParameters(engine); };
	inline void setSimulationParameters(const matlab::data::StructArray& structArray) { setSimulationParameters(engine, structArray); };
	inline void startSimulation() { startSimulation(engine); };
private:
	std::unique_ptr<matlab::engine::MATLABEngine> engine;
	// Static
	inline static std::unique_ptr<matlab::engine::MATLABEngine> getEngine() { return matlab::engine::connectMATLAB(); };
	static void loadModel(std::unique_ptr<matlab::engine::MATLABEngine>& engine, const std::u16string& modelName);
	static void loadModelAndSimulate(std::unique_ptr<matlab::engine::MATLABEngine>& engine, const std::u16string& modelName);
	static void saveResults(std::unique_ptr < matlab::engine::MATLABEngine>& engine, const std::u16string& outputName);
	static void setVariables(std::unique_ptr<matlab::engine::MATLABEngine>& engine, const std::shared_ptr<CircuitData>& data);
	static void setSimulationParameters(std::unique_ptr<matlab::engine::MATLABEngine>& engine);
	static void setSimulationParameters(std::unique_ptr<matlab::engine::MATLABEngine>& engine, const matlab::data::StructArray& structArray);
	static void startSimulation(std::unique_ptr<matlab::engine::MATLABEngine>& engine);
};