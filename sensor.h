#ifndef SENSOR_H
#define SENSOR_H

#include "isensorconverter.h"
#include <QUuid>
#include <QJsonObject>
#include <QProperty>

class Sensor : public QObject {
  Q_OBJECT
public:
  explicit Sensor(bool isNew = false, QObject *parent = nullptr);
  bool operator==(const Sensor &other) const { return name == other.name && oid == other.oid; }

  QUuid oid;
  QString name;
  bool active;
  QString sensorHost;
  int sensorPort;
  QUuid converterID;
  QString channelName;
  QString description;
  QString unit;
  int quantity;
  QVariant value;

  std::unique_ptr<ISensorConverter> _converter;
  ISensorConverter *converter();

  virtual QVariant convert(const QByteArray &data);

  const QVariant at(const int index);
  bool valid();
  void fromJson(const QJsonObject &obj);
  QJsonObject toJson();

  bool isNew = false;
  bool isModified = false;
};

inline QDataStream &operator<<(QDataStream &stream, const Sensor &sensor) {
  return stream << sensor.oid << sensor.name << sensor.active << sensor.sensorHost
                << sensor.sensorPort << sensor.channelName
                << sensor.description << sensor.unit << sensor.quantity;
}

inline QDataStream &operator>>(QDataStream &stream, Sensor &sensor) {
  return stream >> sensor.oid >> sensor.name >> sensor.active >> sensor.sensorHost
         >> sensor.sensorPort >> sensor.channelName
         >> sensor.description >> sensor.unit >> sensor.quantity;
}

#endif // SENSOR_H
