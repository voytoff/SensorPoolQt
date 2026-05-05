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
  return parent.isValid() ? 0 : 10;
}

QVariant SensorModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= sensors.size() || index.row() < 0)
    return QVariant();
  QUuid Oid;
  QString Name;
  bool Active;
  QString SensorHost;
  int SensorPort;
  QString SensorConverter;
  QString ChannelName;
  QString Description;
  QString Unit;
  int Quantity;


  if (role == Qt::DisplayRole) {
    const auto &contact = sensors.at(index.row());

    switch (index.column()) {
    case 0:
      return contact.Oid;
    case 1:
      return contact.Name;
    case 2:
      return contact.Active;
    case 3:
      return contact.SensorHost;
    case 4:
      return contact.SensorPort;
    case 5:
      return contact.SensorConverter;
    case 6:
      return contact.ChannelName;
    case 7:
      return contact.Description;
    case 8:
      return contact.Unit;
    case 9:
      return contact.Quantity;
    default:
      break;
    }
  }
  return QVariant();
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

