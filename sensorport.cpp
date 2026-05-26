#include "sensorport.h"

SensorPort::SensorPort(QObject *parent) : QObject{parent},
  tcpSocket(this),
  timer(this) {
  timer.setTimerType(Qt::PreciseTimer);
  QAbstractSocket::connect(&timer, &QTimer::timeout, this, [this]() {
    if (isOpen())
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

bool SensorPort::connect(const QString host, const int port) {
  tcpSocket.connectToHost(host, port);
  return isOpen();
}

void SensorPort::start(int step) {
  if (!isOpen()) return;
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

bool SensorPort::isOpen() {
  return tcpSocket.state() == QTcpSocket::ConnectedState;
/*
  switch (state) {
    case QAbstractSocket::UnconnectedState: // Disconnected  break;
    case QAbstractSocket::HostLookupState:  // Looking up host...  break;
    case QAbstractSocket::ConnectingState:  // Attempting to connect...  break;
    case QAbstractSocket::ConnectedState:   // Connection established!  break;
    case QAbstractSocket::ClosingState:     // Socket closing...  break;
    default: break;
  }
*/
}

QString SensorPort::read_string() {
  return QString::fromStdString(data.toStdString());
}
