#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <QApplication>
#include <sems.h>

class procedure
{
public:
    procedure(QStringList *indexBase, QHash<QString, quint32> *numberDefines);

    QByteArray      compiled;
    QByteArray      byteHeader;
    QByteArray      byteBody;

    unsigned int    firstStroke;

    QString         name; // Run and other;
    QString         arg1; // beetwen brackets;
    QStringList     code;
    QStringList                 indexBase;
    QHash <QString, quint32>    numberDefines;


    QStringList  atoms;

    //methods:

    QByteArray      numberOfStroke();
    void            compile();
    void            compileHeader();
    QByteArray      compileAtom(QString atom, Sems *sems);
    void            splitStr(QString str, QStringList &atoms, Sems *sems);
};

#endif // PROCEDURE_H
