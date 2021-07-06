
#include "HashMap.h"
unsigned hashFunction(QString name)
{
    unsigned result = 0;
    string str=name.toStdString();
    for (unsigned i = 0; i < str.length(); i++)
        result += (i+1)*(int)(str[i]);

    return result;
}
