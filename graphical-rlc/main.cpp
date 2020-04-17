#include "stdafx.h"
#include "CircuitData.h"
#include "MainWindow.h"
#include "ResourceManager.h"
#include <QtWidgets/QApplication>

// TODO: Add newest matlab/extern libs to program path
int main(int argc, char* argv[])
{
	if (ResourceManager::isMatlabInstalled())
	{
		// Set the program path to include the DLLs
	}
	std::shared_ptr<CircuitData> data = std::make_shared<CircuitData>();
	QApplication a(argc, argv);
	MainWindow w(data);
	w.show();
	return a.exec();
}
