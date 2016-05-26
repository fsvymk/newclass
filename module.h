#ifndef MODULE_H
#define MODULE_H

#include <QApplication>
#include <variable.h>

class module
{
public:
    QStringList             code;
    QByteArray              A6();
    QByteArray              compiled;
    QList<variable>         variables;
    module(QStringList      *code, QStringList *indexBase);
    QStringList             indexBase;

    QString                 compiledHex;

    // A6 parts:

    unsigned char           primary;
    unsigned char           secondary;
    unsigned char           id;

    // methods:
    QString                 toHex();
    void                    takePrimary();
    void                    compile();
    void                    prepareVariables();
};

#endif // MODULE_H
