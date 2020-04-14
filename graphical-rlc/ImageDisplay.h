#pragma once

#include <QWidget>
#include "ui_ImageDisplay.h"
#include "CircuitData.h"

class ImageDisplay : public QWidget
{
	Q_OBJECT

public:
	ImageDisplay(std::shared_ptr<CircuitData> data, QWidget *parent = Q_NULLPTR);

private:
	void buildMaps();
	void showOutput(const QString& simulationOutput);
	void showPreview(const QString& resource);


	QImage circuitImage;
	QImage simulationImage;
	std::shared_ptr<CircuitData> circuitData;
	Ui::ImageDisplay ui;

	// Text inside ComboBox mapped to CircuitComponent
	std::map<QString, CircuitComponent> stringToComponentMap;
private slots:
	void updateComponents(const QString& text);
	void updateCircuitPreview();

signals:
	void updateProgressBar(int visible);
};
