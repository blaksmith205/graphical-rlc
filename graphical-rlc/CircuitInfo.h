#pragma once

// The user selected circuit configuration
enum class CircuitConfiguration {
	SERIES,
	PARALLEL
};

// Components insde of an RLC circuit
enum class CircuitComponent {
	R,
	RL,
	RC,
	RLC,
	L,
	C,
	LC
};

// Input signal behavior
enum class InputSignal {
	SINUSOIDAL,
	SQUARE,
	DC
};

// Circuits related units
enum class CircuitUnit {
	OHM,
	HENRY,
	FARARDS,
	VOLTS,
	VOLTSP_P,
	AMPS,
	HERTZ,
	RAD_PER_SEC,
	RADIANS,
	DEGREES,
};

// Common multipliers found with circuit components
enum class CircuitComponetScale {
	NANO,
	MICRO,
	MILLI,
	BASE,
	KILO,
	MEGA,
	GIGA
};