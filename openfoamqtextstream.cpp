#include "openfoamqtextstream.h"
#include <QDebug>

OpenFOAMQTextStream::OpenFOAMQTextStream(QFile* qf) : QTextStream(qf)
{
}


void OpenFOAMQTextStream::ignore(QString target, int max)
{
    int c = 0;
    QString tmp;
    OpenFOAMQTextStream & in = *this;
    while(!in.atEnd()) {
        (*this) >> tmp;
        if(tmp.compare(target) == 0) {
            c++;
        }
        if (c == max) break;
    }
}
