#ifndef PYTHON_ENTY_H
#define PYTHON_ENTY_H

#include <QString>
class python_enty
{
public:

    int type;
    QString name;
    QString pypath;
    bool install();
    void uninstall();
    bool  regeistToDb();
    void deletefromdb();
    python_enty();
};

#endif // PYTHON_ENTY_H
