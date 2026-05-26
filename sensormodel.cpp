#include "sensormodel.h"
#include <QDataStream>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QList>
#include <QMessageBox>

SensorModel::SensorModel(QObject *parent)
  : QAbstractTableModel(parent)
{}
SensorModel::SensorModel(const QList<Sensor> &sensors, QObject *parent)
  : QAbstractTableModel(parent)
  , sensors(sensors)
{}

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

  if (role == Qt::DisplayRole) {
    const auto &sensor = sensors.at(index.row());

    switch (index.column()) {
    case -1:
      return QString("%1:%2").arg(sensor.sensorHost).arg(sensor.sensorPort);
    case 0:
      return sensor.oid;
    case 1:
      return sensor.name;
    case 2:
      return sensor.active;
    case 3:
      return sensor.sensorHost;
    case 4:
      return sensor.sensorPort;
    //case 5:
    //  return sensor.SensorConverter;
    case 6:
      return sensor.channelName;
    case 7:
      return sensor.description;
    case 8:
      return sensor.unit;
    case 9:
      return sensor.quantity;
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
      return tr("Активный");
    case 3:
      return tr("Сетевой адрес");
    case 4:
      return tr("Порт");
    case 5:
      return tr("Конвертер");
    case 6:
      return tr("Имя канала");
    case 7:
      return tr("Описание");
    case 8:
      return tr("Единица измерения");
    case 9:
      return tr("Частота");
    default:
      break;
    }
  }
  return QVariant();
}

bool SensorModel::visible(int col) const
{
  switch (col) {
  case 0:
    return false;
  case 1:
    return true;
  case 2:
    return false;
  case 3:
    return true;
  case 4:
    return true;
  case 5:
    return false;
  case 6:
    return true;
  case 7:
    return false;
  case 8:
    return false;
  case 9:
    return true;
  default:
    break;
  }
  return false;
}

QModelIndex SensorModel::ind(int row) const
{
  return index(row, 0, QModelIndex());
}

QModelIndex SensorModel::last() const
{
  return index(count() - 1, 0, QModelIndex());
}

QModelIndex SensorModel::first() const
{
  return index(0, 0, QModelIndex());
}

int SensorModel::count() const
{
  return sensors.size();
}

Sensor *SensorModel::get(int row)
{
  if (row >= sensors.size() || row < 0)
    return nullptr;
  Sensor &sensor = sensors[row]; //.at(row);
  return &sensor;
}

void SensorModel::add(Sensor sensor) {
  if (sensor.valid()) {
    sensors.append(sensor);
    layoutChanged();
  }
}

void SensorModel::replace(int row, const Sensor sensor)
{
  sensors.replace(row, sensor);
}

int SensorModel::read()
{
  sensors.clear();
  QFile file(getDbName());
  if (file.exists()) {
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qWarning() << "file open error:" << file.error();
      throw file.error();
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

      for (const QJsonValue &value : std::as_const(jsonArray)) {
        Sensor sensor;
        sensor.fromJson(value.toObject());
        add(sensor);
      }
    }
  }

  return EXIT_SUCCESS;
}

void SensorModel::write()
{
  QFile file(getDbName());
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QJsonArray jsonArray;
    for (auto &sensor : sensors) {
      QJsonObject obj = sensor.toJson();
      jsonArray.append(obj);
    }

    QJsonDocument doc(jsonArray);
    file.write(doc.toJson(QJsonDocument::Compact));
    file.close();
    qDebug() << "Successfully saved array to JSON file.";
  } else {
    qCritical() << "Could not open file for writing:" << file.errorString();
  }
}

QString SensorModel::getDbName()
{
  auto result = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir().mkpath(result);
  return result + "/db.json";
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
  int quantity)
{
  Sensor sensor;
  sensor.oid = oid;
  sensor.name = name;
  sensor.active = active;
  sensor.sensorHost = sensorHost;
  sensor.sensorPort = sensorPort;
   // sensorConverter,
  sensor.channelName = channelName;
  sensor.description = description;
  sensor.unit = unit;
  sensor.quantity = quantity;
  add(sensor);
  return sensor;
}
