#pragma once

#include <QWidget>
#include "ui_ImageDisplay.h"

class ImageDisplay : public QWidget
{
	Q_OBJECT

public:
	ImageDisplay(QWidget *parent = Q_NULLPTR);
	~ImageDisplay();

private:
	Ui::ImageDisplay ui;

private slots:
	void updateComponents();
};
