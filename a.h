#ifndef A_H
#define A_H

#include <QByteArray>

class varParameters{
public:
    quint16 indexRP;
    quint16 eventSET;
    quint16 eventZERO;
    quint16 eventCHANGE;
    varParameters();
};

class a
{
public:
    QByteArray A1();
    QByteArray A2();
    QByteArray A3();
    QByteArray A4();
    QByteArray A5();
    QByteArray A6(quint8 counter, quint8 moduleType, quint8 programID, QList<QByteArray> *A6);
    QByteArray A7();
    QByteArray A8();
    a();
};

#endif // A_H
