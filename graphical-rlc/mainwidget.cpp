#include "stdafx.h"
#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.progressBar->setVisible(false);
}
