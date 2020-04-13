#include "stdafx.h"
#include "MainWindow.h"
#include <QtWidgets/QApplication>

// TODO: Check to make sure the user has matlab installed
// TODO: Add newest matlab/extern libs to program path
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
