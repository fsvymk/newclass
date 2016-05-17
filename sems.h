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

    void semsInitDefault();
    void initOps();
    void initErrors();
    void initPreDefs();

    QString allOps();
    QString allFunctions();
};

#endif // SEMS_H
