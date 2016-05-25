#include "module.h"
#include "vartypes.h"

module::module()
{

}

void module::compile(){
    this->compiled.clear();
    QByteArray *R = &this->compiled;

    R->append("\n\n");
}

void module::prepareVariables(){
    QStringList::iterator it;
    VarTypes VT;
    QRegExp testDefinition(VT.getRegExpQueue() + "[\\s\\t]*([\\w\\d\\_]*)");

    for(it=this->code.begin(); it!=this->code.end(); ++it){
        int test = testDefinition.indexIn(*it);
        if(test >= 0){
            QString type = testDefinition.cap(1);
            QString defs = testDefinition.cap(2);


        }
    }
}
