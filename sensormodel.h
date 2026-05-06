#ifndef SENSORMODEL_H
#define SENSORMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QUuid>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariant>
#include <QStyledItemDelegate>
#include <QPainter>

struct Sensor {
  //Sensor() {Oid = QUuid();}
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

  bool valid() {
    if (Oid.isNull()) Oid = QUuid();
    return true;
  }

  bool operator==(const Sensor &other) const {
    return Name == other.Name && Oid == other.Oid;
  }
};


inline QDataStream &operator<<(QDataStream &stream, const Sensor &sensor) {
  return stream
         << sensor.Oid
         << sensor.Name
         << sensor.Active
         << sensor.SensorHost
         << sensor.SensorPort
         << sensor.SensorConverter
         << sensor.ChannelName
         << sensor.Description
         << sensor.Unit
         << sensor.Quantity;
}

inline QDataStream &operator>>(QDataStream &stream, Sensor &sensor) {
  return stream
         >> sensor.Oid
         >> sensor.Name
         >> sensor.Active
         >> sensor.SensorHost
         >> sensor.SensorPort
         >> sensor.SensorConverter
         >> sensor.ChannelName
         >> sensor.Description
         >> sensor.Unit
         >> sensor.Quantity;
}
inline Sensor operator>>(const QJsonObject &obj, Sensor &sensor) {
  sensor.Oid = QUuid(obj.value("Oid").toString());
  sensor.Name = obj.value("Name").toString();
  sensor.Active = obj.value("Active").toBool();
  sensor.SensorHost = obj.value("SensorHost").toString();
  sensor.SensorPort = obj.value("SensorPort").toInt();
  sensor.SensorConverter = obj.value("SensorConverter").toString();
  sensor.ChannelName = obj.value("ChannelName").toString();
  sensor.Description = obj.value("Description").toString();
  sensor.Unit = obj.value("Unit").toString();
  sensor.Quantity = obj.value("Quantity").toInt();
  return sensor;
}
inline QJsonObject operator<<(QJsonObject &obj, const Sensor &sensor) {
  obj["Oid"] = sensor.Oid.toString();
  obj["Name"] = sensor.Name;
  obj["Active"] = sensor.Active;
  obj["SensorHost"] = sensor.SensorHost;
  obj["SensorPort"] = QString::number(sensor.SensorPort);
  obj["SensorConverter"] = sensor.SensorConverter;
  obj["ChannelName"] = sensor.ChannelName;
  obj["Description"] = sensor.Description;
  obj["Unit"] = sensor.Unit;
  obj["Quantity"] = QString::number(sensor.Quantity);
  return obj;
}


class SensorModel: public QAbstractTableModel {
  Q_OBJECT
  public:
    SensorModel(QObject *parent = nullptr);
    SensorModel(const QList<Sensor> &sensors, QObject *parent = nullptr);
    int readFromFile();
    void saveToFile();
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool visible(int col) const;
    QModelIndex ind(int row) const;
    QModelIndex last() const;
    QModelIndex first() const;
    int count() const;
    void add(Sensor sensor);
    Sensor* get(int row);
    void replace(int row, const Sensor sensor);
private:
    Sensor addEntry(
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
    );
    void addEntry(const Sensor &sensor);
    QList<Sensor> sensors;
};


class CustomDelegate : public QStyledItemDelegate {
  public:
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
    // Custom drawing logic here
    //QStyledItemDelegate::paint(painter, option, index);
    painter->save();
    painter->setPen(Qt::blue);
    painter->drawText(option.rect, Qt::AlignCenter, option.text);
    painter->restore();

  }

};


#endif // SENSORMODEL_H
