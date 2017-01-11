#ifndef OPENFOAMQTEXTSTREAM_H
#define OPENFOAMQTEXTSTREAM_H

#include <QTextStream>
#include <QFile>

class OpenFOAMQTextStream : public QTextStream
{
public:
    OpenFOAMQTextStream(QFile* qf);
    void ignore(QString, int);
};

#endif // OPENFOAMQTEXTSTREAM_H
