#pragma once

#include <QWidget>
#include "ui_CircuitOptions.h"

class CircuitOptions : public QWidget
{
	Q_OBJECT

public:
	CircuitOptions(QWidget *parent = Q_NULLPTR);
	~CircuitOptions();

private:
	Ui::CircuitOptions ui;

private slots:
	void updateCircutConfig(int index);
};
