#pragma once

#include <QtWidgets/QWidget>
#include "ui_mainwidget.h"

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = Q_NULLPTR);

private:
	Ui::MainWidgetClass ui;
};
