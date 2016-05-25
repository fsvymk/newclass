#include "module.h"
#include "vartypes.h"

module::module(QStringList *code, QMap<QString, quint8> *variablesIndexBase)
{
    this->code                  = *code;
    this->variablesIndexBase    = *variablesIndexBase;
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
                        v.assign = 40;
                    }else{
                        // error. undefined assignment.
                    }
                }else{v.name = testVarName.cap(0);}

                v.indexBase = &this->variablesIndexBase;

                this->variables.append(v);
                //index        = this->varIndexes.take(name);
                //assignment   = this->varTypes.take(index);
            }

        }
    }
}

void module::compile(){
    this->compiled.clear();
    QByteArray *R = &this->compiled;
    prepareVariables();
    R->append("\n\n");
}
