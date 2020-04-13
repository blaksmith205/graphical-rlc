#include "stdafx.h"
#include "CircuitData.h"
#include "MainWindow.h"
#include <QtWidgets/QApplication>

// TODO: Check to make sure the user has matlab installed
// TODO: Add newest matlab/extern libs to program path
int main(int argc, char *argv[])
{
	std::shared_ptr<CircuitData> data = std::make_shared<CircuitData>();
	QApplication a(argc, argv);
	MainWindow w(data);
	w.show();
	return a.exec();
}
