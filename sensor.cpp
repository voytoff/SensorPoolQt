#include "sensor.h"
#include "interfacefactory.h"

Sensor::Sensor(bool isNew, QObject *parent)
  : QObject{parent}
  , oid(QUuid::createUuid())
  , isNew(isNew) {}

ISensorConverter *Sensor::converter() {
  if (!_converter && InterfaceFactory::instance().contains(converterID))
    _converter = InterfaceFactory::instance().create(converterID);
  return _converter.get();
}

const QVariant Sensor::at(const int index) {
  QVariant result;
  switch (index) {
  case -1: result.setValue(QString("%1:%2").arg(sensorHost).arg(sensorPort)); break;
  case 0: result.setValue(oid); break;
  case 1: result.setValue(name); break;
  case 2: result.setValue(active); break;
  case 3: result.setValue(sensorHost); break;
  case 4: result.setValue(sensorPort); break;
  case 5: result.setValue(converterID); break;
  case 6: result.setValue(channelName); break;
  case 7: result.setValue(description); break;
  case 8: result.setValue(unit); break;
  case 9: result.setValue(quantity); break;
  case 10: result.setValue(value); break;
  default: break;
  }
  return result;
}

QVariant Sensor::convert(const QByteArray &data) {
  return converter() ? converter()->convert(data).toDouble() : QVariant();
}

bool Sensor::valid() {
  if (oid.isNull())
    oid = QUuid::createUuid();
  return true;
}

void Sensor::fromJson(const QJsonObject &obj) {
  oid = QUuid(obj.value("Oid").toString());
  name = obj.value("Name").toString();
  active = obj.value("Active").toBool();
  sensorHost = obj.value("SensorHost").toString();
  sensorPort = obj.value("SensorPort").toInt();
  converterID = QUuid(obj.value("SensorConverter").toString());
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
  obj["SensorConverter"] = converterID.toString();
  obj["ChannelName"] = channelName;
  obj["Description"] = description;
  obj["Unit"] = unit;
  obj["Quantity"] = quantity;
  return obj;
}
