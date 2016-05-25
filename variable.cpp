#include "variable.h"
#include "a.h"

#include <QApplication>
#include <QDataStream>

variable::variable()
{

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

void variable::getIndex(){
    this->index = this->indexBase.take(this->name);
}
