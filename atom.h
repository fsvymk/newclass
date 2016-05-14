#ifndef ATOM_H
#define ATOM_H

#include <QObject>

class Atom : public QObject
{
    Q_OBJECT
public:
    explicit Atom(QObject *parent = 0);
    QList<QString> atoms;

    QByteArray compile(QString code);
    QByteArray compile(int key);

signals:

public slots:
};

#endif // ATOM_H
