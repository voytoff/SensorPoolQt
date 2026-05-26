#ifndef SENSORPORT_H
#define SENSORPORT_H

#include <QObject>
#include <QTcpSocket>
#include <QDateTime>
#include <QTimer>
#include <QTime>

class SensorPort : public QObject {
  Q_OBJECT
private:
  typedef QByteArray Data;
public:
  explicit SensorPort(QObject *parent = nullptr);
  bool debug = false;
  template <typename T> inline T get() {
    T result;
    if constexpr (std::is_same_v<T, QString>)
      result = read_string();
    else result = T();
    return result;
  }

protected:
  virtual QString read_string();
  QTcpSocket tcpSocket;
  Data data;
  QTimer timer;

public slots:
  bool connect(const QString host, const int port);
  void start(int step);
  void close();
  bool isOpen();

signals:
  void readyData(QByteArray &data);
};

#endif // SENSORPORT_H
