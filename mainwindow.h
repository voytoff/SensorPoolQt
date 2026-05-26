#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QTreeView>
#include <QWebSocket>
#include <QTcpSocket>

#include "ACDObject.h"
#include "sensormodel.h"
#include "sensorport.h"

QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QFile)
QT_FORWARD_DECLARE_CLASS(QGroupBox)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QTreeView)

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(const QString &artistTable, QWidget *parent = nullptr);
  const QString Company = "NIIHM";
  const QString AppName = "SensorPool";

protected:
  void closeEvent(QCloseEvent *event) override;

private slots:
  void adjustHeader();
  void addSensor();
  void about();
  void editSensor();
  void save();
  void showIcons();
  void start();
  void stop();

private:
  //QTableView *view;
  QTreeView *view;
  SensorModel *model;
  QGroupBox *createSensorBox(const QString text);
  QGroupBox *createControlBox(const QString text);
  void createMenuBar();
  void restoreLayout();
  ACDObject *acdObject;

  SensorPort *sensor;

signals:
};

#endif // MAINWINDOW_H
