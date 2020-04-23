#include "stdafx.h"
#include "MainWindow.h"

MainWindow::MainWindow(std::shared_ptr<CircuitData> data, QWidget* parent)
	: QMainWindow(parent), circuitsOptions(new CircuitOptions(data, ui.mainLeft)), outputImages(new TransientDisplay(data, ui.mainRight))
{
	ui.setupUi(this);
	ui.progressBar->setVisible(false);
	ui.mainLeft->setLayout(circuitsOptions->layout());
	ui.mainRight->setLayout(outputImages->layout());
	// Connect the events to the proper handlers
	connect(circuitsOptions, SIGNAL(loadingChanged(int)), this, SLOT(updateProgressBar(int)));
	connect(outputImages, SIGNAL(loadingChanged(int)), this, SLOT(updateProgressBar(int)));
	connect(circuitsOptions, SIGNAL(simulationOutputChanged(const QString&)), outputImages, SLOT(showOutput(const QString&)));
	connect(circuitsOptions, SIGNAL(transientClicked()), outputImages, SLOT(calcTransientAsync()));

	// Setup menu
	createActions();
	createMenus();
}

void MainWindow::createActions()
{
	// About menu actions
	aboutAct = new QAction(tr("&About"), this);
	connect(aboutAct, &QAction::triggered, this, &MainWindow::help);

	aboutQtAct = new QAction(tr("About &Qt"), this);
	connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::createMenus()
{
	// Help menu button
	ui.menuHelp->addAction(aboutAct);
	ui.menuHelp->addAction(aboutQtAct);
}

void MainWindow::help()
{
	QMessageBox::about(this, tr("About GraphicalRLC"),
		tr("<p>This application provides a way to calculate and display the "
			"transient response for RLC circuits with a DC source.</p>"
			"<p> App version: 1.1</p>"));
}

// 0 means not visible
void MainWindow::updateProgressBar(int visible)
{
	ui.progressBar->setVisible(visible != 0);
}