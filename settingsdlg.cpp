#include "settingsdlg.h"
#include "ui_settingsdlg.h"

SettingsDlg::SettingsDlg(Settings *settings, QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::SettingsDlg)
  , settings(settings)
{
  ui->setupUi(this);

  ui->colorScheme->addItem("Light", ColorScheme::Light);
  ui->colorScheme->addItem("Dark", ColorScheme::Dark);
  ui->colorScheme->setCurrentIndex(settings->colorScheme()-1);

  connect(this, &QDialog::finished, this, [this](int result) { accept(result); });

  setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
}

SettingsDlg::~SettingsDlg() {
  delete ui;
}

void SettingsDlg::accept(const int result) {
  if (result == QDialog::Accepted) {
//    settings->frequency(ui->frequency->currentData());
//    settings->axisXType(ui->axisXType->currentData());
    settings->colorScheme(ui->colorScheme->currentData());
  }
}
