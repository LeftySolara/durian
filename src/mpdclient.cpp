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
    if (current_song) {
        mpd_song_free(current_song);
    }

    delete queue_model;
}

void MPDClient::update()
{
    unsigned int old_queue_version = queue_version;
    unsigned int old_playing_id = 0;
    if (current_song) {
        old_playing_id = mpd_song_get_id(current_song);
    }

    status = mpd_run_status(connection);
    current_song = mpd_run_current_song(connection);
    state = mpd_status_get_state(status);
    last_error = mpd_connection_get_error(connection);
    queue_version = mpd_status_get_queue_version(status);
    unsigned int playing_id = mpd_song_get_id(current_song);

    if (state == MPD_STATE_PLAY || state == MPD_STATE_PAUSE) {
        queue_model->playing_id = playing_id;
    }
    else {
        queue_model->playing_id = 0;
    }

    if (old_queue_version != queue_version || old_playing_id != playing_id) {
        emit queueChanged();
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
    status = mpd_run_status(connection);
    queue_version = mpd_status_get_queue_version(status);
    queue_model->setQueue(songs);
}

void MPDClient::playQueuePos(int pos)
{
    if (pos < 0) {
        return;
    }

    mpd_run_play_pos(connection, pos);
    update();
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

    // mpd_run_previous() doesn't change the internal queue version,
    // but we still want to tell the view to update
    emit queueChanged();
}

void MPDClient::playNext()
{
    if (state == MPD_STATE_STOP || state == MPD_STATE_UNKNOWN) {
        return;
    }

    mpd_run_next(connection);
    update();

    // mpd_run_next() doesn't change the internal queue version,
    // but we still want to tell the view to update
    emit queueChanged();
}

void MPDClient::stop()
{
    if (state == MPD_STATE_STOP || state == MPD_STATE_UNKNOWN) {
        return;
    }

    mpd_run_stop(connection);
    update();

    // mpd_run_stop() doesn't change the internal queue version,
    // but we still want to tell the view to update
    emit queueChanged();
}
