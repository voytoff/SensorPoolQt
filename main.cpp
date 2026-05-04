#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QStandardItemModel>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile file(":/db.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
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

    if (doc.isArray()) {
        QJsonArray jsonArray = doc.array();
        QStandardItemModel *model = new QStandardItemModel();

        for (const QJsonValue &value : std::as_const(jsonArray)) {
            QJsonObject obj = value.toObject();
            QList<QStandardItem *> row;
            row << new QStandardItem(obj["Name"].toString());
            //row << new QStandardItem(QUuid(obj["Oid"].toString()));
            model->appendRow(row);
        }
    }

    MainWindow window("artists");
    window.show();
    return app.exec();
}
