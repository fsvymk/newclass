#ifndef MODULE_H
#define MODULE_H

#include <QApplication>

class module
{
public:
    QStringList code;
    QByteArray  compiled;
    module();

    // methods:

    void compile();
};

#endif // MODULE_H
