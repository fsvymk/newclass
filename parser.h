#ifndef PARSER_H
#define PARSER_H

#include "qstring.h"
#include "QStringList"
#include <QMap>
#include <QHash>
#include <QXmlStreamReader>


#include "module.h"
#include "a.h" //varParametes - ok

class Parser{
public:
    //garbage from graphs:
    void       parseFragment(QString text, int StepN, const QMap<QString, int> &sems, int line);
    QByteArray parseLine(const QString &text, const QMap<QString, int> &sems, int line);
    QByteArray processScript(QString value, QStringList numbers, const QMap<QString, int> &sems, int line);

    //Perfect parser:
    void        init();
    void        initSems();
    int         compile();
    unsigned char getDefine(QString name);
    QString     script;
    QByteArray  byteCode;
    QStringList errors;

    QStringList constants;  // deprecated
    QStringList values;     // deprecated

    QHash <QString, QString> defines;
    QHash <QString, quint32> numberDefines;

    void        definesToInt();
    void        pe(QString str);    // print error
    void        testCase02();
    void        addIncludeFile(QString filename);

    QString     allErrors();

    QString     PARSER_QREGEXP_MODULE;
    QString     PARSER_QREGEXP_HW_SEQUENCE;
    QString     PARSER_QREGEXP_MSG;
    QString     PARSER_QREGEXP_BIT_ARRAY;

    QByteArray  compileAtom(QString atom);
    void        splitStr(QString str, QList<QString> &atoms);
    QByteArray  compileBlock(QStringList block);
QString    hwSequenceCompiledHex;
QString    modulesHex;
    // TEST:

    void testCase01();

    // OOP is good;
    QList<module> Modules;      // все модули из программы
    QMap<QString, quint8> varIndexes;
    QMap<quint8, quint8> varTypes;    // 04 - default, 40 - register, 80 - port

    QHash<QString, quint16> registerHash;
    QHash<QString, quint16> portHash;
private:
    QStringList includeFiles;   // Имена файлов
    QStringList includes;       // Содержимое инклудов, независимо от происхождения

    QStringList strings;         // Все строки текущего блока
    QStringList Blocks;          // Все блоки

    QHash<QString, QStringList>  sorted; // Имя блока - и исходный код построчно
    QHash<QString, QByteArray> compiled; // Аналогично - имя блока - байткод.

    QHash<QString, QStringList> hwSequence;
    QByteArray hwSequenceCompiled;

    QStringList variables;  // variables, position is index.
    QStringList indexBase;  // It's the same, but without shit

    QMap<QString, QString> varMap; // variable and expression at definition time.



    QMap<QString, int> parserSems;        // Семафоры.

    int         checkDefines(QString *str);
    int         checkVariables(QString *str);
    int         loadIncludes();


    void        takeModules();
    QByteArray  packVariable(quint8 index, quint8 type, varParameters *VP);
    QByteArray  compileVariables(QStringList *str);
    void  compileModule();
    QString     QStringList_print(QStringList list);
    bool        parseSem(QXmlStreamReader &xml, QMap<QString, int> &sems);
    QString     displaySems(QMap<QString, int> &sems);
    void        fetchSems(QString FileName, QMap<QString, int> &sems);

    QByteArray  compileBlock(QStringList &block);
    void        parseBlock(QString Block, QMap<QString,int> &sems, int line);
    void        splitBlocks(QString code);
    void        classify(QString *code, QHash<QString, QStringList> *result, QString regExp);
    void        globalSems();

    // some functions for parser
    int         whatLine(QString text, int position);
    void        temporary_green();
    void        saveLogs(QString code, QString result);

    void        HWStoHex();
    void        compileHWS();
    void        selectHWS();

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


