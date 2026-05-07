#ifndef THEMEICONS_H
#define THEMEICONS_H

#include <QDialog>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>

namespace Ui {
class ThemeIcons;
}

class ThemeIcons : public QDialog
{
  Q_OBJECT

public:
  explicit ThemeIcons(QWidget *parent = nullptr);
  ~ThemeIcons();

private:
  Ui::ThemeIcons *ui;
  QLabel *createImage(QMetaEnum *metaEnum, const int i);
};

#endif // THEMEICONS_H
