#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QTreeView>
#include "sensormodel.h"

QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QFile)
QT_FORWARD_DECLARE_CLASS(QGroupBox)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QTableView)

class MainWindow : public QMainWindow
{
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

private:
    //QTableView *view;
    QTreeView *view;
    SensorModel *model;
    QGroupBox *createSensorBox(const QString text);
    void createMenuBar();
    void restoreLayout();

signals:
};

#endif // MAINWINDOW_H
