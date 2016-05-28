#include "module.h"
#include "vartypes.h"
#include "QDataStream"

module::module(QStringList *code, QStringList *indexBase)
{
    this->code                  = *code;
    this->indexBase             = *indexBase;
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
                        v.assign            = 80;
                        v.VP.indexRP        = address.toInt(&ok, 10);
                        v.VP.eventCHANGE    = 0x00010000;
                    }else if(assignment=="rg"){
                        bool ok;
                        v.assign = 40;
                        v.VP.indexRP        = address.toInt(&ok, 10);
                    }else{
                        // error. undefined assignment.
                    }
                }else{
                    v.assign    = 4;
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

void module::compile(){
    //this->compiled.clear();
    QDataStream R(this->compiled);
    prepareVariables();
    //QByteArray RRR = this->A6();
    //this->compiled.append(RRR);

    this->collectA6();
    this->compiled.append(this->blockA6);

    //this->compiledHex.append(this->code.at(0));
    this->toHex();
    //this->compiledHex.append(this->toHex());
    //R << this->A6();
}
