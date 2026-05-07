#include <QBindable>
#include <QCloseEvent>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "sensorproperties.h"
#include "ui_sensorproperties.h"

SensorProperties::SensorProperties(Sensor &sensor, QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::SensorProperties)
  , sensor(sensor)
{
  ui->setupUi(this);

  QString range = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
  QRegularExpression ipRegex("^" + range + "\\." + range + "\\." + range + "\\." + range + "$");

  QRegularExpressionValidator *validator = new QRegularExpressionValidator(ipRegex, this);
  ui->sensorHost->setValidator(validator);
  ui->sensorHost->setInputMask("000.000.000.000;_");

  ui->quantity->addItem("1", 1);
  ui->quantity->addItem("10", 10);
  ui->quantity->addItem("100", 100);

  ui->name->setText(sensor.Name);
  ui->active->setChecked(sensor.Active);
  ui->sensorHost->setText(sensor.SensorHost);
  ui->sensorPort->setValue(sensor.SensorPort);
  ui->sensorConverter->setCurrentText(sensor.SensorConverter);
  ui->channelName->setText(sensor.ChannelName);
  ui->description->setText(sensor.Description);
  ui->unit->setCurrentText(sensor.Unit);
  ui->quantity->setCurrentText(QString::number(sensor.Quantity));

  connect(this, &QDialog::finished, this, [this](int result) { accept(result); });

  setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
}

SensorProperties::~SensorProperties()
{
  delete ui;
}

void SensorProperties::closeEvent(QCloseEvent *event) {}

void SensorProperties::accept(const int result)
{
  qDebug() << "Dialog closed with result:" << result;
  if (result == QDialog::Accepted) {
    sensor.Name = ui->name->text();
    sensor.Active = ui->active->isChecked();
    sensor.SensorHost = ui->sensorHost->text();
    sensor.SensorPort = ui->sensorPort->value();
    sensor.SensorConverter = ui->sensorConverter->currentText();
    sensor.ChannelName = ui->channelName->text();
    sensor.Description = ui->description->text();
    sensor.Unit = ui->unit->currentText();
    sensor.Quantity = ui->quantity->currentText().toInt();
  }
}
