#pragma once

#include <QtWidgets/QWidget>
#include "ui_MainWindow.h"
#include "CircuitData.h"
#include "CircuitOptions.h"
#include "TransientDisplay.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(std::shared_ptr<CircuitData> data, QWidget* parent = Q_NULLPTR);
private:
	// Custom left and right widgets
	CircuitOptions* circuitsOptions;
	TransientDisplay* outputImages;

	Ui::MainWindowClass ui;

	// Can't remove this because app displaying breaks
	std::map<QString, Circuit::Components> componentMap;
private slots:
	void updateProgressBar(int visible);
};


