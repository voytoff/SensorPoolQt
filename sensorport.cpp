#include "sensorport.h"

SensorPort::SensorPort(QObject *parent) : QObject{parent},
  tcpSocket(this),
  timer(this) {
  timer.setTimerType(Qt::PreciseTimer);
  QAbstractSocket::connect(&timer, &QTimer::timeout, this, [this]() {
    emit readyData(data);
  });
  QAbstractSocket::connect(&tcpSocket, &QTcpSocket::connected, this, [this]() {
    qDebug() << "Connected to server!";
  });
  QAbstractSocket::connect(&tcpSocket, &QTcpSocket::readyRead, this, [this]() {
    data = tcpSocket.readAll();
    if (debug) qDebug() << "Data received:" << data;
  });
  //QAbstractSocket::connect(&tcpSocket, &QTcpSocket::disconnected, this, &QTcpSocket::deleteLater);
}

void SensorPort::connect(const QString host, const int port) {
  tcpSocket.connectToHost(host, port);
}

void SensorPort::start(int step) {
  int msec = 1000 / step;
  if (!timer.isActive())
    timer.start(msec);
  else
    timer.setInterval(msec);
}

void SensorPort::close() {
  if (timer.isActive())
    timer.stop();
  //tcpSocket.disconnectFromHost();
  tcpSocket.abort();
  data.clear();
}

QString SensorPort::read_string() {
  return QString::fromStdString(data.toStdString());
}
