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
    module(QStringList *code, QMap<QString, quint8>       *variablesIndexBase);

    QMap<QString, quint8>   variablesIndexBase;
    // methods:

    void compile();
    void prepareVariables();
};

#endif // MODULE_H
