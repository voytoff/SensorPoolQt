#include "sensorport.h"
#include <QRegularExpression>

SensorPort::SensorPort(QObject *parent) : QObject{parent},
  tcpSocket(this),
  timer(this) {
  timer.setTimerType(Qt::PreciseTimer);
  QAbstractSocket::connect(&timer, &QTimer::timeout, this, [this]() {
    sendData();
  });
  QAbstractSocket::connect(&tcpSocket, &QTcpSocket::connected, this, [this]() {
    if (debug) qDebug() << "Connected to server:)";
  });
  QAbstractSocket::connect(&tcpSocket, &QTcpSocket::readyRead, this, [this]() {
    data = tcpSocket.readAll();
    if (debug) qDebug() << "Data received:" << data;
  });
  QAbstractSocket::connect(&tcpSocket, &QTcpSocket::disconnected, this, [this]() {
    if (debug) qDebug() << "Disconnected from server:(";
    //deleteLater();
  });
}

bool SensorPort::connect(Sensor *sensor) {
  this->sensor = sensor;
  tcpSocket.connectToHost(sensor->sensorHost, sensor->sensorPort);
  return isOpen();
}

void SensorPort::start() {
  qDebug() << tcpSocket.state();
  if (!isOpen()) return;
  int msec = 1000 / sensor->quantity;
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
  emit readyData(data);
}

bool SensorPort::isOpen() {
  return connectingStates.contains(tcpSocket.state());
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

QString SensorPort::toString() {
  return trim(data.toStdString());
}

QString SensorPort::trim(const std::string &s) {
  auto text = QString::fromStdString(s);
  text.remove(QRegularExpression("^\\s+"));
  text.remove(QRegularExpression("[\\r\\n\\0]+$"));
  return text;
}

void SensorPort::sendData() {
  if (isOpen())
    emit readyData(data);
}
