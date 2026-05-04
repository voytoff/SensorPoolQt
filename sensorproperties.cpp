#include "sensorproperties.h"
#include "ui_sensorproperties.h"

SensorProperties::SensorProperties(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SensorProperties)
{
    ui->setupUi(this);
}

SensorProperties::~SensorProperties()
{
    delete ui;
}
