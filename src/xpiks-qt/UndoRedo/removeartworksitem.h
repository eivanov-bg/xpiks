/*
 * This file is a part of Xpiks - cross platform application for
 * keywording and uploading images for microstocks
 * Copyright (C) 2014-2016 Taras Kushnir <kushnirTV@gmail.com>
 *
 * Xpiks is distributed under the GNU General Public License, version 3.0
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef REMOVEARTWORKSITEM_H
#define REMOVEARTWORKSITEM_H

#include <QVector>
#include <QPair>
#include <QString>
#include <QStringList>
#include "historyitem.h"

namespace UndoRedo {
    class RemoveArtworksHistoryItem : public HistoryItem
    {
    public:
        RemoveArtworksHistoryItem(const QVector<int> &removedArtworksIndices,
                                  const QStringList &removedArtworksFilepathes):
            HistoryItem(RemovedArtworksActionType),
            m_RemovedArtworksIndices(removedArtworksIndices),
            m_RemovedArtworksPathes(removedArtworksFilepathes)
        {}

        virtual ~RemoveArtworksHistoryItem() { }

    public:
        virtual void undo(const Commands::ICommandManager *commandManagerInterface) const;

    public:
        virtual QString getDescription() const {
            int count = m_RemovedArtworksIndices.length();
            return count > 1 ? QString("%1 items removed").arg(count) :
                               QString("1 item removed");
        }

        virtual QString getActionTypeDescription() const { return QString("Remove"); }

    private:
        QVector<int> m_RemovedArtworksIndices;
        QStringList m_RemovedArtworksPathes;
    };
}

#endif // REMOVEARTWORKSITEM_H
