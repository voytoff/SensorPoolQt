#include "sensorsettings.h"
#include "ui_sensorsettings.h"

#include <QPushButton>

SensorSettings::SensorSettings(Sensor *sensor, QWidget *parent)
  : ClosableWidget(parent)
  , ui(new Ui::SensorSettings)
  , sensor(sensor)
{
  ui->setupUi(this);

  QString range = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
  QRegularExpression ipRegex("^" + range + "\\." + range + "\\." + range + "\\." + range + "$");

  QRegularExpressionValidator *validator = new QRegularExpressionValidator(ipRegex, this);
  ui->sensorHost->setValidator(validator);
  //ui->sensorHost->setInputMask("000.000.000.000;_");

  ui->quantity->addItem("1", 1);
  ui->quantity->addItem("10", 10);
  ui->quantity->addItem("100", 100);

  ui->oid->setText(sensor->oid.toString());
  ui->name->setText(sensor->name);
  ui->active->setChecked(sensor->active);
  ui->sensorHost->setText(sensor->sensorHost);
  ui->sensorPort->setValue(sensor->sensorPort);
  ui->channelName->setText(sensor->channelName);
  ui->description->setText(sensor->description);
  ui->unit->setCurrentText(sensor->unit);
  ui->quantity->setCurrentText(QString::number(sensor->quantity));

  connect(ui->buttonBox, &QDialogButtonBox::clicked, this, [this](QAbstractButton *button) { clicked(button); });
}

SensorSettings::~SensorSettings() {
  delete ui;
}

void SensorSettings::clicked(QAbstractButton *button) {
  if (button == ui->buttonBox->button(QDialogButtonBox::Save)) {
    sensor->name = ui->name->text();
    sensor->active = ui->active->isChecked();
    sensor->sensorHost = ui->sensorHost->text();
    sensor->sensorPort = ui->sensorPort->value();
    //sensor.SensorConverter = ui->sensorConverter->currentText();
    sensor->channelName = ui->channelName->text();
    sensor->description = ui->description->text();
    sensor->unit = ui->unit->currentText();
    sensor->quantity = ui->quantity->currentText().toInt();
    sensor->isModified = true;
    emit sensorSaved(sensor);
  }
}
