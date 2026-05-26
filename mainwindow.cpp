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

#include "mainwindow.h"
#include "sensorproperties.h"
#include "themeicons.h"

MainWindow::MainWindow(const QString &artistTable, QWidget *parent)
  : QMainWindow{parent}
  , model(new SensorModel(this)),
  sensor(new SensorPort(this)) {

  connect(sensor, &SensorPort::readyData, this, [this](QByteArray data) {
    qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz") << sensor->get<QString>();
  });

  QIcon::setThemeName("Material Symbols Outlined");
  QGuiApplication::styleHints()->setColorScheme(Qt::ColorScheme::Dark);
  this->setWindowIcon(QIcon::fromTheme(QIcon::ThemeIcon::NetworkWired));

  model->read();

  createMenuBar();
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

void MainWindow::closeEvent(QCloseEvent *event)
{
  QSettings settings(Company, AppName);
  settings.setValue("geometry", saveGeometry());
  settings.setValue("windowState", saveState());

  model->write();

  QMainWindow::closeEvent(event);
}

void MainWindow::createMenuBar() {
  QAction *addAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::ListAdd), tr("Добавить датчик..."), this);
  QAction *deleteAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete), tr("Удалить текущий..."), this);
  QAction *quitAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit), tr("&Quit"), this);
  QAction *aboutAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout), tr("&About"), this);
  QAction *saveAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave), tr("Сохранить"), this);
  QAction *iconsAction = new QAction(tr("Просмотр иконок..."), this);
  QAction *startAction = new QAction(tr("Запуск опроса"), this);
  QAction *stopAction = new QAction(tr("Останов опроса"), this);

  addAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_A));
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
  helpMenu->addAction(iconsAction);

  connect(addAction, &QAction::triggered, this, &MainWindow::addSensor);
  connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
  connect(saveAction, &QAction::triggered, this, &MainWindow::save);
  connect(quitAction, &QAction::triggered, this, [this]() { close(); });
  connect(iconsAction, &QAction::triggered, this, &MainWindow::showIcons);

  connect(startAction, &QAction::triggered, this, &MainWindow::start);
  connect(stopAction, &QAction::triggered, this, &MainWindow::stop);

  menuBar()->setNativeMenuBar(false);
}

void MainWindow::restoreLayout()
{
  QSettings settings(Company, AppName);
  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("windowState").toByteArray());
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

void MainWindow::about()
{
  QMessageBox::about(this,
                     tr("About Music Archive"),
                     tr("<p><b>Sensor Pool</b> программа записи параметров "
                        "обработки экспериментов с датчиков регистрации.</p>"));
}

void MainWindow::addSensor()
{
  Sensor sensor{QUuid()};
  SensorProperties *dialog = new SensorProperties(sensor, this);
  int accepted = dialog->exec();

  if (accepted == QDialog::Accepted && sensor.valid()) {
    model->add(sensor);
    view->setCurrentIndex(model->last());
    view->scrollToBottom();
  }
}

void MainWindow::editSensor()
{
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
