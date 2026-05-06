#include "mainwindow.h"
#include <QApplication>
#include <QComboBox>
#include <QFile>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QTableView>
#include <QIcon>
#include <QSizeGrip>
#include <QStatusBar>
#include <QSettings>
#include <QModelIndex>
#include <QCloseEvent>

#include "mainwindow.h"
#include "sensorproperties.h"

MainWindow::MainWindow(const QString &artistTable, QWidget *parent)
  : QMainWindow{parent}
  , model(new SensorModel(this))
{
  this->setWindowIcon(QIcon(":/images/sensor.svg"));
  model->readFromFile();

  QGroupBox *sensorList = createSensorBox("Датчики");

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(sensorList, 0, 0);
  //layout->addWidget(new QSizeGrip(this), 1, 0, Qt::AlignBottom | Qt::AlignRight);
  //layout->setColumnStretch(1, 1);
  //layout->setColumnMinimumWidth(0, 500);

  QWidget *widget = new QWidget;
  widget->setLayout(layout);
  setCentralWidget(widget);

  createMenuBar();
  statusBar()->setSizeGripEnabled(true);
  restoreLayout();
}

void MainWindow::closeEvent(QCloseEvent *event) {
  QSettings settings(Company, AppName);
  settings.setValue("geometry", saveGeometry());
  settings.setValue("windowState", saveState());

  QMainWindow::closeEvent(event);
}

void MainWindow::createMenuBar()
{
  QAction *addAction = new QAction(QIcon(":/images/tb/add.svg"), tr("Добавить датчик..."), this);
  QAction *deleteAction = new QAction(QIcon(":/images/tb/del.svg"), tr("Удалить текущий..."), this);
  QAction *quitAction = new QAction(QIcon(":/images/tb/exit.svg"), tr("&Quit"), this);
  QAction *aboutAction = new QAction(tr("&About"), this);
  QAction *aboutQtAction = new QAction(tr("About &Qt"), this);
  QAction *saveAction = new QAction(QIcon(":/images/tb/save.svg"), tr("Сохранить"), this);

  addAction->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_A));
  deleteAction->setShortcut(tr("Ctrl+D"));
  quitAction->setShortcuts(QKeySequence::Quit);
  saveAction->setShortcuts(QKeySequence::Save);

  QMenu *fileMenu = menuBar()->addMenu(tr("&Файл"));
  fileMenu->addAction(addAction);
  fileMenu->addAction(deleteAction);
  fileMenu->addSeparator();
  fileMenu->addAction(saveAction);
  fileMenu->addSeparator();
  fileMenu->addAction(quitAction);

  QMenu *helpMenu = menuBar()->addMenu(tr("&?"));
  helpMenu->addAction(aboutAction);
  helpMenu->addAction(aboutQtAction);

  connect(addAction, &QAction::triggered, this, &MainWindow::addSensor);
  connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
  connect(saveAction, &QAction::triggered, this, &MainWindow::save);
}

void MainWindow::restoreLayout()
{
  QSettings settings(Company, AppName);
  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("windowState").toByteArray());
}

QGroupBox *MainWindow::createSensorBox(const QString text)
{
  QGroupBox *box = new QGroupBox(text);

  view = new QTreeView;// QTableView;
  //view->setEditTriggers(QAbstractItemView::NoEditTriggers);
  //view->setSortingEnabled(true);
  //view->setSelectionBehavior(QAbstractItemView::SelectRows);
  //view->setSelectionMode(QAbstractItemView::SingleSelection);
  //view->setShowGrid(false);
  //view->verticalHeader()->hide();
  //view->setAlternatingRowColors(true);
  view->setModel(model);
  adjustHeader();

  view->setRootIsDecorated(false);
  view->setAlternatingRowColors(true);
  view->setSortingEnabled(true);
  connect(view, &QTreeView::doubleClicked, this, &MainWindow::editSensor);
  /*
  connect(view, &QTableView::clicked, this, &MainWindow::showAlbumDetails);
  connect(view, &QTableView::activated, this, &MainWindow::showAlbumDetails);
  */

  QLocale locale = view->locale();
  locale.setNumberOptions(QLocale::OmitGroupSeparator);
  view->setLocale(locale);

  CustomDelegate *delegate = new CustomDelegate();
  view->setItemDelegateForColumn(1, delegate);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(view, 0, {});
  box->setLayout(layout);

  return box;
}

void MainWindow::about()
{
    QMessageBox::about(
      this,
      tr("About Music Archive"),
      tr("<p><b>Sensor Pool</b> программа записи параметров "
         "обработки экспериментов с датчиков регистрации.</p>"));
}

void MainWindow::addSensor() {
  Sensor sensor {QUuid()};
  SensorProperties *dialog = new SensorProperties(sensor, this);
  int accepted = dialog->exec();

  if (accepted == QDialog::Accepted && sensor.valid()) {
    model->add(sensor);
    view->setCurrentIndex(model->last());
    view->scrollToBottom();
  }
}

void MainWindow::editSensor() {
  QModelIndex index = view->currentIndex();
  if (index.isValid()) {
    Sensor sensor = *model->get(index.row());

    SensorProperties *dialog = new SensorProperties(sensor, this);
    int accepted = dialog->exec();
    if (accepted == QDialog::Accepted && sensor.valid()) {
      model->replace(index.row(), sensor);
    }
  }
}

void MainWindow::save() {
  model->saveToFile();
}

void MainWindow::adjustHeader()
{
  for (int n = 0; n < model->columnCount(QModelIndex()); n++)
    if (!model->visible(n))
      view->hideColumn(n);
  view->resizeColumnToContents(0);
  view->resizeColumnToContents(1);
  view->resizeColumnToContents(3);
}
