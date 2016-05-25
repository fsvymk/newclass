#include "a.h"
#include <QByteArray>
#include <QList>

#define packLocal 0x11

//HEADERS like A1 A2 A3 A4 A5 A6 and other.

varParameters::varParameters(){
    this->eventCHANGE   =0;
    this->eventSET      =0;
    this->eventZERO     =0;

    this->indexRP       =0;
}

a::a()
{

}

QByteArray a::A1(){QByteArray result; return result;}
QByteArray a::A2(){QByteArray result; return result;}
QByteArray a::A3(){QByteArray result; return result;}
QByteArray a::A4(){QByteArray result; return result;}
QByteArray a::A5(){QByteArray result; return result;}

QByteArray a::A6(quint8 counter, quint8 moduleType, quint8 programID, QList<QByteArray> *A6){
    QByteArray result;
    quint16 localVarCount = A6->size();

    result.append(counter);
    result.append(packLocal);
    result.append(moduleType);
    result.append(programID);
    result.append(localVarCount);

    QList<QByteArray>::iterator LBA;
    for(LBA = A6->begin(); LBA != A6->end(); ++LBA){
        result.append(*LBA);
    }

    quint16 CRC16 = 0xFFFF;
    result.append(CRC16);

    return result;
}

QByteArray a::A7(){QByteArray result; return result;}
QByteArray a::A8(){QByteArray result; return result;}
