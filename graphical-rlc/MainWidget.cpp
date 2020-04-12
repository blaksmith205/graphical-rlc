#include "stdafx.h"
#include <QtConcurrent>
#include "mainwidget.h"
#include "MatlabSimulator.h"
#include "ResourceHelper.h"

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.progressBar->setVisible(false);
	future = new QFuture<void>();
	watcher = new QFutureWatcher<void>();
	connect(ui.simulateButton, SIGNAL(clicked()), this, SLOT(startSimulationAsync()));
	connect(watcher, SIGNAL(finished()), this, SLOT(showImage()));
}

MainWidget::~MainWidget()
{
	delete future;
	delete watcher;
}

void MainWidget::startSimulationAsync() 
{
	ui.progressBar->setVisible(true);
	*future = QtConcurrent::run(this, &MainWidget::startSimulation);
	watcher->setFuture(*future);
}

void MainWidget::startSimulation()
{
	MatlabSimulator simulator(u"vdp");
	simulator.loadModel();
	simulator.runSimulation();
	simulator.saveResult();
}

void MainWidget::showImage()
{
	simulationImage = ResourceHelper::loadImage(this, "F:/blaks/Documents/Coding/C++/graphical-rlc/graphical-rlc/SimulationOutput/vdpSimulation.png");
	simulationImage = simulationImage.scaled(ui.simulationOutput->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.progressBar->setVisible(false);
	ui.simulationOutput->setPixmap(QPixmap::fromImage(simulationImage));
}
