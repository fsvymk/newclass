#include "parser.h"
#include <qxmlstream.h>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>
#include <QApplication>
#include <QTextStream>
#include <QDate>
#include <QTime>

int Parser::checkDefines(QString *str){

    QString script = *str;
    QString StepArgs;

    QRegExp QR("#[D|d]efine[\\s]+([\\w]+)[\\s]+([\\w|\\d]+)[\\n|\\;]");
    QR.setMinimal(true);

    while(1==1)
    {
        int i = QR.indexIn(script);
        if(i<0) return -1;
        StepArgs = QR.cap(0);
        this->constants.append(QR.cap(1));

        /*
        this->values.append(QR.cap(2));
        this->errors.append(StepArgs);
        */ //deprecated

        this->defines.insert(QR.cap(1),QR.cap(2));

        script = script.right(script.length() - StepArgs.length());
    }

    return 0;
}

int Parser::checkVariables(QString *str){
    int varCount = 0;
    return varCount;

}

void Parser::addIncludeFile(QString filename){
    if(!this->includeFiles.contains(filename,Qt::CaseSensitive))
    this->includeFiles.append(filename);
}

int Parser::compile(){
    // WiFi b1212556789


    //return -1;

    QString *script = &this->script;

    // Подключить все инклуды по списку

    this->addIncludeFile("definitions.h");
    this->addIncludeFile("project1.h");

    // Найти все #define

    int cDr = this->checkDefines(script);

    // Составить таблицу переменных.

    int cVr = this->checkVariables(script);
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

void Parser::parseBlock(QString Block, QMap<QString,int> &sems, int line)
{
    //b("BLOCK. Line = " + QString::number(line));
    //b(Block);
    //b("\r\n\r\n\r\n\r\n\r\n");

    QString str = Block;
    QString str_copy = str; // тот же принцип что и в предыдущей функции

    QRegExp QR("[S|s]tep\\d+\\s");
    QR.setMinimal(true);
    QChar qc;
    QRegExp QRStepN("\\d+");
    QString StepArgs;

    QString StepN;
    QString step;
    QStringList ThreeParts;

    int lineBase = 0;  // позиция текущего шага из блока (в символах)
    int lineInner = 0; // линия внутри блока. line - из аргументов функции - линия, с которой начинается Block.

    while(1==1)
    {
        int i = QR.indexIn(str);
        if(i<0) return;


        lineInner = whatLine(str_copy, lineBase + i); // линия внутри блока! первая в блоке - нулевая здесь.
        if(lineBase == 0) lineBase = i; // только в первом проходе, чтобы Block name{ попали сюда...

        StepArgs = QR.cap(0);
        str = str.right(str.length() - StepArgs.length());

        // получение номера Step

        QRStepN.indexIn(StepArgs);
        StepN = QRStepN.cap(0);
        globalStepNumber = StepN.toInt();
        //globalResult += globalStepNumber;

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

        // Получение одного шага из оставшихся нескольких
        step = str.mid(i,j-i);
        str = str.right(str.length() - j);
        lineBase += j + 1;

        // Разбиваем шаг на составляющие
        //ThreeParts = Command_ControlBy_Options(step, line + lineInner); // DEPRECATED

        /* // DEPRECATED 20.04.2016
        parseFragment(ThreeParts[0], globalStepNumber, sems, line + lineInner + __CommandLine);
        parseFragment(ThreeParts[1], globalStepNumber, sems, line + lineInner + __ControlByLine);
        parseFragment(ThreeParts[2], globalStepNumber, sems, line + lineInner + __OptionsLine);

        */
        //ThreeParts = TrueThreeParts(step, line+lineInner);
    }

}

void Parser::pe(QString str){

};

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
            saveLogs("0","NULL");
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

    //QString log1 = ui->parser_codeEditor->toPlainText();
    //QString log2 = ui->PARSER_TEXT_RESULT->toPlainText();

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
