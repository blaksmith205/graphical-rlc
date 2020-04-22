#pragma once
#include "stdafx.h"
class ResourceManager {
public:
	static const QString ini_MatlabRootKey;
	static const QString matlabWindowsDLLSubDir;
	static QImage loadImage(QWidget* parent, const QString& filename);
	static void updateConfig(const QString& key, const QString val, bool syncNow = true);
	static bool isMatlabInstalled();
	static bool isMatlabPathValid(const QString& path);
	static const QByteArray getEnvPath();
	static const QString getMatlabFullDLLPath();
	static void createOutputDir();
	static void askForMatlabRoot();
	static QString validTransientOutputName(bool isSeries, bool isStep);
private:
	static const QDir* defaultMatlabRoot;
	static QSettings* config;
};

