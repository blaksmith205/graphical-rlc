#pragma once

namespace Circuit
{
	// The user selected circuit configuration
	enum class Configuration {
		SERIES,
		PARALLEL
	};

	// Components insde of an RLC circuit
	enum class Components {
		NONE,
		R,
		RL,
		RC,
		RLC,
		L,
		C,
		LC,
	};

	// Input signal behavior
	enum class InputSignal {
		SINUSOIDAL,
		SQUARE,
		DC
	};

	// Circuits related units
	enum class Units {
		NONE,
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
	enum class ComponetScale {
		NANO,
		MICRO,
		MILLI,
		BASE,
		KILO,
		MEGA,
		GIGA
	};

	struct CircuitComponent {
		CircuitComponent() :component(Circuit::Components::NONE), value(0.0), unit(Circuit::Units::NONE) {};
		CircuitComponent(Circuit::Components component, Circuit::Units unit) : component(component), value(0.0), unit(unit) {};
		CircuitComponent(Circuit::Components component, double val, Circuit::Units unit) : component(component), value(val), unit(unit){};
		inline bool operator==(const Circuit::CircuitComponent& other) { return (component == other.component) && (value == other.value) && (unit == other.unit); };
		inline bool operator!=(const Circuit::CircuitComponent& other) { return !(*this == other); };
		Circuit::Components component;
		double value;
		Circuit::Units unit;
	};
}