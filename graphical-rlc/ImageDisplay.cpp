#include "stdafx.h"
#include "ImageDisplay.h"

ImageDisplay::ImageDisplay(std::shared_ptr<CircuitData> data, QWidget *parent)
	: QWidget(parent), circuitData(data)
{
	ui.setupUi(this);
}

ImageDisplay::~ImageDisplay()
{
}

void ImageDisplay::updateComponents() {

}
