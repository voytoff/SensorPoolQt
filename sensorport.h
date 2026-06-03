#ifndef SENSORPORT_H
#define SENSORPORT_H

#include "sensor.h"

#include <QObject>
#include <QTcpSocket>
#include <QDateTime>
#include <QTimer>
#include <QTime>

class SensorPort : public QObject {
  Q_OBJECT
private:
  //typedef QByteArray Data;
public:
  explicit SensorPort(QObject *parent = nullptr);
  bool debug = false;
  template <typename T> inline T get() {
    T result;
    if constexpr (std::is_same_v<T, QString>)
      result = toString();
    else result = T();
    return result;
  }

protected:
  virtual QString toString();
  virtual double toDouble();
  QTcpSocket tcpSocket;
  QByteArray data;
  QTimer timer;
  Sensor *sensor;
  QVariantList connectedStates {QAbstractSocket::ConnectedState, QTcpSocket::ConnectedState};

private:
  void populateData();

public slots:
  void connect(Sensor *sensor);
  void start();
  void close();
  bool isOpen();

signals:
  void dataChanged(Sensor *sensor, QByteArray &data);
};

#endif // SENSORPORT_H
