#pragma once
#include "stdafx.h"
class ResourceManager {
public:
	static const QString ini_MatlabRootKey;
	static const QString matlabDllPath;
	static QImage loadImage(QWidget* parent, const QString& filename);
	static void updateConfig(const QString& key, const QString val, bool syncNow = true);
	static bool isMatlabInstalled();
private:
	static const QDir* defaultMatlabRoot;
	static QSettings* config;
};

