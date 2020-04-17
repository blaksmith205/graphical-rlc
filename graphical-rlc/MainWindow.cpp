#include "stdafx.h"
#include "MainWindow.h"

MainWindow::MainWindow(std::shared_ptr<CircuitData> data, QWidget* parent)
	: QMainWindow(parent), circuitsOptions(new CircuitOptions(data, ui.mainLeft)), outputImages(new ImageDisplay(data, ui.mainRight))
{
	ui.setupUi(this);
	ui.progressBar->setVisible(false);
	ui.mainLeft->setLayout(circuitsOptions->layout());
	ui.mainRight->setLayout(outputImages->layout());
	// Connect the events to the proper handlers
	connect(circuitsOptions, SIGNAL(loadingChanged(int)), this, SLOT(updateProgressBar(int)));
	connect(outputImages, SIGNAL(loadingChanged(int)), this, SLOT(updateProgressBar(int)));
	connect(circuitsOptions, SIGNAL(simulationOutputChanged(const QString&)), outputImages, SLOT(showOutput(const QString&)));
}

// 0 means not visible
void MainWindow::updateProgressBar(int visible)
{
	ui.progressBar->setVisible(visible != 0);
}