#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mpd = new MPDClient();
    testLabel = ui->labelTest;

    if (mpd->last_error == MPD_ERROR_SUCCESS) {
        testLabel->setText("Connection successful");
    }
    else {
        testLabel->setText("Failed to connect to MPD");
    }
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
