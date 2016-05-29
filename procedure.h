#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <QApplication>

class procedure
{
public:
    procedure();

    QByteArray      compiled;
    QByteArray      byteHeader;
    QByteArray      byteBody;

    unsigned int    firstStroke;

    QString         name; // Run and other;
    QString         arg1; // beetwen brackets;
    QStringList     code;


    QList<QString>  atoms;

    //methods:

    void            compile();
    void            compileHeader();
    QByteArray      compileAtom(QString atom);
    void            splitStr(QString str, QList<QString> &atoms);
};

#endif // PROCEDURE_H
