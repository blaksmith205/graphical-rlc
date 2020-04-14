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
	void buildMap();
	void saveData();

	std::shared_ptr<CircuitData> circuitData;
	Ui::CircuitOptions ui;
	std::map<QLineEdit*, CircuitData::Keys> fieldToDataMap;
private slots:
	void updateCircuitConfig(int index);
	void validateTextValue(const QString& text);

signals:
	void updateProgressBar(int visible);
};
