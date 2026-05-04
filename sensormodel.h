#ifndef SENSORMODEL_H
#define SENSORMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QUuid>

struct Sensor
{
  QString Name;
  QUuid Oid;

  bool operator==(const Sensor &other) const
  {
    return Name == other.Name && Oid == other.Oid;
  }
};

class SensorModel: public QAbstractTableModel
{
  Q_OBJECT
  public:
    SensorModel(QObject *parent = nullptr);
    SensorModel(const QList<Sensor> &sensors, QObject *parent = nullptr);
private:
    QList<Sensor> sensors;
};

#endif // SENSORMODEL_H
