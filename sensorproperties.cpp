#include <QBindable>
#include <QCloseEvent>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "sensorproperties.h"
#include "ui_sensorproperties.h"

SensorProperties::SensorProperties(Sensor *sensor, QWidget *parent)
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

  ui->name->setText(sensor->name);
  ui->active->setChecked(sensor->active);
  ui->sensorHost->setText(sensor->sensorHost);
  ui->sensorPort->setValue(sensor->sensorPort);
  //ui->sensorConverter->setCurrentText(sensor.SensorConverter);
  ui->channelName->setText(sensor->channelName);
  ui->description->setText(sensor->description);
  ui->unit->setCurrentText(sensor->unit);
  ui->quantity->setCurrentText(QString::number(sensor->quantity));

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
    sensor->name = ui->name->text();
    sensor->active = ui->active->isChecked();
    sensor->sensorHost = ui->sensorHost->text();
    sensor->sensorPort = ui->sensorPort->value();
    //sensor.SensorConverter = ui->sensorConverter->currentText();
    sensor->channelName = ui->channelName->text();
    sensor->description = ui->description->text();
    sensor->unit = ui->unit->currentText();
    sensor->quantity = ui->quantity->currentText().toInt();
  }
}
