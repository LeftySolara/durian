/******************************************************************************
 * queuemodel.cpp : data model representing the current MPD queue
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

#include "queuemodel.h"
#include <QFont>

QueueModel::QueueModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QueueModel::~QueueModel()
{
    for (mpd_song *song : songs) {
        if (song) {
            mpd_song_free(song);
        }
    }
    songs.clear();
}

QVariant QueueModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        mpd_tag_type tag = static_cast<mpd_tag_type>(section);
        return QVariant(mpd_tag_name(tag));
    }

    return QVariant();
}

int QueueModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return songs.size();
}

int QueueModel::columnCount(const QModelIndex &parent) const
{
    // The displayed columns are just the possible tags for each song.
    return MPD_TAG_COUNT;
}

QVariant QueueModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    mpd_song *song = songs[index.row()];
    if (role == Qt::DisplayRole) {
        mpd_tag_type tag = static_cast<mpd_tag_type>(index.column());
        QString data = mpd_song_get_tag(song, tag, 0);

        return data;
    }
    else if (role == Qt::FontRole) {
        QFont font;
        unsigned int song_id = mpd_song_get_id(song);
        bool is_bold = (song_id == playing_id);
        font.setBold(is_bold);

        return font;
    }
    else {
        return QVariant();
    }
}

bool QueueModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row);
    // TODO: Implement me!
    endInsertRows();

    return false;
}

bool QueueModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // TODO: Implement me!
    endRemoveRows();

    return false;
}
