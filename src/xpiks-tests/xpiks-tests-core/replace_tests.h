#ifndef REPLACETEST_H
#define REPLACETEST_H

#include <QObject>
#include <QtTest/QtTest>

class ReplaceTests: public QObject
{
    Q_OBJECT
private slots:
    void replaceTrivialTest();
    void noReplaceTrivialTest();
    void caseSensitiveTest();
    void replaceTitleTest();
    void replaceKeywordsTest();
    void replaceToSpaceTest();
    void replaceToNothingTest();
    void spacesReplaceCaseSensitiveTest();
    void spaceReplaceCaseSensitiveNoReplaceTest();
    void replaceSpacesToWordsTest();
    void replaceSpacesToSpacesTest();
    void replaceKeywordsToEmptyTest();
};

#endif // REPLACETEST_H
