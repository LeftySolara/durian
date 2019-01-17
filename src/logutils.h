/******************************************************************************
 * logutils.h : functions for handling logging functionality
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

#ifndef LOGUTILS_H
#define LOGUTILS_H

#include <QApplication>
#include <QStandardPaths>
#include <QFile>

namespace LogUtils
{
    const QString log_file_name = "durian.log";
    const QString log_dir_name = QStandardPaths::writableLocation(
                QStandardPaths::DataLocation) + "/durian";

    bool initLogging();
    void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
}


#endif // LOGUTILS_H
