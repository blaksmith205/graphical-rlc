#pragma once

#include <QWidget>
#include "ui_ImageDisplay.h"
#include "CircuitData.h"

class ImageDisplay : public QWidget
{
	Q_OBJECT

public:
	ImageDisplay(std::shared_ptr<CircuitData> data, QWidget *parent = Q_NULLPTR);
	~ImageDisplay();

private:
	std::shared_ptr<CircuitData> circuitData;
	Ui::ImageDisplay ui;

private slots:
	void updateComponents();
};
