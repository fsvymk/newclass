#include "variable.h"
#include "a.h"

#include <QApplication>
#include <QDataStream>

variable::variable()
{
    this->atomA6.resize(16);
}

QByteArray variable::A6(){
    QByteArray      result;
    QDataStream     Raskalov(&result, QIODevice::WriteOnly);
    Raskalov << this->index << this->assign << this->VP.indexRP
                              << this->VP.eventSET
                              << this->VP.eventZERO
                              << this->VP.eventCHANGE;

    return result;
}

void variable::prepareA6(){
    this->atomA6.clear();
    this->atomA6.append(this->index);
    this->atomA6.append(this->assign);
    this->atomA6.append(this->VP.indexRP);
    this->atomA6.append(this->VP.eventSET);
    this->atomA6.append(this->VP.eventZERO);
    this->atomA6.append(this->VP.eventCHANGE);
}

void variable::prepareA6_stream(){
    QDataStream atom(&this->atomA6, QIODevice::WriteOnly);
    atom << this->index;
    atom << this->assign;
    /*
    this->atomA6.clear();
    this->atomA6.append(this->index);
    this->atomA6.append(this->assign);
    this->atomA6.append(this->VP.indexRP);
    this->atomA6.append(this->VP.eventSET);
    this->atomA6.append(this->VP.eventZERO);
    this->atomA6.append(this->VP.eventCHANGE);
    */
}

void variable::getIndex(){
    // deprecated
}
