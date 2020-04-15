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
	// Custom left and right widgets
	CircuitOptions* circuitsOptions;
	ImageDisplay* outputImages;

	QFuture<void>* future;
	QFutureWatcher<void>* watcher;
	Ui::MainWindowClass ui;

	// Can't remove this because app displaying breaks
	std::map<QString, Circuit::Components> componentMap;
private slots:
	void startSimulationAsync();
	void updateProgressBar(int visible);
};


