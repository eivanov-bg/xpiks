/*
 * This file is a part of Xpiks - cross platform application for
 * keywording and uploading images for microstocks
 * Copyright (C) 2014-2017 Taras Kushnir <kushnirTV@gmail.com>
 *
 * Xpiks is distributed under the GNU Lesser General Public License, version 3.0
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "writingorchestrator.h"
#include <QVector>
#include <QThread>
#include "../Helpers/indiceshelper.h"
#include "../Models/artworkmetadata.h"
#include "../Common/defines.h"
#include "exiv2writingworker.h"

#if defined(TRAVIS_CI)
#define MIN_SPLIT_COUNT 100
#else
#ifdef QT_DEBUG
#define MIN_SPLIT_COUNT 1
#else
#define MIN_SPLIT_COUNT 15
#endif
#endif

#define MAX_WRITING_THREADS 10
#define MIN_WRITING_THREADS 1

namespace MetadataIO {
    WritingOrchestrator::WritingOrchestrator(const QVector<Models::ArtworkMetadata *> &itemsToWrite,
                                             QObject *parent) :
        QObject(parent),
        m_ItemsToWrite(itemsToWrite),
        m_ThreadsCount(MIN_WRITING_THREADS),
        m_FinishedCount(0),
        m_AnyError(false)
    {
        int size = itemsToWrite.size();
        if (size >= MIN_SPLIT_COUNT) {
            int idealThreadCount = qMin(qMax(QThread::idealThreadCount(), MIN_WRITING_THREADS), MAX_WRITING_THREADS);
            m_ThreadsCount = qMin(size, idealThreadCount);

            m_ThreadsCount = Helpers::splitIntoChunks<Models::ArtworkMetadata*>(itemsToWrite, m_ThreadsCount, m_SlicedItemsToWrite);
        } else {
            m_SlicedItemsToWrite.push_back(itemsToWrite);
        }

        LOG_INFO << "Using" << m_ThreadsCount << "threads for" << size << "items to read";
    }

    WritingOrchestrator::~WritingOrchestrator() {
        LOG_DEBUG << "destroyed";
    }

    void WritingOrchestrator::startWriting() {
        LOG_DEBUG << "#";

        int size = m_SlicedItemsToWrite.size();
        for (int i = 0; i < size; ++i) {
            const QVector<Models::ArtworkMetadata *> &itemsToWrite = m_SlicedItemsToWrite.at(i);

            Exiv2WritingWorker *worker = new Exiv2WritingWorker(i, itemsToWrite);

            QThread *thread = new QThread();
            worker->moveToThread(thread);

            QObject::connect(thread, &QThread::started, worker, &Exiv2WritingWorker::process);
            QObject::connect(worker, &Exiv2WritingWorker::stopped, thread, &QThread::quit);

            QObject::connect(worker, &Exiv2WritingWorker::stopped, worker, &Exiv2WritingWorker::deleteLater);
            QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);

            QObject::connect(worker, &Exiv2WritingWorker::finished, this, &WritingOrchestrator::onWorkerFinished);

            thread->start();

            LOG_INFO << "Started worker" << i;
        }

        emit allStarted();
    }

    void WritingOrchestrator::dismiss() {
        this->deleteLater();
    }

    void WritingOrchestrator::onWorkerFinished(bool anyError) {
        Exiv2WritingWorker *worker = qobject_cast<Exiv2WritingWorker *>(sender());
        Q_ASSERT(worker != NULL);

        LOG_INFO << "#" << worker->getWorkerIndex() << "anyError:" << anyError;

        worker->dismiss();

        if (m_FinishedCount.fetchAndAddOrdered(1) == (m_ThreadsCount - 1)) {
            LOG_DEBUG << "Last worker finished";
            emit allFinished(!m_AnyError);
        }
    }
}
