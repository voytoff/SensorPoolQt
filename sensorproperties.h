#ifndef SENSORPROPERTIES_H
#define SENSORPROPERTIES_H

#include <QDialog>

namespace Ui {
class SensorProperties;
}

class SensorProperties : public QDialog
{
    Q_OBJECT

public:
    explicit SensorProperties(QWidget *parent = nullptr);
    ~SensorProperties();

private:
    Ui::SensorProperties *ui;
};

#endif // SENSORPROPERTIES_H
