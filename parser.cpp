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
#include "atom.h"

#include "mainwindow.h"
#include "a.h"

#include <QtCore/qatomic_x86.h>

int Parser::checkDefines(QString *str){
    QString script = *str;
    QString StepArgs;
    QRegExp QR("#[D|d]efine[\\s]+([\\w]+)[\\s]+([\\w|\\d]+)");
    QR.setMinimal(false);

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

QByteArray Parser::packVariable(quint8 index, quint8 type, varParameters *VP){
    // It is devoted to Raskalov the roofer.
    QByteArray result;
    QDataStream Raskalov(&result, QIODevice::WriteOnly);
    Raskalov << index << type << &VP->indexRP
                              << &VP->eventSET
                              << &VP->eventZERO
                              << &VP->eventCHANGE;
    return result;
    /*
     * It is able to compile        - ok
     * Data stream                  - ok
     * var Parameters as argument   - ok
     * return result                - ok
     */
}

QByteArray Parser::compileVariables(QStringList *str){
    QByteArray        result;
    a                 headers;
    QList<QByteArray> listA6;  // последовательность подструктур типа A6
    QStringList       code = *str;

    // копирование из checkVariables

    int varCount      = 0;
    int index         = 0;

    VarTypes VT;

    QRegExp rx(VT.getRegExpQueue()+"[\\s+](\\w+)");            // deprecated

    QRegExp erx(VT.getRegExpQueue()+"[\\s\\t]*([^\\n]*)\\;"); // тип - все остальное (кроме перевода строки), а в конце ;
    QRegExp erx_rgPort("([\\w\\d\\_]*)[\\s\\t]*\\:[\\s\\t]*(port|rg)[\\s\\t]*\\:[\\s\\t]*([\\w\\d\\_]*)"); // имя : порт : номер
    QRegExp erxVarName("\\w+");                              // имя

    QStringList types;
    QStringList list;
    QStringList vars;

    int pos = 0;

    // search type declaration
    QStringList::iterator sit;
    for(sit = code.begin(); sit != code.end(); ++sit){
        if ((pos = erx.indexIn(*sit, pos)) != -1) {
             types << erx.cap(1);
             list << erx.cap(2);
             pos += erx.matchedLength();

             vars << erx.cap(2).split(","); // ищем объявления разделенные запятой внутри одной строки.

             // this->variables << vars; // не сохраняем в экземпляр парсера, вместо этого выдаем через return
        }
    }
    //this->variables.removeDuplicates(); // --

    // Let's sort variables;
    QStringList::iterator it;
    QString name, type, value;
    quint8  assignment;                // variable Register Port // 0x04 0x40 0x80

    varParameters VP;

    for(it=vars.begin(); it!=vars.end(); ++it){
        QString all = *it;
        int indexRgPort  =  erx_rgPort.indexIn(*it);

        int indexVarName = erxVarName.indexIn(*it);
        int countRgPort  = erx_rgPort.captureCount();
        int countVarName = erxVarName.captureCount();

        if(indexRgPort>=0){name  = erx_rgPort.cap(1);
        }else{name = erxVarName.cap(0);}

        index        = this->varIndexes.take(name);
        assignment   = this->varTypes.take(index);
        // taking variable index and type - ok.

        listA6.append(packVariable(index, assignment, &VP));
    }

    quint16 programID = 0xFF;
    result.append(headers.A6(0xFF, 0x11, programID, &listA6));

    return result;
}

int Parser::checkVariables(QString *str){
    int varCount    = 0;
    int index   = 0;

    VarTypes VT;

    QRegExp rx(VT.getRegExpQueue()+"[\\s+](\\w+)");
    QRegExp erx(VT.getRegExpQueue()+"[\\s\\t]*([^\\n]*)\\;");
    QRegExp erx_rgPort("([\\w\\d\\_]*)[\\s\\t]*\\:[\\s\\t]*(port|rg)[\\s\\t]*\\:[\\s\\t]*([\\w\\d\\_]*)");
    QRegExp erxVarName("\\w+");

    QStringList types;
    QStringList list;
    QStringList vars;

    int pos = 0;

    while ((pos = erx.indexIn(*str, pos)) != -1) {
         types << erx.cap(1);
         list << erx.cap(2);
         pos += erx.matchedLength();

         vars = erx.cap(2).split(",");

         this->variables << vars;
    }
    this->variables.removeDuplicates();

    // Let's sort variables;
    QStringList::iterator it;
    QString name, type, value;

    for(it=this->variables.begin(); it!=this->variables.end(); ++it){
        QString all = *it;
        int ZZ =  erx_rgPort.indexIn(*it);
        int YY = erxVarName.indexIn(*it);

        int Z = erx_rgPort.captureCount();
        int Y = erxVarName.captureCount();
        int iType = 4;

        if(ZZ>=0)
        {
            name  = erx_rgPort.cap(1);
            type  = erx_rgPort.cap(2); // nothing, register, port
            value = erx_rgPort.cap(3);

            this->varMap.insert(name, value + " (" + type + ")");

            if(type=="rg")      iType = 40;
            if(type=="port")    iType = 80;

            index++;
            if(index>254) { /*error: index overflow */ }
            this->varIndexes.insert(name, index);
            this->varTypes.insert(index, iType);

        }else
            {
                index++;
                name = erxVarName.cap(0);
                this->varMap.insert(name, "");

                this->varIndexes.insert(name, index);
                this->varTypes.insert(index, iType);
            }

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

QByteArray Parser::compileAtom(QString atom){
    QByteArray result;

    //QRegExp QR("\\\"[\\w\\W]+\\\"|[\\w]+|[\\,\\(\\)]");

    QRegExp QRE_FUNCTION("[\w]+");
    QRegExp QRE_COMMA("\,");
    QRegExp QRE_TEXT("\"([\w\s\d]+)\"");

    // To do:
    /*
     *
     * Check atom, what's this?
     *
     * 1. Function
     * 2. End of function
     * 3. If ..
     * 4. GoTo
     * 5. Separator
     * 6. Comma ','
     * 7. String number
     * 8. Procedure begin
     * 9. Operand
     * 10. End of program 0xFF
     * 11. Const16
     * 12. Const32
     * 13. Variant
     *
     * */

    // 1. Регулярное выражение - список функций, если нет строгого соответствия, значит не функция
    /*
     * 2. Конец функции - ...
     *
     * 3. Регулярка с условным переходом и выражение в аргументе.
     *      Выражение в аргументе....
     *
     * 4. Строгое соответствие шаблону "Безусловный переход в начало цикла"; continue;
     *
     * 5. Это типа любое из ; } {  если я правильно понял
     *
     * 6. Запятая
     *
     * 7. Номер строки - Получим другим путем
     * */

    Sems SEMS;
    QString functions  = SEMS.allFunctions();
    QString operations = SEMS.allOps();

    QString QRSBrakes = "[\\s\\t]*\\([\\w\\W]*\\)";

    QRegExp QR_function(functions);         // 1
      // 2 - pass, eoF will received in other way

    QRegExp QR_if("if[\\s\\t]*\\([\\w\\W]*\\)");  // 3
    QRegExp QR_continue("continue");        // 4
    QRegExp QR_separaror("\\;|\\{|\\}");       // 5
    QRegExp QR_comma("\\,");                 // 6
     // 7 - other way

    QRegExp QR_procedure("procedure"+QRSBrakes); // 8
    QRegExp QR_operand(operations);              // 9

    QRegExp QR_end("");  // 10 end of Program - other way

    QRegExp QR_const16(); // регулярка - все константы
    QRegExp QR_const32();

    QRegExp QR_Variant(); // список всех переменных

    QRegExp QR_expression(); // выражение
    //QRegExp QR_var(SEMS.allTypes+"[\\s\\t]*([w]+)"); // объявление переменной (до знака равенства)

    // Все переменные можно заранее найти и занести
    // в контейнер а из него в регулярку отдельным методом
    /*
     * Пройтись с шаблоном тип переменной - переменная [= выражение]
     *
     * что для этого нужно?
     * шаблон выражений
     * шаблон объявления переменной "тип переменная"

    */

    return result;
}

void Parser::splitStr(QString str, QList<QString> &atoms){
    QByteArray result;
    if(str.length()<1) return;

    QRegExp QR("\\\"[\\w\\W]+\\\"|[\\w]+|[\\,\\(\\)]", Qt::CaseInsensitive);
    QR.setMinimal(false);

    //int count = 0;
    int pos = 0;

    while ((pos = QR.indexIn(str, pos)) != -1) {
        //++count;
        pos += QR.matchedLength();
        atoms.append(QR.cap(0));
        compileAtom(QR.cap(0));
    }
}

QByteArray Parser::compileBlock(QStringList &block){
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
    QList<QString> testResult;
    splitStr(test, testResult);
}

void Parser::HWStoHex(){

    this->hwSequenceCompiledHex.clear();
    int n = this->hwSequenceCompiled.size();
    int i = 0;
    for(i=0; i<n; i++){
        unsigned char c = this->hwSequenceCompiled.at(i);
        QString hex = QString::number(c, 16);

        if(i%4 == 0) this->hwSequenceCompiledHex.append("  ");
        if(i%8 == 0) this->hwSequenceCompiledHex.append("   ");
        if(i%16 == 0) this->hwSequenceCompiledHex.append("\n");
        if(c<16) this->hwSequenceCompiledHex.append("0");
        this->hwSequenceCompiledHex.append(hex + " ");

    }
}

void Parser::compileHWS(){
    QHash<QString, QStringList>::iterator it;
    QStringList content;
    QString     hwSequenceID;

    //byte-code
    char blockCounter = 0xFF;
    char HWS_OPCODE  = 0x06;


    quint16 CRC16       = 0; // не обнуляется в цикле.

    QStringList hwsModules;

    for(it=this->hwSequence.begin();  it!=this->hwSequence.end();  ++it){

        this->hwSequenceCompiled.append(blockCounter);
        this->hwSequenceCompiled.append(HWS_OPCODE);

        content         = it.value();
        hwSequenceID    = it.key();
        quint16 counter     = 0;
                 // ...
        QString strHWSID    = this->defines.value(hwSequenceID);  // то что в скобках
        bool    HWSID_detected;
        quint16 HWSID       = strHWSID.toInt(&HWSID_detected, 16);


        if(!HWSID_detected){
            // error
        }
        // How to convert int16 to 2 chars? Ok, divide it by 256..

        this->hwSequenceCompiled.append(HWSID%256);
        this->hwSequenceCompiled.append(HWSID/256);

        int addrTotal = this->hwSequenceCompiled.size();
        int addrLocal = this->hwSequenceCompiled.size() + 2;

        this->hwSequenceCompiled.append("xxxx");  // CRC16, counter;

        int i;
        int n = content.size();

        QRegExp QRAnchor("\\@([\\w]+)");        //\\@([\\w]+)
        QRegExp QRDefinition("([\\w\\d\\_]+)[\\s\\t]*\\:[\\s\\t]*([\\w\\d]+)");

        QString line;

        for(i=0;i<n;i++){
            line = content.at(i);
            this->strings.append(line); // DEL!
            int a = QRAnchor.indexIn(line);
            int d = QRDefinition.indexIn(line);
            QStringList anchors      = QRAnchor.capturedTexts();
            QStringList definitions  = QRDefinition.capturedTexts();

            if(a>0){
                //CRC16++;
                bool anchor_detected;

                counter++;
                this->strings.append("anchor");
                QString anchor = anchors.at(1);
                quint16 anchorID = this->defines.value(anchor).toInt(&anchor_detected, 16);
                this->hwSequenceCompiled.append(255);
                this->hwSequenceCompiled.append(255);
                this->hwSequenceCompiled.append(anchorID%256);
                this->hwSequenceCompiled.append(anchorID/256);
            }
            if(d>0){
                CRC16++;
                counter++;
                this->strings.append("definition");
                QString strModuleID         = definitions.at(1);
                QString strModulePosition   = definitions.at(2);
                bool module_detected;
                bool position_detected;

                quint8 moduleHWID       = 0;
                quint8 moduleID         = this->defines.value(strModuleID).toInt(&module_detected, 16);
                quint16 modulePosition  = strModulePosition.toInt(&module_detected, 10);

                if(module_detected){
                    // check, is this module in list. If not, append it, else take it number;
                    if (!hwsModules.contains(strModuleID, Qt::CaseInsensitive)){
                        hwsModules.append(strModuleID);
                        if(hwsModules.size()>254){
                            // error. Overflow!
                        }
                    }
                    moduleHWID = hwsModules.indexOf(strModuleID);
                }
                this->hwSequenceCompiled.append(moduleHWID);
                this->hwSequenceCompiled.append(moduleID);
                this->hwSequenceCompiled.append(modulePosition%256);
                this->hwSequenceCompiled.append(modulePosition/256);
            }
        }
        this->hwSequenceCompiled[addrLocal + 0] = counter%256;
        this->hwSequenceCompiled[addrLocal + 1] = counter/256;
        this->hwSequenceCompiled[addrTotal + 0] = counter%256; //crc?
        this->hwSequenceCompiled[addrTotal + 1] = counter/256; //crc?
    }
    this->hwSequenceCompiled[4] = CRC16%256;
    this->hwSequenceCompiled[5] = CRC16/256;

    HWStoHex();
}

void Parser::selectHWS(){

    int i = 0;
    int n = this->sorted.size();

    QHash<QString, QStringList>::iterator it;
    QString firstLine;

    for(it=this->sorted.begin();  it!=this->sorted.end();  ++it){
        firstLine = it.value().at(0);
        this->strings.append(firstLine);
    }

    for(i=0; i<n; i++){
        //QString firstLine = this->sorted.;
    }
}

int Parser::loadIncludes(){
    QStringList::iterator it;

    for(it=this->includeFiles.begin();it!=this->includeFiles.end();++it){
        QFile hFile(*it);
        if (hFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&hFile);
           while (!in.atEnd())
           {
              QString line = in.readLine();
              this->includes.append(line);
           }
           hFile.close();
        }
    }

    // Append includes before first line of main script;


    QString included;
    for(it=this->includes.begin();it!=this->includes.end();++it){
        included.append(*it);
    }
    this->script.prepend(included); // nice.
}

QByteArray Parser::compileModule(QString key){
    QStringList M = this->sorted.take(key);
    QByteArray result;

    QByteArray VARS = compileVariables(&M);
    result.append(VARS);

    result.append("0");
    return result;
}

int Parser::compile(){
    // WiFi b1212556789

   //testCase01();return 0;

    Sems semSoup;

    //return -1;
    QString *script = &this->script;

    this->init();
    //this->splitBlocks(*script); // temporary off

    this->classify(&this->script, &this->sorted, PARSER_QREGEXP_MODULE);

    this->classify(&this->script, &this->hwSequence, PARSER_QREGEXP_HW_SEQUENCE);

    // теперь код поблочно лежит в контейнере sorted.
    //
    // ключ     - название блока
    // значение - список строк из блока, типа QStringList

    // Подключить все инклуды по списку
    this->addIncludeFile("definitions.h");
    this->addIncludeFile("project1.h");
    this->addIncludeFile("kpa_config.h");
    this->addIncludeFile("names.h");
    int lIf = this->loadIncludes();
    // Найти все #define
    int cDr = this->checkDefines(script);

    // Составить таблицу переменных.
    int cVr = this->checkVariables(script);


    compileHWS();

    QByteArray firstGrape = compileModule("KPA_IP3");
}
