#ifndef SENSORPROPERTIES_H
#define SENSORPROPERTIES_H

#include "sensormodel.h"

#include <QDialog>

namespace Ui {
class SensorProperties;
}

class SensorProperties : public QDialog
{
    Q_OBJECT

public:
    explicit SensorProperties(Sensor &sensor, QWidget *parent = nullptr);
    ~SensorProperties();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void accept(const int result);
    Ui::SensorProperties *ui;
    Sensor &sensor;
};

#endif // SENSORPROPERTIES_H
