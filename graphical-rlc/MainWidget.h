#pragma once

#include <QtWidgets/QWidget>
#include "ui_mainwidget.h"

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = Q_NULLPTR);
	~MainWidget();
	void startSimulation();
private:
	QImage circuitImage;
	QImage simulationImage;
	QFuture<void>* future;
	QFutureWatcher<void>* watcher;
	Ui::MainWidgetClass ui;
private slots:
	void startSimulationAsync();
	void showImage();
};
