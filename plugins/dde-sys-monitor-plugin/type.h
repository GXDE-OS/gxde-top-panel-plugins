#ifndef TYPE_H
#define TYPE_H
#include <QString>
#include <QVariant>
//主插件类传递给widget的信息结构体
struct Info{
    QString scpu;int cpu;
    QString scputemp;double cputemp;
    QString sbat;//电池功率字符串，如"10.53W"，没有电池时为空
    double bat;//电池功率瓦特值，没有电池时为-1
    QString smem;int mem;
    QString snetup;int netup;
    QString snetdwon;int netdown;
};

//显示内容设置返回的枚举类型
enum DisplayContentSetting
{
    CPUMEM=0,//只显示CPU内存
    NETSPEED=1,//只显示网速
    ALL=2,//两个都显示
    AllAndTempAndBattery=3//显示CPU、内存、网速、电池和CPU温度(与dde-dock版插件的同名模式样式一致)
};

struct SettingItem{
    QString name;
    QVariant value;
};

typedef QMap<QString,QVariant> Settings;

struct Data
{
    int x;
    int y;
};

#endif // TYPE_H
