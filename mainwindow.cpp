#include <QApplication>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QCloseEvent>
#include <QComboBox>
#include <QFile>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QModelIndex>
#include <QSettings>
#include <QSizeGrip>
#include <QSplitter>
#include <QStatusBar>
#include <QStyleHints>
#include <QTableView>
#include <QToolBar>
#include <QDialog>

#include "mainwindow.h"
#include "sensorproperties.h"
#include "sensorsettings.h"
#include "settingsdlg.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow{parent}
  , model(new SensorModel(this))
  , treeView(new QTreeView(this))
  , tabWidget(new QTabWidget(this))
  , splitter(new QSplitter(this))
  , sensorPort(new SensorPort(this))
  , schemeHelper(new SchemeHelper(this, ":/images/rocket.svg"))
  , settings(new Settings())
  , progressBar(new QProgressBar()) {

  QIcon::setThemeName("Material Symbols Outlined");

  connect(sensorPort, &SensorPort::readyData, this, [this](QByteArray data) {
    // qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz") << sensorPort->get<QString>();
    model->setData(model->index(1, 10, QModelIndex()), sensorPort->get<QString>());
  });

  model->read();

  createActions();
  createControlBar();
  statusBar()->setSizeGripEnabled(true);

  createControlBox();

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(splitter, 0, 0);
  layout->setContentsMargins(2, 1, 2, 0);

  QWidget *widget = new QWidget;
  widget->setLayout(layout);
  setCentralWidget(widget);

  restoreLayout();
}

void MainWindow::closeEvent(QCloseEvent *event) {
  saveLayout();
  model->write();
  QMainWindow::closeEvent(event);
}

void MainWindow::createActions() {
  addAction = schemeHelper->create(tr("Добавить датчик..."), ":/images/tb/add.svg", QKeySequence(Qt::CTRL | Qt::Key_A));
  delAction = schemeHelper->create(tr("Удалить текущий..."), ":/images/tb/del.svg", QKeySequence(Qt::CTRL | Qt::Key_D));
  quitAction = schemeHelper->create(tr("Выход"), ":/images/tb/exit.svg", QKeySequence::Quit);
  aboutAction = schemeHelper->create(tr("&About"));
  saveAction = schemeHelper->create(tr("Сохранить"), ":/images/tb/save.svg", QKeySequence::Save);
  startAction = schemeHelper->create(tr("Запуск опроса"), ":/images/tb/start.svg", QKeySequence(Qt::CTRL | Qt::Key_B));
  stopAction = schemeHelper->create(tr("Останов опроса"), ":/images/tb/stop.svg", QKeySequence(Qt::CTRL | Qt::Key_E));
  lightAction = schemeHelper->createLightAction(tr("Дневной режим"), ":/images/tb/light.svg");
  darkAction = schemeHelper->createDarkAction(tr("Ночной режим"), ":/images/tb/dark.svg");
  settingsAction = schemeHelper->create(tr("Установки..."), ":/images/tb/settings.svg");

  connect(addAction, &QAction::triggered, this, &MainWindow::addSensor);
  connect(delAction, &QAction::triggered, this, &MainWindow::delSensor);
  connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
  connect(saveAction, &QAction::triggered, this, &MainWindow::save);
  connect(quitAction, &QAction::triggered, this, [this]() { close(); });
  connect(settingsAction, &QAction::triggered, this, &MainWindow::doSettings);
  connect(startAction, &QAction::triggered, this, &MainWindow::start);
  connect(stopAction, &QAction::triggered, this, &MainWindow::stop);

  schemeHelper->applayColorScheme(settings->colorScheme(), true);
}

void MainWindow::createControlBar() {
  QMenu *fileMenu = menuBar()->addMenu(tr("&Файл"));
  fileMenu->addAction(addAction);
  fileMenu->addAction(delAction);
  fileMenu->addSeparator();
  fileMenu->addAction(saveAction);
  fileMenu->addSeparator();
  fileMenu->addAction(quitAction);

  QMenu *viewMenu = menuBar()->addMenu(tr("Вид"));
  viewMenu->addAction(lightAction);
  viewMenu->addAction(darkAction);

  QMenu *toolMenu = menuBar()->addMenu(tr("Инструменты"));
  toolMenu->addAction(startAction);
  toolMenu->addAction(stopAction);
  toolMenu->addSeparator();
  toolMenu->addAction(settingsAction);

  QMenu *helpMenu = menuBar()->addMenu(tr("&?"));
  helpMenu->addAction(aboutAction);

  //menuBar()->setNativeMenuBar(false);

  auto toolbar = addToolBar("Главный");
  toolbar->setObjectName("General");
  toolbar->addAction(addAction);
  toolbar->addAction(delAction);
  toolbar->addSeparator();
  toolbar->addAction(startAction);
  toolbar->addAction(stopAction);
  toolbar->addSeparator();
  toolbar->addAction(saveAction);

  progressBar->setRange(0, 0);
  progressBar->setValue(0);
  progressBar->setTextVisible(true);
  statusBar()->addPermanentWidget(progressBar);
  progressBar->setMaximumWidth(120);
  progressBar->setMaximumHeight(16);
  progressBar->hide();
}

void MainWindow::createControlBox() {
  treeView->setModel(model);
  adjustHeader();

  treeView->setRootIsDecorated(false);
  treeView->setAlternatingRowColors(true);
  treeView->setSortingEnabled(true);
  connect(treeView, &QTreeView::doubleClicked, this, &MainWindow::editSensor);

  QLocale locale = treeView->locale();
  locale.setNumberOptions(QLocale::OmitGroupSeparator);
  treeView->setLocale(locale);
  treeView->setItemDelegate(new CustomDelegate());

  tabWidget->setTabsClosable(true);
  connect(tabWidget, &QTabWidget::tabCloseRequested, this, [this](int index) {
    QWidget* w = tabWidget->widget(index);
    tabWidget->removeTab(index);
    delete w;
  });

  splitter->addWidget(treeView);
  splitter->addWidget(tabWidget);
}

void MainWindow::about() {
  QMessageBox::about(
    this,
    tr("About Music Archive"),
    tr("<p><b>Sensor Pool</b> программа записи параметров "
      "обработки экспериментов с датчиков регистрации.</p>"));
}

void MainWindow::addSensor() {
  Sensor *sensor = new Sensor(true);
  SensorSettings *settings = new SensorSettings(sensor, this);
  addTab(settings, "Новый сенсор");
  connect(settings, &SensorSettings::sensorSaved, this, [this](Sensor* sensor) {
    if (sensor->isNew) {
      sensor->isNew = false;
      model->add(sensor);
      treeView->setCurrentIndex(model->last());
      treeView->scrollToBottom();
    }
  });
}

void MainWindow::delSensor() {
  QModelIndex index = treeView->currentIndex();
  if (index.isValid()) {
    Sensor *sensor = model->get(index.row());
    auto response = QMessageBox::question(this, AppName, QString("Удалить текущий сенсор '%1' из списка?").arg(sensor->name));
    if (response == QMessageBox::Yes) {
      model->removeRow(index.row());
      model->layoutChanged();
    }
  }
}

void MainWindow::editSensor() {
  QModelIndex index = treeView->currentIndex();
  if (index.isValid()) {
    Sensor *sensor = model->get(index.row());
    SensorSettings *settings = new SensorSettings(sensor, this);
    addTab(settings, sensor->name);
  }
}

void MainWindow::save() {
  model->write();
}

void MainWindow::start() {
  QModelIndex index = treeView->currentIndex();
  if (index.isValid()) {
    Sensor *sensor = model->get(index.row());
    if (sensorPort->connect(sensor))
      sensorPort->start();
  }
}

void MainWindow::stop() {
  sensorPort->close();
}

void MainWindow::adjustHeader() {
  for (int n = 0; n < model->columnCount(QModelIndex()); n++)
    if (!model->visible(n))
      treeView->hideColumn(n);
  treeView->resizeColumnToContents(0);
  treeView->resizeColumnToContents(1);
  treeView->resizeColumnToContents(3);
}

void MainWindow::saveLayout() {
  settings->geometry(saveGeometry());
  settings->windowState(saveState());
  settings->splitter(splitter->saveState());
}

void MainWindow::restoreLayout() {
  restoreGeometry(settings->geometry());
  restoreState(settings->windowState());
  splitter->restoreState(settings->splitter());
}

void MainWindow::doSettings() {
  SettingsDlg *dialog = new SettingsDlg(settings, this);
  int accepted = dialog->exec();
  if (accepted == QDialog::Accepted) {
    auto colorScheme = settings->colorScheme();
    if ((Qt::ColorScheme)colorScheme != QGuiApplication::styleHints()->colorScheme())
      schemeHelper->applayColorScheme(colorScheme);
  }
}

int MainWindow::addTab(QWidget *widget, const QString &name) {
  int index = tabWidget->addTab(widget, name);
  tabWidget->setCurrentIndex(index);
  return index;
}

