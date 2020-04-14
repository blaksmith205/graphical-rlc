#pragma once

#include <QWidget>
#include "ui_CircuitOptions.h"
#include "CircuitData.h"

class CircuitOptions : public QWidget
{
	Q_OBJECT

public:
	CircuitOptions(std::shared_ptr<CircuitData> data, QWidget *parent = Q_NULLPTR);

private:
	std::shared_ptr<CircuitData> circuitData;
	Ui::CircuitOptions ui;

private slots:
	void updateCircuitConfig(int index);

signals:
	void updateProgressBar(int visible);
};
