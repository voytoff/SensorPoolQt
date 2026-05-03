#include "mainwindow.h"
#include <QApplication>
#include <QComboBox>
#include <QHeaderView>
#include <QFile>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QTableView>

MainWindow::MainWindow(const QString &artistTable, QWidget *parent)
    : QMainWindow{parent}
{
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
    QAction *addAction = new QAction(tr("&Add album..."), this);
    QAction *deleteAction = new QAction(tr("&Delete album..."), this);
    QAction *quitAction = new QAction(tr("&Quit"), this);
    QAction *aboutAction = new QAction(tr("&About"), this);
    QAction *aboutQtAction = new QAction(tr("About &Qt"), this);

    addAction->setShortcut(tr("Ctrl+A"));
    deleteAction->setShortcut(tr("Ctrl+D"));
    quitAction->setShortcuts(QKeySequence::Quit);

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(addAction);
    fileMenu->addAction(deleteAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
/*
    connect(addAction, &QAction::triggered,
            this, &MainWindow::addAlbum);
    connect(deleteAction, &QAction::triggered,
            this, &MainWindow::deleteAlbum);
    connect(quitAction, &QAction::triggered,
            qApp, &QCoreApplication::quit);
    connect(aboutAction, &QAction::triggered,
            this, &MainWindow::about);
    connect(aboutQtAction, &QAction::triggered,
            qApp, &QApplication::aboutQt);
*/
}

QGroupBox* MainWindow::createAlbumGroupBox()
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
    layout->addWidget(albumView, 0, { });
    box->setLayout(layout);

    return box;
}
void MainWindow::adjustHeader()
{
    albumView->hideColumn(0);
    albumView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    albumView->resizeColumnToContents(2);
    albumView->resizeColumnToContents(3);
}
