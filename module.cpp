#include "module.h"
#include "vartypes.h"
#include "QDataStream"

module::module(QStringList *code, QStringList *indexBase, QHash<QString, quint32> *numberDefines)
{
    this->code                  = *code;
    this->indexBase             = *indexBase;
    this->numberDefines         = *numberDefines;
}

void module::collectA6(){
    // Developed on A6() prototype
    quint16 CRC16 = 0xF0F0;

    quint8 counter = 0xEE;
    quint8 typeId  = 0x11;

    this->blockA6.append(counter);
    this->blockA6.append(typeId);
    this->blockA6.append(this->primary);
    this->blockA6.append(this->id);
    this->blockA6.append(this->varCount%256);
    this->blockA6.append(this->varCount/256);

    QList<variable>::iterator it;
    for(it = this->variables.begin(); it != this->variables.end(); ++it){
        this->blockA6.append(it->A6());
    }

    this->blockA6.append(CRC16%256);
    this->blockA6.append(CRC16/256);
}

QByteArray module::A6(){
    QByteArray result;

    QDataStream An(&result, QIODevice::ReadWrite);

    quint16 CRC16 = 0xF0F0;
    quint8 counter = 0xEE;
    quint8 typeId  = 0x11;
        QByteArray TV;
    An << counter << typeId << this->primary << this->id;
    An << this->varCount;

    QList<variable>::iterator it;
    for(it = this->variables.begin(); it != this->variables.end(); ++it){
        it->prepareA6_stream();
        //An << it->atomA6;           // It must works. Lol, it appends quint32 QByteArray size..
        //An << it->atomA6.
    }

    An << CRC16;
    return result;
}


QString module::toHex(){

    this->compiledHex.clear();
    int n = this->compiled.size();
    int i = 0;
    for(i=0; i<n; i++){
        unsigned char c = this->compiled.at(i);
        QString hex = QString::number(c, 16);

        if(i%4 == 0) this->compiledHex.append("  ");
        if(i%8 == 0) this->compiledHex.append("   ");
        if(i%16 == 0) this->compiledHex.append("\n");
        if(c<16) this->compiledHex.append("0");
        this->compiledHex.append(hex + " ");
    }

    return this->compiledHex;
}

void module::prepareVariables(){
    QStringList::iterator it;
    VarTypes VT;
    QRegExp testDefinition(VT.getRegExpQueue() + "[\\s\\t]*([^\\n]*)\\;");
    QRegExp testRgPort("([\\w\\d\\_]*)[\\s\\t]*\\:[\\s\\t]*(port|rg)[\\s\\t]*\\:[\\s\\t]*([\\w\\d\\_]*)"); // иЗя : порт : номер
    QRegExp testVarName("\\w+");

    for(it=this->code.begin(); it!=this->code.end(); ++it){
        int test = testDefinition.indexIn(*it);
        if(test >= 0){
            QString type = testDefinition.cap(1);
            QString defs = testDefinition.cap(2);

            QStringList listCommaSplitted = defs.split("\,");
            variable v;

            QStringList::iterator it;
            for(it=listCommaSplitted.begin(); it!=listCommaSplitted.end(); ++it){
                int indexRgPort     =  testRgPort.indexIn(*it);
                int indexVarName    =  testVarName.indexIn(*it);
                int countRgPort     =  testRgPort.captureCount();
                int countVarName    =  testVarName.captureCount();

                if(indexRgPort >= 0){
                    v.name                  = testRgPort.cap(1);

                    QString assignment      = testRgPort.cap(2);
                    QString address         = testRgPort.cap(3);
                    if(assignment=="port"){
                        bool ok;
                        v.assign            = 0x80;
                        v.VP.indexRP        = address.toInt(&ok, 10);
                        v.VP.eventCHANGE    = 0x00010000;
                    }else if(assignment=="rg"){
                        bool ok;
                        v.assign = 0x40;
                        v.VP.indexRP        = address.toInt(&ok, 10);

                    }else{
                        // error. undefined assignment.
                    }
                }else{
                    v.assign    = 0x04;
                    v.name      = testVarName.cap(0);
                }
                v.index = this->indexBase.indexOf(v.name);
                this->variables.append(v);
            }
        }
    }
    this->varCount = this->variables.size();
}


// will not used;
void module::takePrimary(){
    QRegExp QRPrimary("module[\\s\\t]*\\([\\s\\t]*([\\w]*)[\\s\\t]*\\,[\\s\\t]*([\\w]*)[\\s\\t]*\\)");
    QRPrimary.indexIn(this->code[0]);
    QString primary = QRPrimary.cap(1);
}

void module::collectHeader(){
    QString first = this->code[0];
    QRegExp QRPrimary("module[\\s\\t]*\\([\\s\\t]*([\\w]*)[\\s\\t]*\\,[\\s\\t]*([\\w]*)[\\s\\t]*\\)");
    int i = QRPrimary.indexIn(first);

    QString arg1 = QRPrimary.cap(1);
    QString arg2 = QRPrimary.cap(2);

    quint8 counter = 0xEE;
    quint8 typeId  = 0x10;

    //this->blockHeader.append();
    /*
    Счетчик	0x10
    KPA_IP1	ID?
    0x0002
    0x0004
    */

    this->blockHeader.append(counter);
    this->blockHeader.append(typeId);
    this->blockHeader.append(this->primary);
    this->blockHeader.append(this->id);

    this->blockHeader.append(this->procedureCount);
    this->blockHeader.append(this->hwSequenceId);

}

int module::whatLine(QString text, int position)
{
    text = text.left(position);
    return text.count("\n"); // т.к. перед первой строкой нет перевода строки, но это уже одна строка.
}


void module::classify(QStringList *code, QHash<QString, QStringList> *result, QString regExp)
// copyasted from Parser::classify(,,)
{
    QStringList allStrings;
    QRegExp classRE(regExp);

    QString str;
    QStringList::iterator it;
    for(it=code->begin(); it != code->end(); ++it){
        str.append(*it+"\n");
    }

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
        //pe("Err. 1: Brakes {} are not pair.");
        return;}

    if(BCL!=BCR){
        //pe("Err. 2: Brakes () are not pair.");
        return;}

    while(1==1)
    {
        i = classRE.indexIn(str);if(i<0)return;

        line = whatLine(str_copy, lineBase + i) + 1;
        blockName = classRE.cap(2);
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
        allStrings.append("}"); // crutch.. or important small part.

        result->insert(blockName, allStrings); // Here.
        // We done it..

        str = str.right(str.length()-j);
        lineBase += j;

   }
}

void module::prepareProcedures(){
    QString MODULE_QREGEXP_PROCEDURES = "procedure[\\s\\t]*([\\w]*)[\\s\\t]*\\([\\s\\t]*([\\w]*)[\\s\\t]*\\)";
    QRegExp QRM(MODULE_QREGEXP_PROCEDURES);

    this->classify(&this->code, &this->proceduresCode, MODULE_QREGEXP_PROCEDURES);

    QHash<QString, QStringList>::iterator it;
    QStringList *P;
    QString headerString;

    procedure PROC(&this->indexBase, &this->numberDefines);

    for(it = this->proceduresCode.begin(); it != this->proceduresCode.end(); ++it){
        P = &it.value();
        headerString = it.value().at(0);
        QRM.indexIn(headerString);
        PROC.arg1 = QRM.cap(2); // (..) // equal it.key()
        PROC.name = QRM.cap(1); // Run..

        PROC.code = it.value();
        this->procedures.append(PROC);
    }
}

void module::compile(){

    prepareVariables();
    prepareProcedures();

    collectHeader();
    this->collectA6();
    this->compiled.append(this->blockA6);

     QList<procedure>::iterator PROC;
     for(PROC = this->procedures.begin(); PROC != this->procedures.end(); ++PROC){
        PROC->compile();
        this->compiled.append(PROC->byteBody);
     }

//    int n = this->procedureCount;
//    int i = 0;
//    for(i=0; i<n; i++){

//        this->compiled.append('\0');

//        QByteArray XX = this->procedures.takeAt(i).byteBody;
//        this->compiled.append(this->procedures.at(i).byteBody);
//    }

    this->toHex();
}
