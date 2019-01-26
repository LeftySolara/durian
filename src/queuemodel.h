/******************************************************************************
 * queuemodel.h : data model representing the current MPD queue
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

#ifndef QUEUEMODEL_H
#define QUEUEMODEL_H

#include <QAbstractTableModel>
#include <mpd/client.h>

class QueueModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit QueueModel(QObject *parent = nullptr);
    ~QueueModel() override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void setQueue(QVector<mpd_song *> queue) { songs = queue; }

    mpd_pure int playing_id;
private:
    QVector<mpd_song *> songs;
};

#endif // QUEUEMODEL_H
