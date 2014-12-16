#include "combinedartworksmodel.h"

namespace Models {
    CombinedArtworksModel::CombinedArtworksModel(QObject *parent)
    {
    }

    void CombinedArtworksModel::resetModelData()
    {
        this->beginResetModel();
        m_ImagesFilenames.clear();
        this->endResetModel();

        setDescription("");
        setKeywords(QStringList());
    }

    void CombinedArtworksModel::removeKeywordAt(int keywordIndex)
    {
        if (keywordIndex >= 0 && keywordIndex < m_CommonKeywords.length()) {
            m_CommonKeywords.removeAt(keywordIndex);
            emit keywordsChanged();
        }
    }

    void CombinedArtworksModel::appendKeyword(const QString &keyword)
    {
        m_CommonKeywords.append(keyword);
        emit keywordsChanged();
    }

    int CombinedArtworksModel::rowCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return m_ImagesFilenames.count();
    }

    QVariant CombinedArtworksModel::data(const QModelIndex &index, int role) const
    {
        if (index.row() < 0 || index.row() >= m_ImagesFilenames.count())
            return QVariant();

        const QString &filename = m_ImagesFilenames.at(index.row());

        switch (role) {
        case PathRole:
            return filename;
        default:
            return QVariant();
        }
    }

    QHash<int, QByteArray> CombinedArtworksModel::roleNames() const
    {
        QHash<int, QByteArray> roles;
        roles[PathRole] = "path";
        return roles;
    }
}
