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

#include "updateservice.h"
#include "updatescheckerworker.h"
#include "../Common/defines.h"
#include "../Models/settingsmodel.h"

namespace Conectivity {
    UpdateService::UpdateService(Models::SettingsModel *settingsModel):
        m_UpdatesCheckerWorker(nullptr),
        m_SettingsModel(settingsModel)
    {
        Q_ASSERT(settingsModel != nullptr);
    }

    void UpdateService::startChecking() {
        const bool startWorker = m_SettingsModel->getCheckForUpdates();

        if (startWorker) {
            m_UpdatesCheckerWorker = new UpdatesCheckerWorker();
            QThread *thread = new QThread();
            m_UpdatesCheckerWorker->moveToThread(thread);

            QObject::connect(thread, SIGNAL(started()), m_UpdatesCheckerWorker, SLOT(process()));
            QObject::connect(m_UpdatesCheckerWorker, SIGNAL(stopped()), thread, SLOT(quit()));

            QObject::connect(m_UpdatesCheckerWorker, SIGNAL(stopped()), m_UpdatesCheckerWorker, SLOT(deleteLater()));
            QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

            QObject::connect(m_UpdatesCheckerWorker, SIGNAL(updateAvailable(QString)),
                             this, SIGNAL(updateAvailable(QString)));

            QObject::connect(m_UpdatesCheckerWorker, SIGNAL(stopped()),
                             this, SLOT(workerFinished()));

            thread->start();
        } else {
            LOG_INFO << "Update service disabled";
        }
    }

    void UpdateService::workerFinished() {
        LOG_DEBUG << "#";
    }
}