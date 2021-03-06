#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mpd = new MPDClient();
    ui->queueTableView->setModel(mpd->queue_model);

    QString now_playing_string = mpd->getCurrentSongTag(MPD_TAG_ARTIST)
            + " - " + mpd->getCurrentSongTag(MPD_TAG_TITLE);

    label_now_playing = new QLabel(now_playing_string);
    label_time_elapsed = new QLabel();
    label_random_mode = new QLabel();
    label_repeat_mode = new QLabel();
    label_single_mode = new QLabel();
    label_consume_mode = new QLabel();

    progress_bar = ui->progressBar;
    progress_bar->setMinimum(0);
    progress_bar->setTextVisible(false);

    updateLabels();

    statusBar()->addPermanentWidget(label_now_playing);
    statusBar()->addPermanentWidget(label_time_elapsed);
    statusBar()->addPermanentWidget(label_random_mode);
    statusBar()->addPermanentWidget(label_repeat_mode);
    statusBar()->addPermanentWidget(label_single_mode);
    statusBar()->addPermanentWidget(label_consume_mode);

    time_elapsed_timer = new QTimer(this);
    QObject::connect(time_elapsed_timer, &QTimer::timeout,
                     this, &MainWindow::updateLabels);
    QObject::connect(mpd, &MPDClient::queueChanged,
                     this, &MainWindow::mpd_queueChanged);
    QObject::connect(mpd, &MPDClient::playingTrackChanged,
                     this, &MainWindow::mpd_playingTrackChanged);

    time_elapsed_timer->start(500);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mpd;
    delete label_now_playing;
    delete label_time_elapsed;
    delete label_random_mode;
    delete label_repeat_mode;
    delete label_single_mode;
    delete label_consume_mode;
    delete time_elapsed_timer;
}

void MainWindow::updateSongProgressBar()
{
    mpd_pure unsigned song_length = mpd->getCurrentSongDuration();
    mpd_pure unsigned time_elapsed = mpd->getCurrentSongElapsed();

    progress_bar->setMaximum(song_length);
    progress_bar->setValue(time_elapsed);
}

void MainWindow::updateSongProgressLabel()
{
    mpd_pure unsigned song_length = mpd->getCurrentSongDuration();
    mpd_pure unsigned time_elapsed = mpd->getCurrentSongElapsed();

    int total_minutes = song_length / 60;
    int total_seconds = song_length % 60;
    int elapsed_minutes = time_elapsed / 60;
    int elapsed_seconds = time_elapsed % 60;

    QString label = QString::asprintf("[%d:%02d/%d:%02d]",
                   elapsed_minutes, elapsed_seconds,
                   total_minutes, total_seconds);

    label_time_elapsed->setText(label);
}

// Wrapper for updating all UI labels
void MainWindow::updateLabels()
{
    updateSongProgressBar();
    updateSongProgressLabel();
    updateRandomLabel();
    updateRepeatLabel();
    updateSingleLabel();
    updateConsumeLabel();
}

void MainWindow::updateRandomLabel()
{
    bool random_active = mpd->randomModeActive();
    if (random_active) {
        label_random_mode->setText("Random Mode: on");
    }
    else {
        label_random_mode->setText("Random Mode: off");
    }
    ui->actionRandomMode->setChecked(random_active);
}

void MainWindow::updateRepeatLabel()
{
    bool repeat_active = mpd->repeatModeActive();
    if (repeat_active) {
        label_repeat_mode->setText("Repeat Mode: on");
    }
    else {
        label_repeat_mode->setText("Repeat Mode: off");
    }
    ui->actionRepeatMode->setChecked(repeat_active);
}

void MainWindow::updateSingleLabel()
{
    bool single_active = mpd->singleModeActive();
    if (single_active) {
        label_single_mode->setText("Single Mode: on");
    }
    else {
        label_single_mode->setText("Single Mode: off");
    }
    ui->actionSingleMode->setChecked(single_active);
}

void MainWindow::updateConsumeLabel()
{
    bool consume_active = mpd->consumeModeActive();
    if (consume_active) {
        label_consume_mode->setText("Consume Mode: on");
    }
    else {
        label_consume_mode->setText("Consume Mode: off");
    }
    ui->actionConsumeMode->setChecked(consume_active);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionAboutQt_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_actionPlayPause_triggered()
{
    mpd->togglePause();
}

void MainWindow::on_actionStop_triggered()
{
    mpd->stop();
}

void MainWindow::on_actionRandomMode_triggered()
{
    mpd->toggleRandom();
}

void MainWindow::on_actionRepeatMode_triggered()
{
    mpd->toggleRepeat();
}

void MainWindow::on_actionSingleMode_triggered()
{
    mpd->toggleSingle();
}

void MainWindow::on_actionConsumeMode_triggered()
{
    mpd->toggleConsume();
}

void MainWindow::on_queueTableView_doubleClicked(const QModelIndex &index)
{
    mpd->playQueuePos(index.row());
}

void MainWindow::mpd_queueChanged()
{
    ui->queueTableView->viewport()->update();
}

void MainWindow::mpd_playingTrackChanged()
{
    QString str_now_playing;
    if (mpd->isStopped()) {
        str_now_playing = "";
    }
    else {
        str_now_playing = mpd->getCurrentSongTag(MPD_TAG_ARTIST)
                + " - " + mpd->getCurrentSongTag(MPD_TAG_TITLE);
    }

    label_now_playing->setText(str_now_playing);
    progress_bar->setMaximum(mpd->getCurrentSongDuration());
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
