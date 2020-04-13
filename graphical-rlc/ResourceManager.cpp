#include "stdafx.h"
#include "ResourceManager.h"

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