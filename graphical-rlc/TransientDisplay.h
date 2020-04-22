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

	// Transient Calculation related
	QFuture<void>* future;
	QFutureWatcher<void>* watcher;
	QString outputName;
	QString response, equation;
	double wd;
	std::complex<double> s1, s2;
	QString complexFormat = QString("%1 %2 j%3");

	QImage circuitImage;
	QImage simulationImage;
	std::shared_ptr<CircuitData> circuitData;
	Ui::TransientDisplay ui;
private slots:
	void calcComplete();
	void calcTransientAsync();
	void calcTransient();
	void updateComponents(const QString& text);
	void updateResponse(const QString& text);
	void updateCircuitPreview();
	void showOutput(const QString& simulationOutput);
signals:
	void loadingChanged(int visible);
};
