/*
 * This file is a part of Xpiks - cross platform application for
 * keywording and uploading images for microstocks
 * Copyright (C) 2014-2017 Taras Kushnir <kushnirTV@gmail.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>
#include <QStringList>
#include <QList>
#include <QUrl>
#include <QVector>
#include <QObject>
#include <memory>
#include <vector>
#include "../UndoRedo/ihistoryitem.h"
#include "commandbase.h"
#include "../Conectivity/analyticsuserevent.h"
#include "../Common/flags.h"
#include "icommandmanager.h"
#include "../Common/iservicebase.h"
#include "../Helpers/ifilenotavailablemodel.h"
#include "../Models/metadataelement.h"
#include "../KeywordsPresets/presetkeywordsmodel.h"
#include "../KeywordsPresets/presetkeywordsmodelconfig.h"
#include "../Helpers/asynccoordinator.h"
#include "../MetadataIO/artworkmetadatasnapshot.h"

namespace Encryption {
    class SecretsManager;
}

namespace UndoRedo {
    class UndoRedoManager;
}

namespace Common {
    class BasicMetadataModel;
    class IMetadataOperator;
}

namespace Models {
    class ArtworksRepository;
    class ArtItemsModel;
    class FilteredArtItemsProxyModel;
    class CombinedArtworksModel;
    class ArtworkUploader;
    class UploadInfoRepository;
    class UploadInfo;
    class ArtworkMetadata;
    class ZipArchiver;
    class SettingsModel;
    class RecentDirectoriesModel;
    class RecentFilesModel;
    class LogsModel;
    class LanguagesModel;
    class FindAndReplaceModel;
    class DeleteKeywordsViewModel;
    class UIManager;
    class ArtworkProxyBase;
    class ArtworkProxyModel;
    class SessionManager;
    class SwitcherModel;
}

namespace Suggestion {
    class KeywordsSuggestor;
    class LocalLibrary;
}

namespace MetadataIO {
    class BackupSaverService;
    class MetadataIOCoordinator;
}

namespace SpellCheck {
    class SpellCheckerService;
    class SpellCheckSuggestionModel;
}

namespace Conectivity {
    class TelemetryService;
    class UpdateService;
    class RequestsService;
}

namespace Plugins {
    class PluginManager;
}

namespace Warnings {
    class WarningsService;
    class WarningsModel;
}

namespace QMLExtensions {
    class ColorsModel;
    class ImageCachingService;
}

namespace AutoComplete {
    class AutoCompleteService;
}

namespace Helpers {
    class HelpersQmlWrapper;
}

namespace Translation {
    class TranslationService;
    class TranslationManager;
}

namespace QuickBuffer {
    class QuickBuffer;
}

namespace Maintenance {
    class MaintenanceService;
}

namespace Commands {
    class CommandManager : public QObject, public ICommandManager
    {
        Q_OBJECT
    public:
        CommandManager();
        virtual ~CommandManager() {}

    public:
        void InjectDependency(Models::ArtworksRepository *artworkRepository);
        void InjectDependency(Models::ArtItemsModel *artItemsModel);
        void InjectDependency(Models::FilteredArtItemsProxyModel *filteredItemsModel);
        void InjectDependency(Models::CombinedArtworksModel *combinedArtworksModel);
        void InjectDependency(Models::ArtworkUploader *artworkUploader);
        void InjectDependency(Models::UploadInfoRepository *uploadInfoRepository);
        void InjectDependency(Warnings::WarningsService *warningsService);
        void InjectDependency(Encryption::SecretsManager *secretsManager);
        void InjectDependency(UndoRedo::UndoRedoManager *undoRedoManager);
        void InjectDependency(Models::ZipArchiver *zipArchiver);
        void InjectDependency(Suggestion::KeywordsSuggestor *keywordsSuggestor);
        void InjectDependency(Models::SettingsModel *settingsModel);
        void InjectDependency(Models::RecentDirectoriesModel *recentDirectories);
        void InjectDependency(Models::RecentFilesModel *recentFiles);
        void InjectDependency(SpellCheck::SpellCheckerService *spellCheckerService);
        void InjectDependency(SpellCheck::SpellCheckSuggestionModel *spellCheckSuggestionModel);
        void InjectDependency(MetadataIO::BackupSaverService *backupSaverService);
        void InjectDependency(Conectivity::TelemetryService *telemetryService);
        void InjectDependency(Conectivity::UpdateService *updateService);
        void InjectDependency(Models::LogsModel *logsModel);
        void InjectDependency(MetadataIO::MetadataIOCoordinator *metadataIOCoordinator);
        void InjectDependency(Suggestion::LocalLibrary *localLibrary);
        void InjectDependency(Plugins::PluginManager *pluginManager);
        void InjectDependency(Models::LanguagesModel *languagesModel);
        void InjectDependency(QMLExtensions::ColorsModel *colorsModel);
        void InjectDependency(AutoComplete::AutoCompleteService *autoCompleteService);
        void InjectDependency(QMLExtensions::ImageCachingService *imageCachingService);
        void InjectDependency(Models::FindAndReplaceModel *findAndReplaceModel);
        void InjectDependency(Models::DeleteKeywordsViewModel *deleteKeywordsViewModel);
        void InjectDependency(Helpers::HelpersQmlWrapper *helpersQmlWrapper);
        void InjectDependency(KeywordsPresets::PresetKeywordsModel *presetsModel);
        void InjectDependency(KeywordsPresets::PresetKeywordsModelConfig *presetsModelConfig);
        void InjectDependency(Translation::TranslationService *translationService);
        void InjectDependency(Translation::TranslationManager *translationManager);
        void InjectDependency(Models::UIManager *uiManager);
        void InjectDependency(Models::ArtworkProxyModel *artworkProxy);
        void InjectDependency(Models::SessionManager *sessionManager);
        void InjectDependency(Warnings::WarningsModel *warningsModel);
        void InjectDependency(QuickBuffer::QuickBuffer *quickBuffer);
        void InjectDependency(Maintenance::MaintenanceService *maintenanceService);
        void InjectDependency(Models::SwitcherModel *switcherModel);
        void InjectDependency(Conectivity::RequestsService *requestsService);

    private:
        int generateNextCommandID() { int id = m_LastCommandID++; return id; }

    public:
        virtual std::shared_ptr<Commands::ICommandResult> processCommand(const std::shared_ptr<ICommandBase> &command) override;
        virtual void addWarningsService(Common::IServiceBase<Common::IBasicArtwork, Common::WarningsCheckFlags> *service) override;

    public:
        void recordHistoryItem(std::unique_ptr<UndoRedo::IHistoryItem> &historyItem) const;
        void connectEntitiesSignalsSlots() const;

    public:
        void ensureDependenciesInjected();
        void removeUnavailableFiles();

    public:
        void recodePasswords(const QString &oldMasterPassword,
                                const QString &newMasterPassword,
                                const std::vector<std::shared_ptr<Models::UploadInfo> > &uploadInfos) const;

        void combineArtwork(Models::ArtworkMetadata *metadata, int index) const;
        void combineArtworks(std::vector<Models::MetadataElement> &artworks) const;
        void deleteKeywordsFromArtworks(std::vector<Models::MetadataElement> &artworks) const;
        void setArtworksForUpload(const QVector<Models::ArtworkMetadata*> &artworks) const;
        void setArtworksForZipping(const QVector<Models::ArtworkMetadata*> &artworks) const;
        virtual void connectArtworkSignals(Models::ArtworkMetadata *metadata) const;
        void disconnectArtworkSignals(Models::ArtworkMetadata *metadata) const;
        void readMetadata(const QVector<Models::ArtworkMetadata*> &artworks,
                          const QVector<QPair<int, int> > &rangesToUpdate) const;
        void writeMetadata(const QVector<Models::ArtworkMetadata*> &artworks, bool useBackups) const;
        void addToLibrary(std::unique_ptr<MetadataIO::ArtworksSnapshot> &artworksSnapshot) const;
        void updateArtworks(const QVector<int> &indices) const;
        void updateArtworks(const QVector<QPair<int, int> > &rangesToUpdate) const;
        void addToRecentDirectories(const QString &path) const;
        void addToRecentFiles(const QString &path) const;
        void addToRecentFiles(const QStringList &filenames) const;
        void autoDiscoverExiftool() const;

    public:
        void generatePreviews(const QVector<Models::ArtworkMetadata*> &items) const;
        void submitKeywordForSpellCheck(Common::BasicKeywordsModel *item, int keywordIndex) const;
        void submitForSpellCheck(const QVector<Models::ArtworkMetadata*> &items) const;
        void submitForSpellCheck(const QVector<Common::BasicKeywordsModel *> &items) const;
        void submitItemForSpellCheck(Common::BasicKeywordsModel *item, Common::SpellCheckFlags flags = Common::SpellCheckFlags::All) const;
        void setupSpellCheckSuggestions(Common::IMetadataOperator *item, int index, Common::SuggestionFlags flags);
        void setupSpellCheckSuggestions(std::vector<std::pair<Common::IMetadataOperator *, int> > &itemPairs, Common::SuggestionFlags flags);
        void submitForSpellCheck(const QVector<Common::BasicKeywordsModel *> &items, const QStringList &wordsToCheck) const;

    public:
        void submitKeywordsForWarningsCheck(Models::ArtworkMetadata *item) const;
        void submitForWarningsCheck(Models::ArtworkMetadata *item, Common::WarningsCheckFlags flags = Common::WarningsCheckFlags::All) const;
        void submitForWarningsCheck(const QVector<Models::ArtworkMetadata*> &items) const;

    private:
        void submitForWarningsCheck(const QVector<Common::IBasicArtwork *> &items) const;

    public:
        void saveArtworkBackup(Models::ArtworkMetadata *metadata) const;
        void saveArtworksBackups(const QVector<Models::ArtworkMetadata *> &artworks) const;
        void reportUserAction(Conectivity::UserAction userAction) const;
        void cleanupLocalLibraryAsync() const;
        void afterConstructionCallback();

    private:
        void afterInnerServicesInitialized();
        void executeMaintenanceJobs();
        void readSession();
        int restoreReadSession();

    public:
#ifdef INTEGRATION_TESTS
        int restoreSessionForTest();
#endif
        void saveSession() const;
        void saveSessionInBackground();

    public:
        void beforeDestructionCallback() const;
        void requestCloseApplication() const;
        void restartSpellChecking();
#ifndef CORE_TESTS
        void autoCompleteKeyword(const QString &keyword, QObject *notifyObject) const;
#endif

#ifdef INTEGRATION_TESTS
        void cleanup();
#endif

    private slots:
        void servicesInitialized(int status);

    public:
        void registerCurrentItem(const Models::MetadataElement &metadataElement);
        void registerCurrentItem(Models::ArtworkProxyBase *artworkProxy) const;
        void clearCurrentItem() const;

    public:
        // methods for getters
        Helpers::AsyncCoordinator &getInitCoordinator() { return m_InitCoordinator; }
        virtual Models::ArtworksRepository *getArtworksRepository() const { return m_ArtworksRepository; }
        virtual Models::ArtItemsModel *getArtItemsModel() const { return m_ArtItemsModel; }
        virtual Encryption::SecretsManager *getSecretsManager() const { return m_SecretsManager; }
        virtual Models::UploadInfoRepository *getUploadInfoRepository() { return m_UploadInfoRepository; }
        virtual Suggestion::KeywordsSuggestor *getKeywordsSuggestor() const { return m_KeywordsSuggestor; }
        virtual Models::SettingsModel *getSettingsModel() const { return m_SettingsModel; }
        virtual SpellCheck::SpellCheckerService *getSpellCheckerService() const { return m_SpellCheckerService; }
        virtual MetadataIO::BackupSaverService *getBackupSaverService() const { return m_MetadataSaverService; }
        virtual UndoRedo::UndoRedoManager *getUndoRedoManager() const { return m_UndoRedoManager; }
        virtual QMLExtensions::ColorsModel *getColorsModel() const { return m_ColorsModel; }
        virtual Models::FilteredArtItemsProxyModel *getFilteredArtItemsModel() const { return m_FilteredItemsModel; }
        virtual Models::LogsModel *getLogsModel() const { return m_LogsModel; }
        virtual Models::ArtworkUploader *getArtworkUploader() const { return m_ArtworkUploader; }
        virtual Models::ZipArchiver *getZipArchiver() const { return m_ZipArchiver; }
        virtual Models::DeleteKeywordsViewModel *getDeleteKeywordsModel() const { return m_DeleteKeywordsViewModel; }
        virtual SpellCheck::SpellCheckSuggestionModel *getSpellSuggestionsModel() const { return m_SpellCheckSuggestionModel; }
        virtual KeywordsPresets::PresetKeywordsModel *getPresetsModel() const { return m_PresetsModel; }
        virtual KeywordsPresets::PresetKeywordsModelConfig *getPresetsModelConfig() const { return m_PresetsModelConfig; }
        virtual Translation::TranslationService *getTranslationService() const { return m_TranslationService; }
        virtual Models::UIManager *getUIManager() const { return m_UIManager; }
        virtual QuickBuffer::QuickBuffer *getQuickBuffer() const { return m_QuickBuffer; }
        virtual Models::RecentDirectoriesModel *getRecentDirectories() const { return m_RecentDirectories; }
        virtual Models::RecentFilesModel *getRecentFiles() const { return m_RecentFiles; }
        virtual Maintenance::MaintenanceService *getMaintenanceService() const { return m_MaintenanceService; }
        virtual Models::SessionManager *getSessionManager() const { return m_SessionManager; }
        virtual Warnings::WarningsModel *getWarningsModel() const { return m_WarningsModel; }
        virtual Models::SwitcherModel *getSwitcherModel() const { return m_SwitcherModel; }
        virtual Conectivity::RequestsService *getRequestsService() const { return m_RequestsService; }

#ifdef INTEGRATION_TESTS
        virtual Translation::TranslationManager *getTranslationManager() const { return m_TranslationManager; }
        virtual MetadataIO::MetadataIOCoordinator *getMetadataIOCoordinator() const { return m_MetadataIOCoordinator; }
        virtual Models::CombinedArtworksModel *getCombinedArtworksModel() const { return m_CombinedArtworksModel; }
        virtual AutoComplete::AutoCompleteService *getAutoCompleteService() const { return m_AutoCompleteService; }
        virtual Warnings::WarningsService *getWarningsService() const { return m_WarningsService; }
        virtual Models::FindAndReplaceModel *getFindAndReplaceModel() const { return m_FindAndReplaceModel; }
#endif

    private:
        Helpers::AsyncCoordinator m_InitCoordinator;
        Models::ArtworksRepository *m_ArtworksRepository;
        Models::ArtItemsModel *m_ArtItemsModel;
        Models::FilteredArtItemsProxyModel *m_FilteredItemsModel;
        Models::CombinedArtworksModel *m_CombinedArtworksModel;
        Models::ArtworkUploader *m_ArtworkUploader;
        Models::UploadInfoRepository *m_UploadInfoRepository;
        Warnings::WarningsService *m_WarningsService;
        Encryption::SecretsManager *m_SecretsManager;
        UndoRedo::UndoRedoManager *m_UndoRedoManager;
        Models::ZipArchiver *m_ZipArchiver;
        Suggestion::KeywordsSuggestor *m_KeywordsSuggestor;
        Models::SettingsModel *m_SettingsModel;
        Models::RecentDirectoriesModel *m_RecentDirectories;
        Models::RecentFilesModel *m_RecentFiles;
        SpellCheck::SpellCheckerService *m_SpellCheckerService;
        SpellCheck::SpellCheckSuggestionModel *m_SpellCheckSuggestionModel;
        MetadataIO::BackupSaverService *m_MetadataSaverService;
        Conectivity::TelemetryService *m_TelemetryService;
        Conectivity::UpdateService *m_UpdateService;
        Models::LogsModel *m_LogsModel;
        Suggestion::LocalLibrary *m_LocalLibrary;
        MetadataIO::MetadataIOCoordinator *m_MetadataIOCoordinator;
        Plugins::PluginManager *m_PluginManager;
        Models::LanguagesModel *m_LanguagesModel;
        QMLExtensions::ColorsModel *m_ColorsModel;
        AutoComplete::AutoCompleteService *m_AutoCompleteService;
        QMLExtensions::ImageCachingService *m_ImageCachingService;
        Models::DeleteKeywordsViewModel *m_DeleteKeywordsViewModel;
        Models::FindAndReplaceModel *m_FindAndReplaceModel;
        Helpers::HelpersQmlWrapper *m_HelpersQmlWrapper;
        KeywordsPresets::PresetKeywordsModel *m_PresetsModel;
        KeywordsPresets::PresetKeywordsModelConfig *m_PresetsModelConfig;
        Translation::TranslationService *m_TranslationService;
        Translation::TranslationManager *m_TranslationManager;
        Models::UIManager *m_UIManager;
        Models::ArtworkProxyModel *m_ArtworkProxyModel;
        Models::SessionManager *m_SessionManager;
        Warnings::WarningsModel *m_WarningsModel;
        QuickBuffer::QuickBuffer *m_QuickBuffer;
        Maintenance::MaintenanceService *m_MaintenanceService;
        Models::SwitcherModel *m_SwitcherModel;
        Conectivity::RequestsService *m_RequestsService;

        QVector<Common::IServiceBase<Common::IBasicArtwork, Common::WarningsCheckFlags> *> m_WarningsCheckers;
        QVector<Helpers::IFileNotAvailableModel*> m_AvailabilityListeners;

        volatile bool m_ServicesInitialized;
        volatile bool m_AfterInitCalled;
        volatile int m_LastCommandID;
    };
}

#endif // COMMANDMANAGER_H
