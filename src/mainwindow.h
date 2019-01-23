#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mpdclient.h"

#include <QMainWindow>
#include <QTableView>
#include <QLabel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slots for menu items
    void on_actionExit_triggered();
    void on_actionAboutQt_triggered();

    // Slots for the main table view
    void on_queueTableView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    MPDClient *mpd;
};

#endif // MAINWINDOW_H
