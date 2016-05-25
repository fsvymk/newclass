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
    varParameters   VP;

    // methods:
    variable();

    QByteArray A6();
};

#endif // VARIABLE_H
