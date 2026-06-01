#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QTreeView>
#include <QWebSocket>
#include <QTcpSocket>
#include <QProgressBar>
#include <QGroupBox>

#include "schemehelper.h"
#include "sensormodel.h"
#include "sensorport.h"
#include "settings.h"
/*
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QFile)
QT_FORWARD_DECLARE_CLASS(QGroupBox)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QTreeView)
*/
class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);
  const QString Company = "NIIHM";
  const QString AppName = "SensorPool";

protected:
  void closeEvent(QCloseEvent *event) override;

private slots:
  void adjustHeader();
  void addSensor();
  void delSensor();
  void about();
  void editSensor();
  void save();
  void showIcons();
  void start();
  void stop();

private:
  void createActions();

  Settings* settings;

  QAction *addAction;
  QAction *delAction;
  QAction *quitAction;
  QAction *aboutAction;
  QAction *saveAction;
  QAction *iconsAction;
  QAction *startAction;
  QAction *stopAction;
  QAction *lightAction;
  QAction *darkAction;
  QAction *settingsAction;

  QProgressBar *progressBar;

  //ACDObject *acdObject;
  SensorPort *sensor;

  QTreeView *view;
  SensorModel *model;

  SchemeHelper *schemeHelper;

  QGroupBox *createSensorBox(const QString text);
  QGroupBox *createControlBox(const QString text);
  void createControlBar();
  void restoreLayout();
  void saveLayout();

  void doSettings();


signals:
};

#endif // MAINWINDOW_H
