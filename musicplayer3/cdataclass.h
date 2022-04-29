#ifndef CDATACLASS_H
#define CDATACLASS_H


#include "QtCore/qobject.h"
#include "QtCore/qtmetamacros.h"
class CDataClass:public QObject
{
      Q_OBJECT
public:
//    CDataClass();
    explicit CDataClass(QObject *parent = nullptr);
       Q_INVOKABLE QVariant testGetData(int count);

};

#endif // CDATACLASS_H
