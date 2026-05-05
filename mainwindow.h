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
private slots:
    void adjustHeader();
    void addAlbum();
    void about();

private:
    //QTableView *view;
    QTreeView *view;
    SensorModel *table;
    QGroupBox *createSensorBox();
    void createMenuBar();

signals:
};

#endif // MAINWINDOW_H
