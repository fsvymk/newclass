#include "vartypes.h"
#include <QString>
#include <QStringList>

VarTypes::VarTypes()
{
        this->types.append("dint");
        this->types.append("uint");
        this->types.append("int");
        this->types.append("dword");
        this->types.append("uword");
        this->types.append("word");

        // Deprecated.

        // Look at that:
        this->types << "string" << "bool";

}


QString VarTypes::getRegExpQueue()
{
    QString result = "(";

    int cnt = 0;
    QStringList::const_iterator it;
    for(it=this->types.constBegin(); it!=this->types.constEnd(); ++it)
    {
        result += *it;
        result += "|";
        cnt++;
    }

    result += "var)";

    return result;
}
