#include "module.h"

module::module()
{

}

void module::compile(){
    this->compiled.clear();
    QByteArray *R = &this->compiled;

    R->append("\n\n");
}
