#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mpd = new MPDClient();
    ui->queueTableView->setModel(mpd->queue_model);

    QObject::connect(mpd, &MPDClient::queueChanged,
                     this, &MainWindow::mpd_queueChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mpd;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionAboutQt_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_queueTableView_doubleClicked(const QModelIndex &index)
{
    mpd->playQueuePos(index.row());
}

void MainWindow::mpd_queueChanged()
{
    ui->queueTableView->viewport()->update();
}

void MainWindow::on_buttonPlayPause_clicked()
{
    mpd->togglePause();
}

void MainWindow::on_buttonPrev_clicked()
{
    mpd->playPrev();
}

void MainWindow::on_buttonNext_clicked()
{
    mpd->playNext();
}

void MainWindow::on_buttonStop_clicked()
{
    mpd->stop();
}
