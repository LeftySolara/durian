/******************************************************************************
 * mpdclient.cpp : an object representing an MPD connection
 * ****************************************************************************
 * Copyright (C) 2019 Jalen Adams
 *
 * Authors: Jalen Adams <leftysolara@gmail.com>
 *
 * This file is part of Durian.
 *
 * Durian is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Durian is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Durian.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "mpdclient.h"

MPDClient::MPDClient(QString host, unsigned int port, unsigned int timeout) :
    max_playlist_length(16384)  // Max length defined by the MPD library
{
    this->host = host;
    this->port = port;
    this->timeout = timeout;

    this->queue_model = new QueueModel();

    // Create a timer to periodically check for updates to the MPD database.
    // This will allow Durian to reflect changes made from other clients.
    this->timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &MPDClient::update);
    timer->start();

    this->connection = mpd_connection_new(host.toStdString().c_str(), port, timeout);
    this->last_error = mpd_connection_get_error(connection);

    if (last_error == MPD_ERROR_SUCCESS) {
        qInfo("Connected to MPD.");
        fetchQueue();
    }
    else {
        const char *error_msg = mpd_connection_get_error_message(connection);
        qCritical("Could not connect to MPD. Last error: %s", error_msg);
    }
}

MPDClient::~MPDClient()
{
    if (connection) {
        mpd_connection_free(connection);
    }
    if (status) {
        mpd_status_free(status);
    }

    delete queue_model;
}

void MPDClient::update()
{
    mpd_pure unsigned old_queue_version = queue_version;
    mpd_pure int old_playing_id = playing_id;

    status = mpd_run_status(connection);
    state = mpd_status_get_state(status);
    last_error = mpd_connection_get_error(connection);
    queue_version = mpd_status_get_queue_version(status);
    playing_id = mpd_status_get_song_id(status);
    queue_model->playing_id = playing_id;

    if (last_error != MPD_ERROR_SUCCESS) {
        const char *error_msg = mpd_connection_get_error_message(connection);
        qWarning("MPD Error: %s", error_msg);
    }

    if (old_queue_version != queue_version) {
        emit queueChanged();
    }
    if (old_playing_id != playing_id) {
        emit playingTrackChanged();
    }
}

void MPDClient::fetchQueue()
{
    if (!connection) {
        return;
    }

    QVector<mpd_song *> songs;
    struct mpd_song *song;

    mpd_send_list_queue_meta(connection);
    while ((song = mpd_recv_song(connection))) {
        songs.append(song);
    }

    mpd_response_finish(connection);
    update();
    queue_model->setQueue(songs);
}

void MPDClient::playQueuePos(int pos)
{
    if (pos < 0) {
        return;
    }

    mpd_run_play_pos(connection, pos);
    update();
    emit playingTrackChanged();
}

void MPDClient::togglePause()
{
    if (state == MPD_STATE_STOP || state == MPD_STATE_UNKNOWN) {
        return;
    }

    mpd_run_toggle_pause(connection);
    last_error = mpd_connection_get_error(connection);
}

void MPDClient::playPrev()
{
    if (state == MPD_STATE_STOP || state == MPD_STATE_UNKNOWN) {
        return;
    }

    mpd_run_previous(connection);
    update();
    emit playingTrackChanged();
}

void MPDClient::playNext()
{
    if (state == MPD_STATE_STOP || state == MPD_STATE_UNKNOWN) {
        return;
    }

    mpd_run_next(connection);
    update();
    emit playingTrackChanged();
}

void MPDClient::stop()
{
    if (state == MPD_STATE_STOP || state == MPD_STATE_UNKNOWN) {
        return;
    }

    mpd_run_stop(connection);
    update();
    emit playingTrackChanged();
}

void MPDClient::toggleRandom()
{
    mpd_run_random(connection, !randomModeActive());
}

void MPDClient::toggleRepeat()
{
    mpd_run_repeat(connection, !repeatModeActive());
}

void MPDClient::toggleSingle()
{
    mpd_run_single(connection, !singleModeActive());
}

void MPDClient::toggleConsume()
{
    mpd_run_consume(connection, !consumeModeActive());
}

// Get the value of a tag from the currently playing song.
// Mainly used for the status bar in the main window.
QString MPDClient::getCurrentSongTag(mpd_tag_type tag)
{
    if (playing_id == -1) {
        return QString();
    }
    struct mpd_song *current_song = mpd_run_current_song(connection);
    QString value = mpd_song_get_tag(current_song, tag, 0);

    return value;
}

unsigned MPDClient::getCurrentSongDuration()
{
    if (playing_id == -1) {
        return 0;
    }
    struct mpd_song *current_song = mpd_run_current_song(connection);
    return mpd_song_get_duration(current_song);
}

unsigned MPDClient::getCurrentSongElapsed()
{
    if (playing_id == -1) {
        return 0;
    }
    else {
        return mpd_status_get_elapsed_time(status);
    }
}

bool MPDClient::isStopped()
{
    return (state == MPD_STATE_STOP || state == MPD_STATE_UNKNOWN);
}

bool MPDClient::randomModeActive()
{
    return mpd_status_get_random(status);
}

bool MPDClient::repeatModeActive()
{
    return mpd_status_get_repeat(status);
}

bool MPDClient::singleModeActive()
{
    return mpd_status_get_single(status);
}

bool MPDClient::consumeModeActive()
{
    return mpd_status_get_consume(status);
}
