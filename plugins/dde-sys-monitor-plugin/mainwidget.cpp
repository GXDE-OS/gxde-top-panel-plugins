#include "mainwidget.h"
#include <DFontSizeManager>
#include <DGuiApplicationHelper>

DGUI_USE_NAMESPACE

MainWidget::MainWidget(Settings& settings,Dock::Position position)
{
    centralLayout=NULL;
    cpuMemLabel=NULL;
    netLabel=NULL;
    netChart=NULL;
    cpuChart=NULL;
    memChart=NULL;
    //setMinimumSize(5,5);
    //设置等宽字体
    font.setFamily("Noto Mono");
    // 获取dpi，一般默认都是96，根据dpi进行字体的缩放，直接设置pointsize无法解决hidpi问题
    dpi = QApplication::primaryScreen()->logicalDotsPerInch();
    oldsettings=settings;
    oldposition=position;
}

MainWidget::~MainWidget()
{
    if(cpuMemLabel!=NULL){delete cpuMemLabel;cpuMemLabel=NULL;}
    if(netLabel!=NULL){delete netLabel;netLabel=NULL;}
    if(netChart!=NULL){delete netChart;netChart=NULL;}
    if(cpuChart!=NULL){delete cpuChart;cpuChart=NULL;}
    if(memChart!=NULL){delete memChart;memChart=NULL;}
    if(centralLayout!=NULL){delete centralLayout;centralLayout=NULL;}
}

void MainWidget::UpdateData(const Info &info, Dock::Position position, const Settings &settings)
{
    //qDebug()<<"MainWidget::UpdateData() start";
    //如果dock位置发生如下变化，则手动重构ui
    if(
            ((oldposition==Dock::Top||oldposition==Dock::Bottom)&&(position==Dock::Left||position==Dock::Right))
            ||
            ((position==Dock::Top||position==Dock::Bottom)&&(oldposition==Dock::Left||oldposition==Dock::Right))
      )
    {
        if(cpuMemLabel!=NULL){delete cpuMemLabel;cpuMemLabel=NULL;}
        if(netLabel!=NULL){delete netLabel;netLabel=NULL;}
        if(netChart!=NULL){delete netChart;netChart=NULL;}
        if(cpuChart!=NULL){delete cpuChart;cpuChart=NULL;}
        if(memChart!=NULL){delete memChart;memChart=NULL;}
        if(centralLayout!=NULL){delete centralLayout;centralLayout=NULL;}
    }
    oldposition=position;

    if(!settings.value("chartModeCheckBox").toInt())//文字模式
    {
        //当模式切换的时候
        if(oldsettings.value("chartModeCheckBox").toInt())
        {
            //先清理掉之前的ui
            //qDebug()<<"模式切换，先清理掉之前的图表ui";
            if(netChart!=NULL){delete netChart;netChart=NULL;}
            if(cpuChart!=NULL){delete cpuChart;cpuChart=NULL;}
            if(memChart!=NULL){delete memChart;memChart=NULL;}
            if(centralLayout!=NULL){delete centralLayout;centralLayout=NULL;}
        }
        //当模式切换或者第一次初始化的时候，需要新建ui
        if(centralLayout==NULL)
        {
            //创建新的ui
            qDebug()<<"创建新的文字ui";
            cpuMemLabel=new QLabel();
            cpuMemLabel->setAlignment(Qt::AlignCenter);
            netLabel=new QLabel();
            netLabel->setAlignment(Qt::AlignCenter);
            centralLayout = new QBoxLayout((position==Dock::Top||position==Dock::Bottom)?
                                               QBoxLayout::LeftToRight:QBoxLayout::TopToBottom);
            centralLayout->addWidget(cpuMemLabel);
            centralLayout->addWidget(netLabel);
            centralLayout->setContentsMargins(0, 0, 0, 0);
            setLayout(centralLayout);
        }
        centralLayout->setSpacing(settings.value("wordSpacingSpinBox").toInt());
        font.setPixelSize((dpi*settings.value("fontSizeSpinBox").toInt())/72);
        cpuMemLabel->setFont(font);
        netLabel->setFont(font);

        QString style;
        if(DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType)
            style=QString("QLabel {color: %1;}").arg("#000");
        else
            style=QString("QLabel {color: %1;}").arg("#fff");

        cpuMemLabel->setStyleSheet(style);
        netLabel->setStyleSheet(style);

        switch (settings.value("displayContentComboBox").toInt())
        {
        case DisplayContentSetting::AllAndTempAndBattery:
            //与dde-dock版插件的同名模式样式一致：单标签两行，含Tem/Bat字段。
            //两行文字宽度不同（Tem:xx°C 比 Bat:xx.xW 长），必须左对齐才能上下两列对齐，
            //居中对齐会让短的一行缩进
            cpuMemLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
            cpuMemLabel->setVisible(true);
            netLabel->setVisible(false);
            cpuMemLabel->setText(QString("CPU:%1↑%2/S Tem:%3°C\nMEM:%4↓%5/S Bat:%6W")
                                 .arg(info.scpu)
                                 .arg(info.snetup)
                                 .arg(QString::number(info.cputemp, 'f', 1))
                                 .arg(info.smem)
                                 .arg(info.snetdwon)
                                 .arg(QString::number(qMax(info.bat, 0.0), 'f', 1)));
            break;
        case DisplayContentSetting::CPUMEM:
            cpuMemLabel->setVisible(true);
            netLabel->setVisible(false);
            cpuMemLabel->setText(QString("%1%2\n%3%4")
                                 .arg(settings.value("cpuDiyWordLineEdit").value<QString>())
                                 .arg(info.scpu)
                                 .arg(settings.value("memDiyWordLineEdit").value<QString>())
                                 .arg(info.smem));
            break;
        case DisplayContentSetting::NETSPEED:
            cpuMemLabel->setVisible(false);
            netLabel->setVisible(true);
            netLabel->setText(QString("%1%2/s\n%3%4/s")
                              .arg(settings.value("upNetspeedDiyWordLineEdit").value<QString>())
                              .arg(info.snetup)
                              .arg(settings.value("downNetspeedDiyWordLineEdit").value<QString>())
                              .arg(info.snetdwon));
            break;
        default://DisplayContentSetting::ALL:
            cpuMemLabel->setVisible(true);
            netLabel->setVisible(true);
            cpuMemLabel->setText(QString("%1%2\n%3%4")
                                 .arg(settings.value("cpuDiyWordLineEdit").value<QString>())
                                 .arg(info.scpu)
                                 .arg(settings.value("memDiyWordLineEdit").value<QString>())
                                 .arg(info.smem));
            netLabel->setText(QString("%1%2/s\n%3%4/s")
                              .arg(settings.value("upNetspeedDiyWordLineEdit").value<QString>())
                              .arg(info.snetup)
                              .arg(settings.value("downNetspeedDiyWordLineEdit").value<QString>())
                              .arg(info.snetdwon));
            break;
        }
    }
    else//图表模式
    {
        //当模式切换的时候，清除文字ui
        if(!oldsettings.value("chartModeCheckBox").toInt())
        {
            delete cpuMemLabel;cpuMemLabel=NULL;
            delete netLabel;netLabel=NULL;
            delete centralLayout;centralLayout=NULL;
        }
        //当图表有变化的时候，清除所有图表ui
        if(oldsettings.value("cpuChartCheckBox").toInt()+settings.value("cpuChartCheckBox").toInt()==1||
           oldsettings.value("memChartCheckBox").toInt()+settings.value("memChartCheckBox").toInt()==1||
           oldsettings.value("netChartCheckBox").toInt()+settings.value("netChartCheckBox").toInt()==1)
        {
            if(cpuChart!=NULL){delete cpuChart;cpuChart=NULL;}
            if(memChart!=NULL){delete memChart;memChart=NULL;}
            if(netChart!=NULL){delete netChart;netChart=NULL;}
            delete centralLayout;centralLayout=NULL;
        }
        //当模式切换或者第一次初始化的时候，绘制图表ui
        if(centralLayout==NULL)
        {
            centralLayout = new QBoxLayout((position==Dock::Top||position==Dock::Bottom)?
                                               QBoxLayout::LeftToRight:QBoxLayout::TopToBottom);
            centralLayout->setContentsMargins(0, 0, 0, 0);
            setLayout(centralLayout);
        }

        centralLayout->setSpacing(settings.value("chartSpacingSpinBox").toInt());
        if(settings.value("cpuChartCheckBox").toInt())
        {
            if(cpuChart==NULL)
            {
                cpuChart=new StreamChart();
                centralLayout->addWidget(cpuChart);
            }
            data.x=info.cpu;
            data.y=0;
            cpuChart->height=settings.value("cpuHeightSpinBox").toInt();
            cpuChart->width=settings.value("cpuWidthSpinBox").toInt();
            cpuChart->color1=settings.value("cpuWorkWidget").value<QColor>();
            cpuChart->color2=QColor(255,255,255,0);
            cpuChart->borderRound=settings.value("cpuBorderRoundSpinBox").toInt();
            cpuChart->colorBorder=settings.value("cpuBorderWidget").value<QColor>();
            cpuChart->colorBackground=settings.value("cpuBackgroundWidget").value<QColor>();
            cpuChart->spacing=settings.value("chartSpacingSpinBox").toInt();

            cpuChart->updateChart(data);
        }
        if(settings.value("memChartCheckBox").toInt())
        {
            if(memChart==NULL)
            {
                memChart=new StreamChart();
                centralLayout->addWidget(memChart);
            }
            data.x=info.mem;
            data.y=0;
            memChart->height=settings.value("memHeightSpinBox").toInt();
            memChart->width=settings.value("memWidthSpinBox").toInt();
            memChart->color1=settings.value("memUsedWidget").value<QColor>();
            memChart->color2=QColor(255,255,255,0);
            memChart->borderRound=settings.value("memBorderRoundSpinBox").toInt();
            memChart->colorBorder=settings.value("memBorderWidget").value<QColor>();
            memChart->colorBackground=settings.value("memBackgroundWidget").value<QColor>();
            memChart->spacing=settings.value("chartSpacingSpinBox").toInt();

            memChart->updateChart(data);
        }
        if(settings.value("netChartCheckBox").toInt())
        {
            if(netChart==NULL)
            {
                netChart=new StreamChart();
                centralLayout->addWidget(netChart);
            }
            data.x=info.netdown*100/1024/settings.value("netDownTopSpinBox").toInt();
            data.y=info.netup*100/1024/settings.value("netUpTopSpinBox").toInt();
            netChart->height=settings.value("netHeightSpinBox").toInt();
            netChart->width=settings.value("netWidthSpinBox").toInt();
            netChart->color1=settings.value("netDownWidget").value<QColor>();
            netChart->color2=settings.value("netUpWidget").value<QColor>();
            netChart->borderRound=settings.value("netBorderRoundSpinBox").toInt();
            netChart->colorBorder=settings.value("netBorderWidget").value<QColor>();
            netChart->colorBackground=settings.value("netBackgroundWidget").value<QColor>();
            netChart->spacing=settings.value("chartSpacingSpinBox").toInt();

            netChart->updateChart(data);
        }

    }
    oldsettings=settings;

    //数值变化可能让文字变宽/变窄（如 99K -> 1.0M，勾选温度后行尾追加温度），sizeHint 随之改变。
    //面板只在条目增删、托盘数量变化时重算条目宽度，不会自动响应这个变化，
    //需要主动通知它重新计算，否则条目一直保持创建时的宽度，文字被截断/挤在一起。
    const QSize newHint=sizeHint();
    if(newHint!=m_lastSizeHint)
    {
        m_lastSizeHint=newHint;
        updateGeometry();
        emit requestUpdateGeometry();
    }
    //qDebug()<<"MainWidget::UpdateData() finished";
}

QSize MainWidget::sizeHint() const
{
    if(centralLayout==NULL)return QSize(100,30);
    int w,h;
    const Dock::Position position = qApp->property(PROP_POSITION).value<Dock::Position>();
    if(!oldsettings.value("chartModeCheckBox").toInt())//文字模式
    {
        switch (oldsettings.value("displayContentComboBox").toInt())
        {
            case DisplayContentSetting::CPUMEM:
            case DisplayContentSetting::AllAndTempAndBattery://全部文字都在cpuMemLabel上，算法同CPUMEM
                w=QFontMetrics(font).boundingRect(cpuMemLabel->text()).size().width()/2;
                h=QFontMetrics(font).boundingRect(cpuMemLabel->text()).size().height()*2;
                break;
            case DisplayContentSetting::NETSPEED:
                w=QFontMetrics(font).boundingRect(netLabel->text()).size().width()/2;
                h=QFontMetrics(font).boundingRect(netLabel->text()).size().height()*2;
                break;
            case DisplayContentSetting::ALL:
                if(position==Dock::Top||position==Dock::Bottom)
                {
                    w=QFontMetrics(font).boundingRect(cpuMemLabel->text()).size().width()/2+
                     QFontMetrics(font).boundingRect(netLabel->text()).size().width()/2;
                    h=qMax(QFontMetrics(font).boundingRect(cpuMemLabel->text()).size().height()*2,
                           QFontMetrics(font).boundingRect(netLabel->text()).size().height()*2);
                }
                else
                {
                    w=qMax(QFontMetrics(font).boundingRect(cpuMemLabel->text()).size().width()/2,
                           QFontMetrics(font).boundingRect(netLabel->text()).size().width()/2);
                    h=QFontMetrics(font).boundingRect(cpuMemLabel->text()).size().height()*2+
                      QFontMetrics(font).boundingRect(netLabel->text()).size().height()*2;
                }
                break;
            default:
                w=QFontMetrics(font).boundingRect(netLabel->text()).size().width()/2;
                h=QFontMetrics(font).boundingRect(netLabel->text()).size().height()*2;
                break;
        }
    }
    else//图表模式
    {
        if(position==Dock::Top||position==Dock::Bottom)
        {
            w=(cpuChart==NULL?0:cpuChart->width)+
                    (memChart==NULL?0:memChart->width)+
                    (netChart==NULL?0:netChart->width);
            h=qMax(cpuChart==NULL?0:cpuChart->height,
                   qMax(memChart==NULL?0:memChart->height,netChart==NULL?0:netChart->height));
        }
        else
        {
            w=qMax(cpuChart==NULL?0:cpuChart->width,
                   qMax(memChart==NULL?0:memChart->width,netChart==NULL?0:netChart->width));
            h=(cpuChart==NULL?0:cpuChart->height)+
                    (memChart==NULL?0:memChart->height)+
                    (netChart==NULL?0:netChart->height);
        }
    }
    QSize size(w,h);
    //qDebug()<<size;
    //qDebug()<<QString("width:%1    height:%2").arg(width()).arg(height());
    return size;
}
