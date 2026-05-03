#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    QFile file(":/db.json");
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        qWarning() << "Couldn't open file:" << ":/db.json";
        return EXIT_FAILURE;
    }
    QByteArray rawData = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(rawData, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << error.errorString();
        return EXIT_FAILURE;
    }

    if (doc.isObject()) {
        QJsonObject obj = doc.object();

        // Example: Reading a string value for key "appName"
        if (obj.contains("appName")) {
            QString name = obj["appName"].toString();
            qDebug() << "App Name:" << name;
        }
    }

    MainWindow window("artists");
    window.show();
    return app.exec();

}
