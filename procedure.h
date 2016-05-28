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

    QStringList     code;

    //methods:

    void            compile();
    void            compileHeader();
};

#endif // PROCEDURE_H
