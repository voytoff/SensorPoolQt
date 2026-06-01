#include "sensor.h"

Sensor::Sensor(QObject *parent) : QObject{parent}, oid() {}

const QVariant Sensor::at(const int index) {
  QVariant result;
  switch (index) {
  case -1:
    result.setValue(QString("%1:%2").arg(sensorHost).arg(sensorPort));
  case 0:
    result.setValue(oid);
  case 1:
    result.setValue(name);
  case 2:
    result.setValue(active);
  case 3:
    result.setValue(sensorHost);
  case 4:
    result.setValue(sensorPort);
  //case 5:
  //  result.setValue(SensorConverter);
  case 6:
    result.setValue(channelName);
  case 7:
    result.setValue(description);
  case 8:
    result.setValue(unit);
  case 9:
    result.setValue(quantity);
  case 10:
    result.setValue(value);
  default:
    break;
  }
  return result;
}

double Sensor::convert(QByteArray data) {
  return 0;
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
