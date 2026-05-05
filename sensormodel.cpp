#include "sensormodel.h"
#include <QDataStream>
#include <QList>
#include <QFile>
#include <QMessageBox>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

SensorModel::SensorModel(QObject *parent)
    : QAbstractTableModel(parent) {
}
SensorModel::SensorModel(const QList<Sensor> &sensors, QObject *parent)
    : QAbstractTableModel(parent), sensors(sensors) {
}

int SensorModel::rowCount(const QModelIndex &parent) const
{
  return parent.isValid() ? 0 : sensors.size();
}

int SensorModel::columnCount(const QModelIndex &parent) const
{
  return parent.isValid() ? 0 : 5;
}

QVariant SensorModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= sensors.size() || index.row() < 0) return QVariant();

  if (role == Qt::DisplayRole) {
    const auto &sensor = sensors.at(index.row());

    switch (index.column()) {
    case 0:
      return sensor.Active;
    case 1:
      return sensor.Name;
    case 2:
      return QString("%1:%2").arg(sensor.SensorHost).arg(sensor.SensorPort);
    case 3:
      return sensor.ChannelName;
    case 4:
      return sensor.Quantity;
    default:
      break;
    }
  }
  return QVariant();
}
QVariant SensorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return tr("#");
    case 1:
      return tr("Название");
    case 2:
      return tr("Адрес");
    case 3:
      return tr("Имя канала");
    case 4:
      return tr("Частота");
    default:
      break;
    }
  }
  return QVariant();
}

const Sensor* SensorModel::get(const int row)
{
  if (row >= sensors.size() || row < 0) return nullptr;
  const auto &sensor = sensors.at(row);
  return &sensor;
}

int SensorModel::readFromFile()
{
  QFile file(":/db.json");

  if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
    throw file.error();
  }

  sensors.clear();
  // 1 Binnary
  //QDataStream in(&file);
  //in >> sensors;

  // 2. Json
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

    for (const QJsonValue &value : std::as_const(jsonArray)) {
      QJsonObject obj = value.toObject();
      Sensor sensor;
      obj >> sensor;
      addEntry(sensor);
    }
  }

  return EXIT_SUCCESS;
}

void SensorModel::addEntry(const Sensor &sensor)
{
  sensors.append(sensor);
}

Sensor SensorModel::addEntry(
  QUuid oid,
  QString name,
  bool active,
  QString sensorHost,
  int sensorPort,
  QString sensorConverter,
  QString channelName,
  QString description,
  QString unit,
  int quantity
  ) {
  Sensor sensor {oid, name, active, sensorHost, sensorPort, sensorConverter, channelName, description, unit, quantity};
  addEntry(sensor);
  return sensor;
}

