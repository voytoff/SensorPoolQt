#include <QException>
#include <QIcon>
#include <QLabel>
#include <QMetaEnum>
#include <QStackedWidget>

#include "themeicons.h"
#include "ui_themeicons.h"

ThemeIcons::ThemeIcons(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::ThemeIcons)
{
  ui->setupUi(this);
  QMetaEnum metaEnum = QMetaEnum::fromType<QIcon::ThemeIcon>();
  for (int i = 0; i < metaEnum.keyCount(); ++i) {
    try {
      const auto *key = metaEnum.key(i);
      QIcon::ThemeIcon value = (QIcon::ThemeIcon) metaEnum.value(i);
      if (value == QIcon::ThemeIcon::NThemeIcons)
        continue;
      qDebug() << "Key:" << key << "Value:" << value;

      QIcon icon(QIcon::fromTheme(value));
      QLabel *iconLabel = new QLabel();
      iconLabel->setPixmap(icon.pixmap(QSize(24, 24)));
      ui->gridLayout->addWidget(iconLabel, i, 0, 1, 1, Qt::AlignLeft);
      iconLabel->setVisible(true);

      QLabel *textLabel = new QLabel(key);
      ui->gridLayout->addWidget(textLabel, i, 1, 1, 1);

    } catch (const std::exception &e) {
      qCritical() << "Exception caught in notify:" << e.what();
    } catch (...) {
      qCritical() << "Unknown exception caught in notify";
    }
  }
}

ThemeIcons::~ThemeIcons()
{
  delete ui;
}
