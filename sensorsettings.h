#ifndef SENSORSETTINGS_H
#define SENSORSETTINGS_H

#include "closablewidget.h"
#include "interfacefactory.h"
#include "sensor.h"
#include <QWidget>
#include <QAbstractButton>
#include <QStringList>

namespace Ui {
class SensorSettings;
}

class SensorSettings : public ClosableWidget {
  Q_OBJECT

public:
  explicit SensorSettings(Sensor *sensor, QWidget *parent = nullptr);
  ~SensorSettings();
  InterfaceFactory *factory;
  Sensor *sensor;

protected:
  void clicked(QAbstractButton *button);
  void extracted(QJsonArray &jsonArray);
  void readUnits();

private:
  Ui::SensorSettings *ui;
  QStringList units;

signals:
  void sensorSaved(Sensor *sensor);
};

#endif // SENSORSETTINGS_H
