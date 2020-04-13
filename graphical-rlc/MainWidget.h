#pragma once

#include <QtWidgets/QWidget>
#include "ui_mainwidget.h"
#include "MatlabManager.h"
#include "CircuitProperties.h"

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = Q_NULLPTR);
	~MainWidget();
	void startSimulation();
private:
	void buildComponentMap();
	void showCircuit(const QString& resource);
private:
	CircuitConfiguration selectedConfiguration;
	CircuitComponents selectedComponents;
	QImage circuitImage;
	QImage simulationImage;
	QFuture<void>* future;
	QFutureWatcher<void>* watcher;
	Ui::MainWidgetClass ui;

	// Text inside ComboBox mapped to CircuitComponents
	std::map<QString, CircuitComponents> componentMap;
private slots:
	void startSimulationAsync();
	void showOutput();
	void selectedComponets(const QString& text);
	void updateConfiguration(int index);
};


