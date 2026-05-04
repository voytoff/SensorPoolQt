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
#include "sensorproperties.h"

MainWindow::MainWindow(const QString &artistTable, QWidget *parent)
    : QMainWindow{parent}
{
    this->setWindowIcon(QIcon(":/images/sensor.svg"));
    QGroupBox *albums = createAlbumGroupBox();

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(albums, 0, 0);
    layout->setColumnStretch(1, 1);
    layout->setColumnMinimumWidth(0, 500);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);

    createMenuBar();
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
            this, &MainWindow::addAlbum);
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

QGroupBox *MainWindow::createAlbumGroupBox()
{
    QGroupBox *box = new QGroupBox(tr("Album"));

    albumView = new QTableView;
    albumView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    albumView->setSortingEnabled(true);
    albumView->setSelectionBehavior(QAbstractItemView::SelectRows);
    albumView->setSelectionMode(QAbstractItemView::SingleSelection);
    albumView->setShowGrid(false);
    albumView->verticalHeader()->hide();
    albumView->setAlternatingRowColors(true);
    //albumView->setModel(model);
    adjustHeader();

    QLocale locale = albumView->locale();
    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    albumView->setLocale(locale);
    /*
    connect(albumView, &QTableView::clicked,
            this, &MainWindow::showAlbumDetails);
    connect(albumView, &QTableView::activated,
            this, &MainWindow::showAlbumDetails);
    */
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(albumView, 0, {});
    box->setLayout(layout);

    return box;
}

void MainWindow::addAlbum()
{
    SensorProperties *dialog = new SensorProperties(/*model, albumData, file, */this);
    int accepted = dialog->exec();

    if (accepted == QDialog::Accepted) {
        //int lastRow = model->rowCount() - 1;
        //albumView->selectRow(lastRow);
        albumView->scrollToBottom();
        //showAlbumDetails(model->index(lastRow, 0));
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Music Archive"),
                       tr("<p>The <b>Music Archive</b> example shows how to present "
                          "data from different data sources in the same application. "
                          "The album titles, and the corresponding artists and release dates, "
                          "are kept in a database, while each album's tracks are stored "
                          "in an XML file. </p><p>The example also shows how to add as "
                          "well as remove data from both the database and the "
                          "associated XML file using the API provided by the Qt SQL and "
                          "Qt XML modules, respectively.</p>"));
}

void MainWindow::adjustHeader()
{
    albumView->hideColumn(0);
    //albumView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    //albumView->resizeColumnToContents(2);
    //albumView->resizeColumnToContents(3);
}
