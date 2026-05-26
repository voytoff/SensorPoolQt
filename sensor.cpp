#include "sensor.h"

Sensor::Sensor() : oid() {}

double Sensor::convert(QByteArray data) {

}

bool Sensor::valid() {
  if (oid.isNull())
    oid = QUuid();
  return true;
}

void Sensor::fromJson(const QJsonObject &obj) {
  oid = QUuid(obj.value("Oid").toString());
  name = obj.value("Name").toString();
  active = obj.value("Active").toBool();
  sensorHost = obj.value("SensorHost").toString();
  sensorPort = obj.value("SensorPort").toInt();
  //  sensor.SensorConverter = obj.value("SensorConverter").toString();
  channelName = obj.value("ChannelName").toString();
  description = obj.value("Description").toString();
  unit = obj.value("Unit").toString();
  quantity = obj.value("Quantity").toInt();
}

QJsonObject Sensor::toJson() {
  QJsonObject obj;
  obj["Oid"] = oid.toString();
  obj["Name"] = name;
  obj["Active"] = active;
  obj["SensorHost"] = sensorHost;
  obj["SensorPort"] = sensorPort;
  //  obj["SensorConverter"] = SensorConverter;
  obj["ChannelName"] = channelName;
  obj["Description"] = description;
  obj["Unit"] = unit;
  obj["Quantity"] = quantity;
  return obj;
}
