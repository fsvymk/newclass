#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <QApplication>
#include <sems.h>

class procedure
{
public:
    procedure(QStringList *indexBase, QHash<QString, quint32> *numberDefines);

    QByteArray      compiled;
    QByteArray      byteHeader;
    QByteArray      byteBody;

    unsigned int    firstStroke;

    QString         name; // Run and other;
    QString         arg1; // beetwen brackets;
    QStringList     code;
    QStringList                 indexBase;
    QHash <QString, quint32>    numberDefines;


    QStringList  atoms;

    //methods:

    QByteArray      numberOfStroke();
    void            compile();
    void            compileHeader();
    QByteArray      compileAtom(QString atom, Sems *sems);
    void            splitStr(QString str, QStringList &atoms, Sems *sems);

    // Reg Exp

    QString C;  // = "\\>\\=|\\>|\\<|\\+|\\/|\\-|\\^|\\\\|\\<=|\\||\\=|\\*|\\<\\>|\\:\\=|\\&"; // all Operators.
    QString B;  // = "[\\s\\t]*"; // all spaces and tabs
    QString E;  // = "[\\w]+";     // all functions, keywords, variables types and other that contains alphabet symbols and numbers and _
    QString CLS;// = "\\[|\\]|\\(|\\)|\\,|\\{|\\}";  // brakes and comma;
    QString M;  // = "\\\"[\\w\\W]*\\\"";    // all in quotes
    QString S;  // = "\\;";          // separator

    QString Z;// = C + "|" + E + "|" + CLS + "|" + M + "|" + S; // critical mass of uranium

    QRegExp QR;//(Z);

    QRegExp HEX;//("(0x[abcdef\\d]+)");
};

#endif // PROCEDURE_H
