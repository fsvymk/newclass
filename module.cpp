#include "module.h"
#include "vartypes.h"
#include "QDataStream"

module::module(QStringList *code, QStringList *indexBase)
{
    this->code                  = *code;
    this->indexBase             = *indexBase;
}

QByteArray module::A6(){
    QByteArray result;
    QByteArray header;
    QDataStream An(result);

    quint16 CRC16 = 0xF0F0;

    unsigned char counter = 0xEE;
    unsigned char typeId  = 0x11;

    header.append(counter);
    header.append(typeId);
    header.append(this->primary);
    header.append(this->id);

    An << header;

    QList<variable>::iterator it;
    for(it = this->variables.begin(); it != this->variables.end(); ++it){
        An << *it->A6();
    }

    An << CRC16;
    return result;
}

void module::prepareVariables(){
    QStringList::iterator it;
    VarTypes VT;
    QRegExp testDefinition(VT.getRegExpQueue() + "[\\s\\t]*([^\\n]*)\\;");
    QRegExp testRgPort("([\\w\\d\\_]*)[\\s\\t]*\\:[\\s\\t]*(port|rg)[\\s\\t]*\\:[\\s\\t]*([\\w\\d\\_]*)"); // имя : порт : номер
    QRegExp testVarName("\\w+");

    for(it=this->code.begin(); it!=this->code.end(); ++it){
        int test = testDefinition.indexIn(*it);
        if(test >= 0){
            QString type = testDefinition.cap(1);
            QString defs = testDefinition.cap(2);

            QStringList listCommaSplitted = defs.split("\\,");
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
                        v.assign = 40;
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
}

void module::takePrimary(){
    QRegExp QRPrimary("module[\\s\\t]*\\([\\s\\t]*([\\w]*)[\\s\\t]*\\,[\\s\\t]*([\\w]*)[\\s\\t]*\\)");
    QRPrimary.indexIn(this->code[0]);
}

void module::compile(){
    this->compiled.clear();
    QByteArray *R = &this->compiled;
    takePrimary();
    prepareVariables();


    R->append("\n\n");
}
