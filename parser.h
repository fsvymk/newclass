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
    void        addIncludeFile(QString filename);

    QString     allErrors();

private:
    QStringList includeFiles;   // Имена файлов
    QStringList includes;       // Содержимое инклудов, независимо от происхождения

    QStringList strings;         // Все строки текущего блока
    QStringList Blocks;          // Все блоки

    bool        parseSem(QXmlStreamReader &xml, QMap<QString, int> &sems);
    QString     displaySems(QMap<QString, int> &sems);
    void        fetchSems(QString FileName, QMap<QString, int> &sems);

    void        parseBlock(QString Block, QMap<QString,int> &sems, int line);
    void        splitBlocks(QString code);

    // some functions for parser
    int         whatLine(QString text, int position);
    void        temporary_green();
    void        saveLogs(QString code, QString result);

    // Parser
    static const int LineSize = 32;
    QStringList BlockNames;
    QByteArray globalResult;
    int __CommandLine;
    int __ControlByLine;
    int __OptionsLine;
    int globalStepNumber;

    //

};

#endif // PARSER_H


