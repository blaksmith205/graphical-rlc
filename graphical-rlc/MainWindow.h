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
	void createActions();
	void createMenus();

	// Custom left and right widgets
	CircuitOptions* circuitsOptions;
	TransientDisplay* outputImages;

	Ui::MainWindowClass ui;

	// Menu related
	QAction* aboutAct;
	QAction* aboutQtAct;
	// Can't remove this because app displaying breaks
	std::map<QString, Circuit::Components> componentMap;
private slots:
	void help();
	void updateProgressBar(int visible);
};


