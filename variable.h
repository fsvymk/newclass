#ifndef VARIABLE_H
#define VARIABLE_H

#include <QApplication>
#include "a.h"

class variable
{

public:
    // Structure:
    quint8          index;
    quint8          assign; // variable 4, register 40, port 80
    quint16         portNumber;
    varParameters   VP;

    //
    QString         name;
    QByteArray      atomA6;

    // methods:
    variable();
    void        prepareA6();
    void        prepareA6_stream();
    void        getIndex();
    QByteArray  A6();
};

#endif // VARIABLE_H
