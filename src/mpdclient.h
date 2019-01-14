/******************************************************************************
 * mpdclient.h : an object representing an MPD connection
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


#ifndef MPDCLIENT_H
#define MPDCLIENT_H

#include <mpd/client.h>
#include <QObject>

#define MPD_DEFAULT_HOST "localhost"
#define MPD_DEFAULT_PORT 6600
#define MPD_DEFAULT_TIMEOUT 30000

class MPDClient : public QObject
{
    Q_OBJECT

public:
    MPDClient(QString host = MPD_DEFAULT_HOST, unsigned int port = MPD_DEFAULT_PORT,
              unsigned int timeout = MPD_DEFAULT_TIMEOUT);
    virtual ~MPDClient();

    enum mpd_error last_error; /* TODO: make this private once initial testing is done */

private:
    struct mpd_connection *connection;

    QString host;
    unsigned int port;
    unsigned int timeout;

};

#endif // MPDCLIENT_H
