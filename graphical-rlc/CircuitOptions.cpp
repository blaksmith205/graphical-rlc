#include "stdafx.h"
#include "CircuitOptions.h"

CircuitOptions::CircuitOptions(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

CircuitOptions::~CircuitOptions()
{
}

void CircuitOptions::updateCircutConfig(int index) {
	switch (index) {
	case 0:
		break;
	case 1:
		break;
	}
}
