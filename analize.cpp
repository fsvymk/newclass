#include "analize.h"

class AnalizeData : public QSharedData
{
public:

};

Analize::Analize(QObject *parent) : QObject(parent), data(new AnalizeData)
{

}

Analize::Analize(const Analize &rhs) : data(rhs.data)
{

}

Analize &Analize::operator=(const Analize &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

Analize::~Analize()
{

}
