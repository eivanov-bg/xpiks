#include "loghighlighter.h"
#include <QColor>

namespace Helpers {
    LogHighlighter::LogHighlighter(QTextDocument *document) :
        QSyntaxHighlighter(document){
        //from Colors.js
        m_DestructiveColor = QColor::fromRgb(209, 11, 11);
        m_ArtworkModifiedColor  = QColor::fromRgb(244,156,18);
        m_DefaultInputBackground = QColor::fromRgb(153,153,153);
    }

    void LogHighlighter::highlightBlock(const QString &text){
        int size = text.size();
        QString word = text.mid(24, 24+11);
        if (word.startsWith(QLatin1Literal("- Debug:"))){
            setFormat(0, size, m_DefaultInputBackground);
        }
        if (word.startsWith(QLatin1Literal("- Info:")));

        if (word.startsWith(QLatin1Literal("- Warning:"))){
            setFormat(0, size, m_ArtworkModifiedColor);
        }
        if (word.startsWith(QLatin1Literal("- Critical:"))){
            setFormat(0, size, m_DestructiveColor);
        }
        if (word.startsWith(QLatin1Literal("- Fatal:"))){
            setFormat(0, size, m_DestructiveColor);
        }

        return;
    }
}
