#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QObject>
#include <QBoxLayout>
#include <QRgb>
#include <QDebug>
#include <QScreen>
#include <QApplication>
#include "pluginsiteminterface.h"
#include "streamchart.h"
#include "type.h"

extern struct SettingItem settingItems[];

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(Settings&,Dock::Position);
    ~MainWidget();
    void UpdateData(const Info& info,Dock::Position position,const Settings& settings);
    QSize sizeHint() const;

signals:
    //文字宽度变化时发出，插件收到后调用 itemUpdate 让面板重新计算条目尺寸
    void requestUpdateGeometry();

public:
    int dpi;
    QBoxLayout *centralLayout;
    // 文字模式数据显示在这2个Label上
    QLabel *cpuMemLabel,*netLabel;
    //显示数据的图表类
    StreamChart *netChart,*cpuChart,*memChart;
    struct Data data;
    // 字体
    QFont font;
    //保存之前的设置
    Settings oldsettings;
    //保存之前的位置
    Dock::Position oldposition;
    //保存上一次的sizeHint，用于检测内容宽度变化
    QSize m_lastSizeHint;


private:
    Ui::MainWidget *ui;
    void initLabels(void);
    void initChart(void);
};

#endif // MAINWIDGET_H
