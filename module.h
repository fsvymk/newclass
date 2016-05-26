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
    module(QStringList *code, QStringList *indexBase);
    QStringList             indexBase;

    // methods:

    void compile();
    void prepareVariables(QStringList indexes);
};

#endif // MODULE_H
