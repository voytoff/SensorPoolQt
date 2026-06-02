#include "closablewidget.h"

ClosableWidget::ClosableWidget(QWidget *parent) : QWidget{parent} {}

void ClosableWidget::closeEvent(QCloseEvent *event) {
  emit closing(this);
  event->accept();
}