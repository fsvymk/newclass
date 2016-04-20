#include "parser.h"
#include <qxmlstream.h>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>
#include <QApplication>

int Parser::compile(){

    return -1;
}

bool Parser::parseSem(QXmlStreamReader &xml, QMap<QString, int> &sems)
{
     //p("MainWindow::parseSem");
     if (xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "sem")
         return false;
     QXmlStreamAttributes attributes = xml.attributes();
     int id = 0;
     if (attributes.hasAttribute("id")) {
         id = attributes.value("id").toInt();
     }
     else
         return false;

     xml.readNext();
     while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "sem"))
     {
         if (xml.tokenType() == QXmlStreamReader::StartElement)
         {
             if (xml.name() == "name"){
                 xml.readNext();
                 QString name = xml.text().toString();
                 if (sems.contains(name))
                     return false;
                 sems.insert(name, id);
             }
         }
         xml.readNext();
     }
     return true;
}


QString Parser::displaySems(QMap<QString, int> &sems)
{
    QMap<QString, int>::const_iterator it = sems.begin();
    for(;it != sems.end(); ++it)
    {
       return "KEY:" + it.key() + "=" + QString::number(it.value());
    }
}

void Parser::fetchSems(QString FileName, QMap<QString, int> &sems)
{
    QFile file(FileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                    /*
                        QMessageBox::critical(this,"Load XML File Problem",
                        "Couldn't open config.xml to load settings for parse",
                        QMessageBox::Ok);
                        return;
                    */
            }

    QXmlStreamReader xml(&file);

    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement)
        {
            if (xml.name() == "sems")
                continue;
            if (xml.name() == "sem"){
                if (!parseSem(xml, sems)) {
                   /* QMessageBox::critical(this,"XML File Problem",
                    "Error parse XML file",
                    QMessageBox::Ok); */
                    return;
                }
            }
        }
    }
}

void Parser::splitBlocks(QString code)
{
    //temporary_green();
    //ui->PARSER_TEXT_RESULT->clear();

    QMap<QString, int> globalSems;
    fetchSems(":/config.xml", globalSems);
    displaySems(globalSems);

    //QString str = ui->parser_codeEditor->toPlainText();
    QString str  = code;    // из аргумента
    QString str_copy = str; // т.к. str будет урезаться в процессе разделения на блоки.

    // Проверка парности скобок.
    int BFL = str.count("{");
    int BFR = str.count("}");
    int BCL = str.count("(");
    int BCR = str.count(")");

    if(BFL!=BFR){
        pe("Err. 1: Brakes {} are not pair.");
        return;
    }

    if(BCL!=BCR){
        pe("Err. 2: Brakes () are not pair.");
        return;
    }

    QByteArray BlockResult;
    QRegExp Block("Block(\\s+)(\\w+)",Qt::CaseInsensitive);
    int i = 0;

    QString blockNameAll;
    QStringList blockNameParts;
    QString blockName;

    QChar qc;
    int lineBase = 0;
    int line = 0;
    QString block;

    while(1==1)
    {
        BlockResult.clear();
        i = Block.indexIn(str);

        line = whatLine(str_copy, lineBase + i) + 1;

        if(i<0)
        {
            saveLogs();
            //rb("GlobalResult = ");
            // вывод готового пакета данных.
            //rb(globalResult.toHex());
            return;
        }

        blockNameAll = Block.cap(0);
        blockNameParts = blockNameAll.split(" ");
        blockName = blockNameParts[1];
        //rb("block Name = " + blockName);

        BlockResult += (char)0; // Block sem code
        BlockResult += blockName;

        while (BlockResult.length() < 32){
            BlockResult += (char)0;
        }

        //rb("BL = " + BlockResult.toHex());
        globalResult += BlockResult;


        int j = str.indexOf('{',i);
        qc = str[j];

        int bl=1;
        int br=0;

        while(bl != br)
        {
            j++;
            qc = str[j];
            if(qc=='{') bl++;
            if(qc=='}') br++;
        }
        block = str.mid(i,j-i);
        str = str.right(str.length()-j);
        lineBase += j;
        // и отправим содержимое в следующую функцию

        parseBlock(block, globalSems, line);
   }
    // сюда код лучше не вставлять т.к. выход из цикла - сразу return;
}
