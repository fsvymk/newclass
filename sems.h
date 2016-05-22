#ifndef SEMS_H
#define SEMS_H

#include <QHash>

class Sems
{
public:
    Sems();
    QHash<QString, int> Sem;
    QHash<QString, int> Opeartion;
    QHash<QString, int> errorCode;
    QHash<QString, int> predefinedVars;
    QHash<QString, int> Type;

    void semsInitDefault();
    void initOps();
    void initErrors();
    void initPreDefs();
    void initTypes();

    QString allOps();
    QString allFunctions();
    QString allTypes();
};

#endif // SEMS_H
