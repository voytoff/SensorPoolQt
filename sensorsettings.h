#ifndef SENSORSETTINGS_H
#define SENSORSETTINGS_H

#include "closablewidget.h"
#include "sensor.h"
#include <QWidget>
#include <QAbstractButton>

namespace Ui {
class SensorSettings;
}

class SensorSettings : public ClosableWidget {
  Q_OBJECT

public:
  explicit SensorSettings(Sensor *sensor, QWidget *parent = nullptr);
  ~SensorSettings();
  Sensor *sensor;

protected:
  void clicked(QAbstractButton *button);

private:
  Ui::SensorSettings *ui;

signals:
  void sensorSaved(Sensor *sensor);
};

#endif // SENSORSETTINGS_H
