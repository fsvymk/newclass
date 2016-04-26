#ifndef VARTYPES_H
#define VARTYPES_H

#include <QStringList>

class VarTypes
{
public:
    VarTypes();
    QStringList types;
    QString getRegExpQueue();
};

#endif // VARTYPES_H
