#ifndef MODULE_H
#define MODULE_H

#include <QApplication>
#include <variable.h>

class module
{
public:
    QStringList             code;
    QByteArray              compiled;
    QList<variable>         variables;
    QStringList             indexBase;
    QString                 compiledHex;
    QString                 name;

    // A6 parts:

    unsigned char           primary;
    unsigned char           secondary;
    unsigned char           id;
    quint16                 varCount;

    // methods:
    module(QStringList      *code, QStringList *indexBase);
    QByteArray              A6();
    QString                 toHex();
    void                    takePrimary();
    void                    compile();
    void                    prepareVariables();
};

#endif // MODULE_H
