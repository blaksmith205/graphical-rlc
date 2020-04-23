#pragma once

namespace Circuit
{
	// The user selected circuit configuration
	enum class Configuration {
		SERIES,
		PARALLEL
	};

	// The user selected response type
	enum class Response {
		NATURAL,
		STEP
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
		VOLTS, // Amplitude
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
		CircuitComponent(Circuit::Components component, double val, Circuit::Units unit) : component(component), value(val), unit(unit) {};
		inline bool operator==(const Circuit::CircuitComponent& other) { return (component == other.component) && (value == other.value) && (unit == other.unit); };
		inline bool operator!=(const Circuit::CircuitComponent& other) { return !(*this == other); };
		Circuit::Components component;
		double value;
		Circuit::Units unit;
	};

	static std::map<Circuit::ComponetScale, const double> scaleMap = { {Circuit::ComponetScale::NANO, 1e-9}, {Circuit::ComponetScale::MICRO, 1e-6},
	{Circuit::ComponetScale::MILLI, 1e-3}, {Circuit::ComponetScale::BASE, 1}, {Circuit::ComponetScale::KILO, 1e+3}, {Circuit::ComponetScale::MEGA, 1e+6},
	{Circuit::ComponetScale::GIGA, 1e+9} };

	// ComboBox unit mapped to respective unit. Can't figure out how to store Ω
	static std::map<const QString, const Circuit::Units> unitMap = { {"H", Circuit::Units::HENRY},
		{"F", Circuit::Units::FARARDS}, {"V", Circuit::Units::VOLTS}, {"Vpp", Circuit::Units::VOLTSP_P},
		{"Hz", Circuit::Units::HERTZ}, {"rad/s", Circuit::Units::RAD_PER_SEC}, {"radians", Circuit::Units::RADIANS},
		{"degrees", Circuit::Units::DEGREES} };

	// Component Strings mapped to respective components
	static std::map<const QString, const Circuit::Components> componentMap = { { "R", Circuit::Components::R }, { "L", Circuit::Components::L },
	{ "C", Circuit::Components::C }, { "RL", Circuit::Components::RL },{ "RC", Circuit::Components::RC },
	{ "RLC", Circuit::Components::RLC }, {"LC", Circuit::Components::LC}, {"", Circuit::Components::NONE} };

	// Signal strings to respective input signal
	static std::map<const QString, const Circuit::InputSignal> inputSignalMap = { { "Sinusoidal", Circuit::InputSignal::SINUSOIDAL }, { "Square", Circuit::InputSignal::SQUARE },
		{"DC", Circuit::InputSignal::DC} };
}