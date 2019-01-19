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
