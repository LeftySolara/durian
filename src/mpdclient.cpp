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

MPDClient::MPDClient(QString host, unsigned int port, unsigned int timeout)
{
    this->host = host;
    this->port = port;
    this->timeout = timeout;

    this->connection = mpd_connection_new(host.toStdString().c_str(), port, timeout);
    this->last_error = mpd_connection_get_error(connection);
}

MPDClient::~MPDClient()
{
    if (connection) {
        mpd_connection_free(connection);
    }
}
