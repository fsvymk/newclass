#ifndef PARSER_H
#define PARSER_H

#include "qstring.h"
#include "QStringList"

class Parser{
public:
    void        init();
    int         compile();
    QString     script;
    QByteArray  byteCode;
private:
    QStringList strings;         // Все строки текущего блока
    QStringList Blocks;          // Все блоки
    void        splitBlocks();
};

#endif // PARSER_H


