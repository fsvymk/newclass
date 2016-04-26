#ifndef ANALIZE_H
#define ANALIZE_H

#include <QObject>
#include <QSharedDataPointer>

class AnalizeData;

class Analize : public QObject
{
    Q_OBJECT
public:
    explicit Analize(QObject *parent = 0);
    Analize(const Analize &);
    Analize &operator=(const Analize &);
    ~Analize();

signals:

public slots:

private:
    QSharedDataPointer<AnalizeData> data;
};

#endif // ANALIZE_H