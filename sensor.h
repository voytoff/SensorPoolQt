#ifndef SENSOR_H
#define SENSOR_H

#include <QUuid>
#include <QJsonObject>
class Sensor {// : ISensorConverter {
public:
  Sensor();
  bool operator==(const Sensor &other) const { return name == other.name && oid == other.oid; }

  QUuid oid;
  QString name;
  bool active;
  QString sensorHost;
  int sensorPort;
  QString channelName;
  QString description;
  QString unit;
  int quantity;
  double current;

  virtual double convert(QByteArray data);

  bool valid();
  void fromJson(const QJsonObject &obj);
  QJsonObject toJson();
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
