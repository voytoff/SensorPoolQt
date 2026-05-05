#include <QBindable>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "sensorproperties.h"
#include "ui_sensorproperties.h"

SensorProperties::SensorProperties(Sensor &sensor, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SensorProperties)
{
  ui->setupUi(this);

  QString range = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
  QRegularExpression ipRegex("^" + range + "\\." + range + "\\." + range + "\\." + range + "$");

  QRegularExpressionValidator *validator = new QRegularExpressionValidator(ipRegex, this);
  ui->sensorHost->setValidator(validator);
  ui->sensorHost->setInputMask("000.000.000.000;_");

  ui->name->setText(sensor.Name);
  ui->active->setChecked(sensor.Active);
  ui->sensorHost->setText(sensor.SensorHost);
  ui->sensorPort->setValue(sensor.SensorPort);
  ui->sensorConverter->setCurrentText(sensor.SensorConverter);
  ui->channelName->setText(sensor.ChannelName);
  ui->description->setText(sensor.Description);
  ui->unit->setCurrentText(sensor.Unit);
  ui->quantity->setValue(sensor.Quantity);
}

SensorProperties::~SensorProperties()
{
    delete ui;
}
