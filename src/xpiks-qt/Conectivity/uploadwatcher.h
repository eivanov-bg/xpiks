#ifndef UPLOADWATCHER_H
#define UPLOADWATCHER_H

#include <QAbstractListModel>
#include <QVector>
#include <QPair>

namespace Conectivity {
    class UploadWatcher:
        public QAbstractListModel
    {
    Q_OBJECT
    Q_PROPERTY(int failedImagesCount READ getFailedImagesCount NOTIFY failedImagesCountChanged)

    public:
        UploadWatcher(QObject *parent=0);

    public:
        int getFailedImagesCount() const { return m_FailedImagesCount; }

    public:
        Q_INVOKABLE void resetModel();

    public:
        enum UploadWatcher_Roles {
            FtpAddressRole = Qt::UserRole + 1,
            FailedImagesRole
        };

    public:
        virtual int rowCount(const QModelIndex &parent) const override;
        virtual QVariant data(const QModelIndex &index, int role) const override;

    public slots:
        void reportUploadErrorHandler(const QString &filepath, const QString &host);

    signals:
        void failedImagesCountChanged();

    protected:
        virtual QHash<int, QByteArray> roleNames() const override;

    private:
        QVector<QPair<QString, QStringList> > m_FtpInfo;
        int m_FailedImagesCount;
    };
}
#endif // UPLOADWATCHER_H
