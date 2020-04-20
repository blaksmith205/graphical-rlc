#pragma once

#include <QWidget>
#include "ui_TransientDisplay.h"
#include "CircuitData.h"

class TransientDisplay : public QWidget {
	Q_OBJECT

public:
	TransientDisplay(std::shared_ptr<CircuitData> data, QWidget* parent = Q_NULLPTR);

private:
	void showPreview(const QString& resource);

	QImage circuitImage;
	QImage simulationImage;
	std::shared_ptr<CircuitData> circuitData;
	Ui::TransientDisplay ui;
private slots:
	void updateComponents(const QString& text);
	void updateCircuitPreview();
	void showOutput(const QString& simulationOutput);

signals:
	void loadingChanged(int visible);
};
