#pragma once

// The user selected circuit configuration
enum class CircuitConfiguration {
	SERIES,
	PARALLEL
};

// The user selected circuit components to simulate. In order of the ComboBox
enum class CircuitComponents {
	R,
	RL,
	RC,
	RLC
};