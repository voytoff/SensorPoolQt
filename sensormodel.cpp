#include "sensormodel.h"

SensorModel::SensorModel(QObject *parent)
    : QAbstractTableModel(parent) {
}
SensorModel::SensorModel(const QList<Sensor> &sensors, QObject *parent)
    : QAbstractTableModel(parent), sensors(sensors) {
}
