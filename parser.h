#ifndef PARSER_H
#define PARSER_H

#include "qstring.h"
#include "QStringList"
#include <QMap>
#include <QHash>
#include <QXmlStreamReader>

class Parser{
public:
    void        init();
    void        initSems();
    int         compile();
    QString     script;
    QByteArray  byteCode;
    QStringList errors;

    QStringList constants;  // deprecated
    QStringList values;     // deprecated

    QHash <QString, QString> defines;

    void        pe(QString str);    // print error

    void        addIncludeFile(QString filename);

    QString     allErrors();

    QString PARSER_QREGEXP_MODULE;
    QString PARSER_QREGEXP_HW_SEQUENCE;
    QString PARSER_QREGEXP_MSG;
    QString PARSER_QREGEXP_BIT_ARRAY;

private:
    QStringList includeFiles;   // Имена файлов
    QStringList includes;       // Содержимое инклудов, независимо от происхождения

    QStringList strings;         // Все строки текущего блока
    QStringList Blocks;          // Все блоки

    QHash<QString, QStringList>  sorted; // Имя блока - и исходный код построчно
    QHash<QString, QByteArray> compiled; // Аналогично - имя блока - байткод.

    QHash<QString, QStringList> hwSequence;

    QMap<QString, int> parserSems;        // Семафоры.

    int         checkDefines(QString *str);
    int         checkVariables(QString *str);

    bool        parseSem(QXmlStreamReader &xml, QMap<QString, int> &sems);
    QString     displaySems(QMap<QString, int> &sems);
    void        fetchSems(QString FileName, QMap<QString, int> &sems);

    void        parseBlock(QString Block, QMap<QString,int> &sems, int line);
    void        splitBlocks(QString code);
    void        classify(QString *code, QHash<QString, QStringList> *result, QString regExp);
    void        globalSems();

    // some functions for parser
    int         whatLine(QString text, int position);
    void        temporary_green();
    void        saveLogs(QString code, QString result);

    // Parser
    static const int LineSize = 32;
    QStringList BlockNames;
    QByteArray globalResult;

    /*int __CommandLine;
    int __ControlByLine;
    int __OptionsLine;
    int globalStepNumber;*/

    //

};

#endif // PARSER_H


