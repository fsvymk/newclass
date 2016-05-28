#ifndef MODULE_H
#define MODULE_H

#include <QApplication>
#include <variable.h>

class module
{
public:
    QStringList             code;

    QByteArray              blockHeader;
    QByteArray              blockA6;
    QByteArray              block18;
    QByteArray              compiled;

    QList<variable>         variables;
    QStringList             indexBase;
    QString                 compiledHex;
    QString                 name;

    // block18 parts:
    quint16                 procedureCount;
    quint16                 hwSequenceId;

    // A6 parts:

    unsigned char           primary;
    unsigned char           secondary;
    unsigned char           id;
    quint16                 varCount;

    // methods:
    void                    collectA6();
    void                    collectHeader();
    module(QStringList      *code, QStringList *indexBase);
    QByteArray              A6();
    QString                 toHex();
    void                    takePrimary();
    void                    compile();
    void                    prepareVariables();
};

#endif // MODULE_H
