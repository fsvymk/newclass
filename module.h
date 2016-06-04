#ifndef MODULE_H
#define MODULE_H

#include <QApplication>
#include <variable.h>
#include <procedure.h>
#include <QRegExp>

class module
{
public:
    QString                 name;
    QStringList             code;
    QHash<QString, QStringList>     proceduresCode;
    // byte Blocks:
    QByteArray              blockHeader;
    QByteArray              blockA6;
    QByteArray              block18;
    QByteArray              compiled;

    QString                 compiledHex;

    //Will transfered from Parser
    QList<variable>             variables;
    QList<procedure>            procedures;
    QStringList                 indexBase;
    QHash <QString, quint32>    numberDefines; // You can set any defines for each module (if it is very necessary of course)

    // block18 parts:
    quint16                 procedureCount;
    quint16                 hwSequenceId;

    // A6 parts:

    unsigned char           primary;
    unsigned char           secondary;
    unsigned char           id;
    quint16                 varCount;

    // methods:
    void                    collectA6();
    void                    collectHeader();
    module(QStringList      *code, QStringList *indexBase, QHash<QString, quint32> *numberDefines);
    QByteArray              A6();
    QString                 toHex();
    void                    takePrimary();
    void                    compile();

    void                    prepareProcedures();
    void                    prepareVariables();

    // Candidates to be transfered to a base abstract class
    int                     whatLine(QString text, int position);
    void                    classify(QStringList *code, QHash<QString, QStringList> *result, QString regExp);

    // REG EXP
     QRegExp testDefinition;
     QRegExp testRgPort;
     QRegExp testVarName;

     QRegExp QRPrimary;//("module[\\s\\t]*\\([\\s\\t]*([\\w]*)[\\s\\t]*\\,[\\s\\t]*([\\w]*)[\\s\\t]*\\)");
};

#endif // MODULE_H
