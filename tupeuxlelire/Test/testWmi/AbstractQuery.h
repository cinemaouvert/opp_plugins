#ifndef ABSTRACTQUERY_H
#define ABSTRACTQUERY_H

#include <QObject>
#include <string>

using std::string;

class AbstractQuery
{
public:
    AbstractQuery();
    virtual void setReponse(string reponse) = 0;
};

#endif // ABSTRACTQUERY_H
