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

#ifndef IMAGECACHINGWORKER_H
#define IMAGECACHINGWORKER_H

#include "../Common/itemprocessingworker.h"
#include <QString>
#include <QHash>
#include <QDateTime>
#include <QSize>
#include <QReadWriteLock>
#include "imagecacherequest.h"

namespace QMLExtensions {
    struct CachedImage {
        QDateTime m_LastModified;
        QString m_Filename;
        QSize m_Size;
    };

    QDataStream &operator<<(QDataStream &out, const CachedImage &v);
    QDataStream &operator>>(QDataStream &in, CachedImage &v);

    class ImageCachingWorker : public QObject, public Common::ItemProcessingWorker<ImageCacheRequest>
    {
        Q_OBJECT
    protected:
        virtual bool initWorker();
        virtual bool processOneItem(ImageCacheRequest *item);

    protected:
        virtual void notifyQueueIsEmpty() { emit queueIsEmpty(); }
        virtual void workerStopped() { saveIndex(); emit stopped(); }

    public slots:
        void process() { doWork(); }
        void cancel() { stopWorking(); }

    signals:
        void stopped();
        void queueIsEmpty();

    public:
        bool tryGetCachedImage(const QString &key, const QSize &requestedSize,
                               QString &cached, bool &needsUpdate);

    private:
        void readIndex();
        void saveIndex();
        bool isProcessed(ImageCacheRequest *item);

    private:
        volatile int m_ProcessedItemsCount;
        QString m_ImagesCacheDir;
        QString m_IndexFilepath;
        QReadWriteLock m_CacheLock;
        QHash<QString, CachedImage> m_CacheIndex;
    };
}

#endif // IMAGECACHINGWORKER_H
