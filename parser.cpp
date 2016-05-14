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
#include "sems.h"

#include "mainwindow.h"

// #include "../sid/sid.h"

# include <QtCore/qatomic_x86.h>

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

QString Parser::QStringList_print(QStringList list)
{
    int cnt = 0;
    QString result;
    QStringList::const_iterator it;
    for(it=list.constBegin(); it!=list.constEnd(); ++it)
    {
        result.append("\nlist["+QString::number(cnt)+"]");
        result.append("\n" + *it);
        cnt++;
    }
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

void Parser::parseBlock(QString Block, QMap<QString,int> &sems, int line)// all deprecated?
{


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

void Parser::parseFragment(QString text, int StepN, const QMap<QString, int> &sems, int line)
{
        QByteArray result;

        result += StepN;

        text.replace("}"," ");
        text.replace("{"," ");
        text.replace("\'", "");
        //text.replace("\‘", "");
        //text.replace("\’", "");

        text.replace("Command ", "", Qt::CaseInsensitive);
        //text.replace("Options", "", Qt::CaseInsensitive); // потому что у Options есть код.
        text.replace("Control_by", "", Qt::CaseInsensitive);

        QStringList lines = text.split("\n");

        for (int i = 0; i < lines.length(); i++)
        {
            result += parseLine(lines[i], sems, line);
        }

        int t = result.length();
        for(int i=0; i<(LineSize-1-t); i++){
            result.append((char)0);
        }

        result.append(line);

        globalResult += result;
        //rr(result.toHex());
}

QByteArray Parser::parseLine(const QString &text, const QMap<QString, int> &sems, int line)
{
    //r("PARSE LINE " + text);

    int i = text.indexOf(' ');
    QString type = text.mid(0, i);
    int lastPosition = 0;

    i = text.indexOf('(', lastPosition);
    QString value = text.mid(lastPosition, i - lastPosition).trimmed();
    lastPosition = i;
    i = text.indexOf(')', lastPosition);

    //Текст между скобками
    QString betweenBrackets =  text.mid(lastPosition+1, i - lastPosition - 1);
    //Массив чисел
    QStringList numbers = betweenBrackets.split(",", QString::SkipEmptyParts);

    QByteArray result = processScript(value, numbers, sems, line);

    return result;
}

QByteArray Parser::processScript(QString value, QStringList numbers, const QMap<QString, int> &sems, int line)
{
    //  Самое главное - одному вызову processScript соответствует один вызов parseLine
    //  смысл функции - конкретную строку превратить в байт-код.
    //  line - номер линии
    //  value - команда
    //  numbers - параметры (если есть)
    //  sems - всегда одинаковый набор, можно было бы запихнуть это в глобальную область, если не лень.

    QByteArray result;
    QString SLine = QString::number(line);

    value = value.toLower();
    //Ищем, есть ли нужный семафор из xml файла
    if (!sems.contains(value)) // нет такого ключевого слова
    {
        int VL = value.length();
        //int test_options = value.indexOf("options", Qt::CaseInsensitive);


        if(VL > 0) // если ключевое слово длинее нуля символов
        {
            pe("Err.3 Wrong keyword. " + value + " at line " + SLine); // // + " has not sem. length = " + QString::number(VL));
        }
    }
    else
    {
        result.append(sems[value]);
    }

    //обработка символа *
      for (int i = 0; i < numbers.size(); i++) {
        QString number = numbers[i].trimmed();
            if (number == "*")
                result += (char)0x19;
            else {
                bool ok = false;
                int e = number.indexOf("=");
                QString leftPart  = number.left(e).trimmed();
                QString rightPart = number.right(number.size()-e-1).trimmed();

                QString leftPartSem = "";
                QString rightPartSem= "";

                if (!sems.contains(leftPart)){

                    leftPartSem += "QDEBUG: erro sedm " + leftPart;
                }
                else{
                    result.append(sems[leftPart]);
                    leftPartSem += QString::number(sems[leftPart]);
                }

                if (!sems.contains(rightPart)){
                    // преобразование правой части параметра в число
                    result += (char)rightPart.toUInt(&ok);
                    //rightPartSem += (int)rightPart.toUInt(&ok);
                }
                else{
                    result.append(sems[rightPart]);
                    //rightPartSem += QString::number(sems[rightPart]);
                }
            }
    }

    return result;
}


QByteArray Parser::compileStr(QString str){
    QByteArray result;

    // String samples:
    /*  debug_prnt ("Module IP1 was run at %02d:%02d", m_pos, m_channel);
     *


    0x06                                   04/06/08 - длины блока, тут они везде
0x10	0                   десять - номер строки,   ноль - старший байт (?)
    67                                  67-я строка
    06                                     04/06/08
0x01	0x23                            01 - функция  23 - индекс функции debug_prnt
    ????                               после чего оставим два байта пустыми
    0x04                                    4  - длина блока
0x03	0                                   3  - константа 16bit  0 - старший брат
Индекс компилятора для текста (16-р)        16-битная константа - индекс (указатель) на текст (В Пилотной версии - 0xFFFF)
    0x04                                    4 - длина блока
0x06	0x01                            6 - запятая         1 - счетчик запятых, вот и первая
    0x04                                    4 длина блока
0x02	0xF3                            2 - переменная      F3 = одна из системных, от F0..FF
    0x04                                    4 Длина блока
0x06	0x02                            6 - запятая         2 - вторая запятая
    0x04                                    4 ддлина блока
0x02	0xF5                            2- переменная       F5 системная
    0x04                                    4 длина блока

0x0E   	0                               E = конец тела функции 0 - старший брат

    0x04
0x08	0xF1                            8 - операнд         F1 = конец записи (из таблицы) ПСЕВДООПЕРАЦИЯ. ВЫПОЛНИТЬ.


    // Даже запятая может иметь свой отдельный блок, ну что же.

    // пройтись по всей строке и найти все, что может иметь свой блок.
    // В дальнейшем можно будет строго проверять строку на корректность, а пока просто переведем.

    // что имеем - блоки-атомы, меньше уже не раздробить

    // Прогоняем проверку по таблице "Блоки программирования"

 //=============================================================================================================================//
  //  Блоки описаний программирования Bn

    0x06 (длина блока)
    0x01 (Функция)	Code
    Смещение

    Поле "Смещение" содержит смещение (в блоках) к первому блоку, следующему сразу за блоком "Конец тела функции"



    0x04 (длина блока)
    0x0E (Конец тела функции)	0


    0x08 (длина блока)
    0x0D (Условные переходы)	Управление переходами
    Смещение (ДА)
    Смещение (НЕТ)


    0x06 (длина блока)
    0x0С (Безусловные переход)	Управление переходом
    Смещение


    0x06 (длина блока)
    0x05 (Разграничитель)	Code
    Смещение

    Поле "Смещение" содержит смещение (в блоках) к первому блоку, следующему сразу за зоной разграничения

    0x04 (длина блока)
    0x06 (Запятая)	Счетчик запятых, с 1

    0x06 (длина блока)
    0x10 (Номер строки)	0
    Номер строки

    0x08 (длина блока)
    0x07 (начало процедуры)	0
    Маска вызывающего события

    0x04 (длина блока)
    0x08 (Операнд)	Code

    0x04 (длина блока)
    0x09 (посл.блок программы)	0xFF

    0x06 (длина блока)
    0x03 (16b - константа)	0
    16-разрядная константа

    0x08 (длина блока)
    0x04 (32b - константа)	0
    32-разрядная константа

    0x04 (длина блока)
    0x02 (Переменная)	Code
*/
    QHash<QString, QString> Atoms;

    QRegExp QRE_SPLIT_STRING("\\\"[\\w\\W]+\\\"|[\\w]+|[\\,\\(\\)]");
    //QRegExp QR("\\\"[\\w\\W]+\\\"|[\\w]+|[\\,\\(\\)]");

    QRegExp QRE_FUNCTION("[\w]+");
    QRegExp QRE_COMMA("\,");
    QRegExp QRE_TEXT("\"([\w\s\d]+)\"");

    if(str.length()<1) return ""; // со нулевой строкой нечего делать

    QRegExp QR("\\\"[\\w\\W]+\\\"|[\\w]+|[\\,\\(\\)]", Qt::CaseInsensitive);
    QR.setMinimal(false);
    QStringList SL;
    QHash<int, QStringList> allResult;

    int count = 0;
    int pos = 0;
    int i;
    int x;

    QString code = str;

    while ((pos = QR.indexIn(code, pos)) != -1) {

        ++count;
        pos += QR.matchedLength();

        SL.clear();
        SL.append(QR.capturedTexts());

        i = 0;
        x = SL.count();

        Atoms.insert(QR.cap(0), QR.cap(1));

    }
    return 0;
}

QByteArray  compileBlock(QStringList &block){
    QByteArray result;

    return result;
}


void Parser::classify(QString *code, QHash<QString, QStringList> *result, QString regExp)
// inherit copypasted code from splitBlocks(..)
{
    QStringList allStrings;
    QRegExp classRE(regExp);

    QString str  = *code;
    QString str_copy = str;
    QString block;
    QString excess;         // то что находится до регулярки
    QString blockName;

    QChar qc;

    int     i        = 0;
    int     lineBase = 0;
    int     line     = 0;

    unsigned int BFL = str.count("{");
    unsigned int BFR = str.count("}");
    unsigned int BCL = str.count("(");
    unsigned int BCR = str.count(")");

    if(BFL!=BFR){
        pe("Err. 1: Brakes {} are not pair.");
        return;}

    if(BCL!=BCR){
        pe("Err. 2: Brakes () are not pair.");
        return;}

    while(1==1)
    {
        i = classRE.indexIn(str);if(i<0)return;

        line = whatLine(str_copy, lineBase + i) + 1;
        blockName = classRE.cap(1);
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
        allStrings = block.split("\n");

        result->insert(blockName, allStrings); // Here.



        str = str.right(str.length()-j);
        lineBase += j;
        /*// deprecated
        this->Blocks.append(block);//
        *///
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

void Parser::init(){
    this->PARSER_QREGEXP_MODULE = "module[\\s\\t]*\\([\\s\\t]*([\\w]*)[\\s\\t]*\\,[\\s\\t]*([\\w]*)[\\s\\t]*\\)";
    this->PARSER_QREGEXP_HW_SEQUENCE = "HW_SEQUENCE[\\s\\t]*\\([\\s\\t]*([\\w]*)[\\s\\t]*\\)";
}

void Parser::initSems(){
    this->parserSems.insert("rotl", 0x01);

this->parserSems.insert("rotl",             0x01);
this->parserSems.insert("rotr",             0x02);
this->parserSems.insert("ashl",             0x03);
this->parserSems.insert("ashr",             0x04);
this->parserSems.insert("iif",              0x05);
this->parserSems.insert("read",             0x06);
this->parserSems.insert("write",            0x07);
this->parserSems.insert("write_all", 		0x08);
this->parserSems.insert("write_sel", 		0x09);
this->parserSems.insert("on",               0x0A);
this->parserSems.insert("off",              0x0B);
this->parserSems.insert("local_send_event", 0x0C);
this->parserSems.insert("send_event", 		0x0D);
this->parserSems.insert("send_event_sel", 	0x0E);
this->parserSems.insert("send_event_sync", 	0x0F);
this->parserSems.insert("send_event_sync_sel", 	0x10);
this->parserSems.insert("send_event_all",   0x11);
this->parserSems.insert("end_proc", 		0x12);
this->parserSems.insert("delay",            0x13);
this->parserSems.insert("make_dialog", 		0x14);
this->parserSems.insert("prin",             0x15);
this->parserSems.insert("telemetry", 		0x16);
this->parserSems.insert("telemetry_reset", 	0x17);
this->parserSems.insert("check_hw_connection", 	 0x18);
this->parserSems.insert("break",            0x19);
this->parserSems.insert("continue",         0x20);
this->parserSems.insert("transmitt",        0x21);
this->parserSems.insert("receive",          0x22);
this->parserSems.insert("debug_prnt",       0x23);

}

void Parser::testCase01(){
    QString test =  "debug_prnt (\"Module IP1 was run at %02d:%02d\", m_pos, m_channel);";
    QByteArray res = compileStr(test);


}


int Parser::compile(){
    // WiFi b1212556789

    testCase01();return 0;

    Sems semSoup;

    //return -1;
    QString *script = &this->script;

    this->init();
    //this->splitBlocks(*script); // temporary off

    this->classify(&this->script, &this->sorted, PARSER_QREGEXP_MODULE);
    this->classify(&this->script, &this->sorted, PARSER_QREGEXP_HW_SEQUENCE);

    // теперь код поблочно лежит в контейнере sorted.
    //
    // ключ     - название блока
    // значение - список строк из блока, типа QStringList

    // Подключить все инклуды по списку
    this->addIncludeFile("definitions.h");
    this->addIncludeFile("project1.h");

    // Найти все #define
    int cDr = this->checkDefines(script);

    // Составить таблицу переменных.
    int cVr = this->checkVariables(script);

}
