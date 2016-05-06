#include "parser.h"
#include <qxmlstream.h>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>
#include <QApplication>
#include <QTextStream>
#include <QDate>
#include <QTime>

#include "vartypes.h"

int Parser::checkDefines(QString *str){
    QString script = *str;
    QString StepArgs;
    QRegExp QR("#[D|d]efine[\\s]+([\\w]+)[\\s]+([\\w|\\d]+)");
    QR.setMinimal(true);

    while(1==1)
    {
        int i = QR.indexIn(script);
        if(i<0) return -1;
        StepArgs = QR.cap(0);
        this->constants.append(QR.cap(1));
        this->defines.insert(QR.cap(1),QR.cap(2));
        script = script.right(script.length() - StepArgs.length());
    }
    return 0;
}

int Parser::checkVariables(QString *str){
    int varCount = 0;

    VarTypes VT;

    QRegExp rx(VT.getRegExpQueue()+"[\\s+](\\w+)");
    QStringList list;
    int pos = 0;

    while ((pos = rx.indexIn(*str, pos)) != -1) {
         list << rx.cap(1);
         pos += rx.matchedLength();
    }
    return varCount;
}

void Parser::addIncludeFile(QString filename){
    if(!this->includeFiles.contains(filename,Qt::CaseSensitive))
    this->includeFiles.append(filename);
}


bool Parser::parseSem(QXmlStreamReader &xml, QMap<QString, int> &sems)
{

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

                    return;
                }
            }
        }
    }
}

void Parser::parseBlock(QString Block, QMap<QString,int> &sems, int line)
{
    // all deprecated?

    QString str = Block;
    QString str_copy = str;

    QRegExp QR("[S|s]tep\\d+\\s");
    QR.setMinimal(true);
    QChar qc;
    QRegExp QRStepN("\\d+");
    QString StepArgs;

    QString StepN;
    QString step;
    QStringList ThreeParts;

    int lineBase = 0;
    int lineInner = 0;

    while(1==1)
    {
        int i = QR.indexIn(str);
        if(i<0) return;


        lineInner = whatLine(str_copy, lineBase + i);
        if(lineBase == 0) lineBase = i;

        StepArgs = QR.cap(0);
        str = str.right(str.length() - StepArgs.length());


        QRStepN.indexIn(StepArgs);
        StepN = QRStepN.cap(0);
        //globalStepNumber = StepN.toInt();

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

        step = str.mid(i,j-i);
        str = str.right(str.length() - j);
        lineBase += j + 1;
    }

}

void Parser::pe(QString str){

};

void Parser::splitBlocks(QString code)
{
    QString str  = code;
    QString str_copy = str;

    unsigned int BFL = str.count("{");
    unsigned int BFR = str.count("}");
    unsigned int BCL = str.count("(");
    unsigned int BCR = str.count(")");

    if(BFL!=BFR){
        pe("Err. 1: Brakes {} are not pair.");
        return;
    }

    if(BCL!=BCR){
        pe("Err. 2: Brakes () are not pair.");
        return;
    }

    QByteArray BlockResult;
    QRegExp Block("module[\\s\\t]*\\([\\s\\t]*([\\w]*)[\\s\\t]*\\,[\\s\\t]*([\\w]*)[\\s\\t]*\\)");
    int i = 0;
    QString blockName;
    QChar qc;

    int     lineBase = 0;
    int     line     = 0;
    QString block;

    while(1==1)
    {
        i = Block.indexIn(str);if(i<0)return;

        line = whatLine(str_copy, lineBase + i) + 1;
        blockName = Block.cap(1);
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

    this->Blocks.append(block);
   }
}

void Parser::globalSems(){
    QMap<QString, int> globalSems;
    fetchSems(":/config.xml", globalSems);
    displaySems(globalSems);
}

void Parser::classify(QString *code, QStringList *result)
// inherit copypasted code from splitBlocks(..)
//
{
    QString str  = *code;
    QString str_copy = str;

    unsigned int BFL = str.count("{");
    unsigned int BFR = str.count("}");
    unsigned int BCL = str.count("(");
    unsigned int BCR = str.count(")");

    if(BFL!=BFR){
        pe("Err. 1: Brakes {} are not pair.");
        return;
    }

    if(BCL!=BCR){
        pe("Err. 2: Brakes () are not pair.");
        return;
    }

    QByteArray BlockResult;
    QRegExp Block("module[\\s\\t]*\\([\\s\\t]*([\\w]*)[\\s\\t]*\\,[\\s\\t]*([\\w]*)[\\s\\t]*\\)");
    int i = 0;
    QString blockName;
    QChar qc;

    int     lineBase = 0;
    int     line     = 0;
    QString block;

    while(1==1)
    {
        i = Block.indexIn(str);if(i<0)return;

        line = whatLine(str_copy, lineBase + i) + 1;
        blockName = Block.cap(1);
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

    this->Blocks.append(block);
   }
}

int Parser::whatLine(QString text, int position)
{
    text = text.left(position);
    return text.count("\n"); // т.к. перед первой строкой нет перевода строки, но это уже одна строка.
}

void Parser::temporary_green()
{

}

void Parser::saveLogs(QString code, QString result)
{
    QString fileName1 = "log_result.txt";
    QString fileName2 = "log_errors.txt";

    QString log1 = code;
    QString log2 = result;

    log1.replace("\n","\r\n");
    log2.replace("\n","\r\n");

    QFile file1(fileName1);
    QFile file2(fileName2);

    QFile::OpenMode FileMode1 = QIODevice::WriteOnly;
    QFile::OpenMode FileMode2 = QIODevice::WriteOnly;

    if(file1.exists(fileName1)) FileMode1 = QIODevice::Append;
    if(file2.exists(fileName2)) FileMode2 = QIODevice::Append;

    file1.open(FileMode1);
    file2.open(FileMode2);

    QTextStream stream1(&file1);
    QTextStream stream2(&file2);

    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QString date_str = date.toString() + " " + time.toString();

    stream1 << "\r\n\r\n##### " + date_str + " #####\r\n";
    stream2 << "\r\n\r\n===== " + date_str + " =====\r\n";

    stream1 << log1;
    stream2 << log2;

    file1.close();
    file2.close();
}


int Parser::compile(){
    // WiFi b1212556789

    //return -1;
    QString *script = &this->script;

    this->splitBlocks(*script);

    // Подключить все инклуды по списку
    this->addIncludeFile("definitions.h");
    this->addIncludeFile("project1.h");

    // Найти все #define
    int cDr = this->checkDefines(script);

    // Составить таблицу переменных.
    int cVr = this->checkVariables(script);
}
