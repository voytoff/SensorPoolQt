#include "sensor.h"

Sensor::Sensor() : oid() {}

bool Sensor::valid() {
  if (oid.isNull())
    oid = QUuid();
  return true;
}

Sensor Sensor::fromJson(const QJsonObject &obj) {
  Sensor sensor;
  sensor.oid = QUuid(obj.value("Oid").toString());
  sensor.name = obj.value("Name").toString();
  sensor.active = obj.value("Active").toBool();
  sensor.sensorHost = obj.value("SensorHost").toString();
  sensor.sensorPort = obj.value("SensorPort").toInt();
  //  sensor.SensorConverter = obj.value("SensorConverter").toString();
  sensor.channelName = obj.value("ChannelName").toString();
  sensor.description = obj.value("Description").toString();
  sensor.unit = obj.value("Unit").toString();
  sensor.quantity = obj.value("Quantity").toInt();
  return sensor;
}

QJsonObject Sensor::toJson(const Sensor &sensor) {
  QJsonObject obj;
  obj["Oid"] = sensor.oid.toString();
  obj["Name"] = sensor.name;
  obj["Active"] = sensor.active;
  obj["SensorHost"] = sensor.sensorHost;
  obj["SensorPort"] = sensor.sensorPort;
  //  obj["SensorConverter"] = sensor.SensorConverter;
  obj["ChannelName"] = sensor.channelName;
  obj["Description"] = sensor.description;
  obj["Unit"] = sensor.unit;
  obj["Quantity"] = sensor.quantity;
  return obj;
}
