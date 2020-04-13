#pragma once

#include <QtWidgets/QWidget>
#include "ui_MainWindow.h"
#include "CircuitData.h"
#include "CircuitOptions.h"
#include "ImageDisplay.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(std::shared_ptr<CircuitData> data, QWidget *parent = Q_NULLPTR);
	~MainWindow();
	void startSimulation();
private:
	void buildComponentMap();
	void showCircuit(const QString& resource);
private:
	std::shared_ptr<CircuitData> circuitData;
	CircuitComponent selectedComponents;
	// Custom left and right widgets
	CircuitOptions* circuitsOptions;
	ImageDisplay* outputImages;

	QImage circuitImage;
	QImage simulationImage;
	QFuture<void>* future;
	QFutureWatcher<void>* watcher;
	Ui::MainWindowClass ui;

	// Text inside ComboBox mapped to CircuitComponent
	std::map<QString, CircuitComponent> componentMap;
private slots:
	void startSimulationAsync();
	void showOutput();
	void selectedComponets(const QString& text);
};


