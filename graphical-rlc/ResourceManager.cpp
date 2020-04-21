#include "stdafx.h"
#include "ResourceManager.h"

// Variable Init
const QString ResourceManager::ini_MatlabRootKey = QStringLiteral("Matlab/rootdir");
const QString ResourceManager::matlabWindowsDLLSubDir = QStringLiteral("/extern/bin/win64");
const QDir* ResourceManager::defaultMatlabRoot = new QDir("C:/Program Files/MATLAB");
QSettings* ResourceManager::config = new QSettings(QDir::currentPath() + "/GraphicalRLC.ini", QSettings::Format::IniFormat);

QImage ResourceManager::loadImage(QWidget* parent, const QString& filename)
{
	QImageReader reader(filename);
	reader.setAutoTransform(true);
	const QImage newImage = reader.read();
	if (newImage.isNull())
	{
		QMessageBox::information(parent, QGuiApplication::applicationDisplayName(),
			QObject::tr("Cannot load %1: %2").arg(QDir::toNativeSeparators(filename), reader.errorString()));
		return QImage();
	}

	return newImage;
}

void ResourceManager::updateConfig(const QString& key, const QString val, bool syncNow)
{
	config->setValue(key, val);
	if (syncNow)
	{
		config->sync();
	}
}

bool ResourceManager::isMatlabInstalled()
{
	QRegularExpression matlabRe("/(R{1}20)\\w+/i");
	// First check if the location was setbefore:
	if (config->contains(ini_MatlabRootKey) && isMatlabPathValid(config->value(ini_MatlabRootKey).toString())) return true;

	// First check if installed in default location and the extern directory is available
	if (defaultMatlabRoot->exists())
	{
		// Obtain the child directories from newest to oldest
		QStringList subFiles = defaultMatlabRoot->entryList(QDir::Filter::NoDotAndDotDot | QDir::Filter::Dirs, QDir::SortFlag::Reversed);
		for (auto subFile : subFiles)
		{
			auto exp = matlabRe.match(subFile);
			if (exp.isValid())
			{
				QDir latestVersion = QDir(defaultMatlabRoot->absolutePath() + QDir::separator() + subFile);
				if (isMatlabPathValid(latestVersion.absolutePath()))
				{
					// Update config to have the latest root dir
					updateConfig(ini_MatlabRootKey, latestVersion.absolutePath());
					return true;
				}
			}
		}
	}

	// Can't find Matlab in default location
	return false;
}

bool ResourceManager::isMatlabPathValid(const QString& path)
{
	QDir dir(path);
	return dir.exists(dir.absolutePath() + QDir::separator() + matlabWindowsDLLSubDir);
}

const QByteArray ResourceManager::getEnvPath()
{
	QString path = qgetenv("PATH") + ";";
	QDir dllPath = getMatlabFullDLLPath();
	path += dllPath.toNativeSeparators(dllPath.absolutePath());
	return path.toUtf8();
}

const QString ResourceManager::getMatlabFullDLLPath()
{
	return config->value(ini_MatlabRootKey).toString() + matlabWindowsDLLSubDir;
}

void ResourceManager::createOutputDir()
{
	QDir outputDir("./generated");
	if (!outputDir.exists())
	{
		outputDir.mkpath(".");
	}
}

void ResourceManager::askForMatlabRoot()
{
	QDir selectedRoot;
	int ret = QMessageBox::warning(nullptr, QObject::tr("Missing Matlab"), QObject::tr("Root folder for MATLAB could not be found.\nPlease select the MATLAB version.\n\nExample: C:/Program Files/MATLAB/R2020a"), QMessageBox::Open, QMessageBox::Cancel);
	switch (ret)
	{
	case QMessageBox::Cancel:
		break;
	case QMessageBox::Open:
		selectedRoot = QFileDialog::getExistingDirectory(nullptr, QObject::tr("Select MATLAB Version"),
			"/home",
			QFileDialog::ShowDirsOnly
			| QFileDialog::DontResolveSymlinks);
	}

	// Make sure the selected path is valid
	if (isMatlabPathValid(selectedRoot.absolutePath()))
	{
		updateConfig(ini_MatlabRootKey, selectedRoot.absolutePath());
	}
}
