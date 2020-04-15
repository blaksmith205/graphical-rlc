#include "stdafx.h"
#include "ResourceManager.h"

// Variable Init
const QString ResourceManager::ini_MatlabRootKey = QStringLiteral("Matlab/rootdir");
const QString ResourceManager::matlabDllPath = QStringLiteral("/extern/bin/win64");
const QDir* ResourceManager::defaultMatlabRoot = new QDir("C:/Program Files/MATLAB");
QSettings* ResourceManager::config = new QSettings(QDir::currentPath() + "/GraphicalRLC.ini", QSettings::Format::IniFormat);

QImage ResourceManager::loadImage(QWidget *parent, const QString& filename)
{
    QImageReader reader(filename);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(parent, QGuiApplication::applicationDisplayName(),
            QObject::tr("Cannot load %1: %2")
            .arg(QDir::toNativeSeparators(filename), reader.errorString()));
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
    if (ResourceManager::config->contains(ResourceManager::ini_MatlabRootKey)) return true;

    // First check if installed in default location and the extern directory is available
    if (ResourceManager::defaultMatlabRoot->exists()) {
        // Obtain the child directories from newest to oldest
        QStringList subFiles = ResourceManager::defaultMatlabRoot->entryList(QDir::Filter::NoDotAndDotDot | QDir::Filter::Dirs, QDir::SortFlag::Reversed);
        for (auto subFile : subFiles) {
            auto exp = matlabRe.match(subFile);
            if (exp.isValid())
            {
                QDir latestVersion = QDir(ResourceManager::defaultMatlabRoot->absolutePath() + QDir::separator() + subFile);
                if (latestVersion.exists(latestVersion.absolutePath() + ResourceManager::matlabDllPath))
                {
                    // Update config to have the latest root dir
                    ResourceManager::updateConfig(ResourceManager::ini_MatlabRootKey, latestVersion.absolutePath());
                    return true;
                }
            }
        }
    }

    // Can't find Matlab in default location
    return false;
}
