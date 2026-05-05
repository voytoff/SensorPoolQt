#ifndef SENSORMODEL_H
#define SENSORMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QUuid>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariant>

struct Sensor
{
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

  bool operator==(const Sensor &other) const
  {
    return Name == other.Name && Oid == other.Oid;
  }
};


inline QDataStream &operator<<(QDataStream &stream, const Sensor &sensor)
{
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

inline QDataStream &operator>>(QDataStream &stream, Sensor &sensor)
{
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
inline Sensor operator>>(QJsonObject &obj, Sensor &sensor)
{
  return Sensor
      {
          QUuid(obj["Oid"].toString()),
          obj["Name"].toString(),
          obj["Active"].toBool(),
          obj["SensorHost"].toString(),
          obj["SensorPort"].toInt(),
          obj["SensorConverter"].toString(),
          obj["ChannelName"].toString(),
          obj["Description"].toString(),
          obj["Unit"].toString(),
          obj["Quantity"].toInt()
      };
}


class SensorModel: public QAbstractTableModel
{
  Q_OBJECT
  public:
    SensorModel(QObject *parent = nullptr);
    SensorModel(const QList<Sensor> &sensors, QObject *parent = nullptr);
    int readFromFile();
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
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

#endif // SENSORMODEL_H
