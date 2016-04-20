#ifndef PARSER_H
#define PARSER_H

#include "qstring.h"
#include "QStringList"
#include <QMap>
#include <QXmlStreamReader>

class Parser{
public:
    void        init();
    int         compile();
    QString     script;
    QByteArray  byteCode;
    QStringList errors;

    void        pe(QString str);    // print error
    QString     allErrors();

private:
    QStringList strings;         // Все строки текущего блока
    QStringList Blocks;          // Все блоки

    bool        parseSem(QXmlStreamReader &xml, QMap<QString, int> &sems);
    QString     displaySems(QMap<QString, int> &sems);
    void        fetchSems(QString FileName, QMap<QString, int> &sems);

    void        parseBlock(QString Block, QMap<QString,int> &sems, int line);
    void        splitBlocks(QString code);


};

#endif // PARSER_H


