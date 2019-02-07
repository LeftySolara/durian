#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mpdclient.h"

#include <QMainWindow>
#include <QTableView>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateSongProgressBar();
    void updateSongProgressLabel();
    void updateRandomLabel();
    void updateRepeatLabel();
    void updateSingleLabel();
    void updateLabels();

private slots:
    // Slots for menu items
    void on_actionExit_triggered();
    void on_actionAboutQt_triggered();

    void on_actionPlayPause_triggered();
    void on_actionStop_triggered();

    void on_actionRandomMode_triggered();
    void on_actionRepeatMode_triggered();
    void on_actionSingleMode_triggered();

    // Slots for the main view
    void on_queueTableView_doubleClicked(const QModelIndex &index);
    void mpd_queueChanged();
    void mpd_playingTrackChanged();

    // Slots for player control buttons
    void on_buttonPlayPause_clicked();
    void on_buttonPrev_clicked();
    void on_buttonNext_clicked();
    void on_buttonStop_clicked();

private:
    Ui::MainWindow *ui;
    MPDClient *mpd;

    QLabel *label_now_playing;
    QLabel *label_time_elapsed;
    QLabel *label_random_mode;
    QLabel *label_repeat_mode;
    QLabel *label_single_mode;
    QProgressBar *progress_bar;

    QTimer *time_elapsed_timer;
};

#endif // MAINWINDOW_H
