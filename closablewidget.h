#ifndef CLOSABLEWIDGET_H
#define CLOSABLEWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <QScrollArea>

class ClosableWidget : public QWidget {
  Q_OBJECT
public:
  explicit ClosableWidget(QWidget *parent = nullptr);

signals:
  void closing(QWidget* widget);

protected:
  void closeEvent(QCloseEvent *event) override;
};

#endif // CLOSABLEWIDGET_H
