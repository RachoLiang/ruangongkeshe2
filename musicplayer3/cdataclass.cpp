#include "cdataclass.h"

#include "QtCore/qvariant.h"
CDataClass::CDataClass(QObject *parent) : QObject(parent)
{

}

QVariant CDataClass::testGetData(int count)
{
    int i = 0;
    int total = 0;
    for (int i=0;i<count;i++)
    {
        total += i;
    }
//    return QString("传入的数字是：%1").arg(count);
    return QString("计算返回的数字是：%1").arg(total);
}
