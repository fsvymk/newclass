#include "vartypes.h"
#include <QString>
#include <QStringList>

VarTypes::VarTypes()
{
        this->types.append("int");
        this->types.append("dint");
        this->types.append("word");
        this->types.append("dword");
        // Deprecated.

        // Look at that:
        this->types << "string" << "bool";

}


QString VarTypes::getRegExpQueue()
{

}
