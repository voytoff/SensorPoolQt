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

#include "mainwindow.h"
#include "sensorproperties.h"
#include "settingsdlg.h"
#include "themeicons.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow{parent},
  model(new SensorModel(this)),
  sensor(new SensorPort(this)),
  schemeHelper(new SchemeHelper(this, ":/images/rocket.svg")),
  settings(new Settings()),
  progressBar(new QProgressBar()) {

  QIcon::setThemeName("Material Symbols Outlined");

  connect(sensor, &SensorPort::readyData, this, [this](QByteArray data) {
    qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz") << sensor->get<QString>();
  });

  model->read();

  createActions();
  createControlBar();
  statusBar()->setSizeGripEnabled(true);

  auto *sensorList = createSensorBox("Датчики");
  auto controlBox = createControlBox("Управление");
  //auto splitter = new QSplitter(this);
  //splitter->addWidget(sensorList);
  //splitter->addWidget(controlBox);

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(sensorList, 0, 0);
  //layout->addWidget(splitter, 0, 0);
  layout->addWidget(controlBox, 0, 1);

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
  saveAction = schemeHelper->create(tr("Сохранить"), nullptr, QKeySequence::Save);
  iconsAction = schemeHelper->create(tr("Просмотр иконок..."));
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
  connect(iconsAction, &QAction::triggered, this, &MainWindow::showIcons);
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
  helpMenu->addAction(iconsAction);

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

void MainWindow::restoreLayout() {
  restoreGeometry(settings->geometry());
  restoreState(settings->windowState());
  //splitter->restoreState(settings->splitter());
}

QGroupBox *MainWindow::createSensorBox(const QString text) {
  QGroupBox *box = new QGroupBox(text);

  view = new QTreeView;
  view->setModel(model);
  adjustHeader();

  view->setRootIsDecorated(false);
  view->setAlternatingRowColors(true);
  view->setSortingEnabled(true);
  connect(view, &QTreeView::doubleClicked, this, &MainWindow::editSensor);

  QLocale locale = view->locale();
  locale.setNumberOptions(QLocale::OmitGroupSeparator);
  view->setLocale(locale);

  view->setItemDelegate(new CustomDelegate());

  QBoxLayout *layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
  layout->addWidget(view);
  box->setLayout(layout);

  return box;
}

QGroupBox *MainWindow::createControlBox(const QString text) {
  QGroupBox *box = new QGroupBox(text);
  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Orientation::Vertical, box);

  connect(buttonBox, &QDialogButtonBox::accepted, this, &MainWindow::start);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &MainWindow::stop);

  QBoxLayout *layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
  layout->addWidget(buttonBox);
  box->setLayout(layout);

  return box;
}

void MainWindow::about() {
  QMessageBox::about(
    this,
    tr("About Music Archive"),
    tr("<p><b>Sensor Pool</b> программа записи параметров "
      "обработки экспериментов с датчиков регистрации.</p>"));
}

void MainWindow::addSensor() {
  Sensor sensor;
  SensorProperties *dialog = new SensorProperties(sensor, this);
  int accepted = dialog->exec();

  if (accepted == QDialog::Accepted && sensor.valid()) {
    model->add(sensor);
    view->setCurrentIndex(model->last());
    view->scrollToBottom();
  }
}

void MainWindow::delSensor() {
  QModelIndex index = view->currentIndex();
  if (index.isValid()) {
    Sensor *sensor = model->get(index.row());
    auto response = QMessageBox::question(this, AppName, QString("Удалить текущий сенсор '%1' из списка?").arg(sensor->name));
    if (response == QMessageBox::Yes)
      model->removeRow(index.row());
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
  model->write();
}

void MainWindow::showIcons() {
  ThemeIcons *d = new ThemeIcons(this);
  d->exec();
}

void MainWindow::start() {
  sensor->connect("192.168.10.230", 25);
  sensor->start(10);
}

void MainWindow::stop() {
  sensor->close();
}

void MainWindow::adjustHeader() {
  for (int n = 0; n < model->columnCount(QModelIndex()); n++)
    if (!model->visible(n))
      view->hideColumn(n);
  view->resizeColumnToContents(0);
  view->resizeColumnToContents(1);
  view->resizeColumnToContents(3);
}

void MainWindow::saveLayout() {
  settings->geometry(saveGeometry());
  settings->windowState(saveState());
  //settings->splitter(splitter->saveState());
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

