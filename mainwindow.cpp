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

#include "sensorproperties.h"

MainWindow::MainWindow(const QString &artistTable, QWidget *parent)
  : QMainWindow{parent},
  table(new SensorModel(this))
{
  this->setWindowIcon(QIcon(":/images/sensor.svg"));
  table->readFromFile();

  QGroupBox *sensorList = createSensorBox();

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
  settings.setValue("geometry", saveGeometry()); // Saves window size/position
  settings.setValue("windowState", saveState()); // Saves dock/toolbar layout
  QMainWindow::closeEvent(event);
}

void MainWindow::createMenuBar()
{
    QAction *addAction = new QAction(QIcon(":/images/tb/add.svg"), tr("Добавить датчик..."), this);
    QAction *deleteAction = new QAction(QIcon(":/images/tb/del.svg"), tr("Удалить текущий..."), this);
    QAction *quitAction = new QAction(QIcon(":/images/tb/exit.svg"), tr("&Quit"), this);
    QAction *aboutAction = new QAction(tr("&About"), this);
    QAction *aboutQtAction = new QAction(tr("About &Qt"), this);

    addAction->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_A));
    deleteAction->setShortcut(tr("Ctrl+D"));
    quitAction->setShortcuts(QKeySequence::Quit);

    QMenu *fileMenu = menuBar()->addMenu(tr("&Файл"));
    fileMenu->addAction(addAction);
    fileMenu->addAction(deleteAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    QMenu *helpMenu = menuBar()->addMenu(tr("&?"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

    connect(addAction, &QAction::triggered,
            this, &MainWindow::addSensor);
    connect(aboutAction, &QAction::triggered,
            this, &MainWindow::about);
    /*
    connect(deleteAction, &QAction::triggered,
            this, &MainWindow::deleteAlbum);
    connect(quitAction, &QAction::triggered,
            qApp, &QCoreApplication::quit);
    connect(aboutQtAction, &QAction::triggered,
            qApp, &QApplication::aboutQt);
*/
}

void MainWindow::restoreLayout()
{
  QSettings settings(Company, AppName);
  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("windowState").toByteArray());
}

QGroupBox *MainWindow::createSensorBox()
{
    QGroupBox *box = new QGroupBox(tr("Sensors"));

  view = new QTreeView;// QTableView;
    //view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //view->setSortingEnabled(true);
    //view->setSelectionBehavior(QAbstractItemView::SelectRows);
    //view->setSelectionMode(QAbstractItemView::SingleSelection);
    //view->setShowGrid(false);
    //view->verticalHeader()->hide();
    //view->setAlternatingRowColors(true);
    view->setModel(table);
    adjustHeader();

    view->setRootIsDecorated(false);
    view->setAlternatingRowColors(true);
    view->setSortingEnabled(true);
    connect(view, &QTreeView::doubleClicked, this, &MainWindow::treeDoubleClick);

    QLocale locale = view->locale();
    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    view->setLocale(locale);
    /*
    connect(view, &QTableView::clicked,
            this, &MainWindow::showAlbumDetails);
    connect(view, &QTableView::activated,
            this, &MainWindow::showAlbumDetails);
    */
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view, 0, {});
    box->setLayout(layout);

    return box;
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Music Archive"),
                       tr("<p>The <b>Sensor pull</b> программа записи параметров "
                          "обработки экспериментов с датчиков регистрации.</p>"));
}

void MainWindow::addSensor()
{
  Sensor sensor{QUuid()};
  SensorProperties *dialog = new SensorProperties(sensor, this);
  int accepted = dialog->exec();

  if (accepted == QDialog::Accepted) {
    //int lastRow = model->rowCount() - 1;
    //view->selectRow(lastRow);
    view->scrollToBottom();
    //showAlbumDetails(model->index(lastRow, 0));
  }
}

void MainWindow::treeDoubleClick()
{
  Sensor sensor = *table->get(0);

  SensorProperties *dialog = new SensorProperties(sensor, this);
  int accepted = dialog->exec();

  if (accepted == QDialog::Accepted) {
    //int lastRow = model->rowCount() - 1;
    //view->selectRow(lastRow);
    view->scrollToBottom();
    //showAlbumDetails(model->index(lastRow, 0));
  }
}

void MainWindow::adjustHeader()
{
    view->hideColumn(0);
    //view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    view->resizeColumnToContents(2);
    view->resizeColumnToContents(3);
}
