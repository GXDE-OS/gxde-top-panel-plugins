#include "tipswidget.h"

#include <DGuiApplicationHelper>

#include <QPainter>

TipsWidget::TipsWidget(QWidget *parent) : QFrame(parent)
{

}

void TipsWidget::setText(const QString &text)
{
    m_text = text;

    setFixedSize(fontMetrics().horizontalAdvance(text) + 6, fontMetrics().height());

    update();
}

void TipsWidget::refreshFont()
{
    setFixedSize(fontMetrics().horizontalAdvance(m_text) + 6, fontMetrics().height());
    update();
}

void TipsWidget::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
    refreshFont();

    QPainter painter(this);
    const bool dark = Dtk::Gui::DGuiApplicationHelper::instance()->themeType()
        == Dtk::Gui::DGuiApplicationHelper::DarkType;
    painter.setPen(QPen(dark ? QColor(Qt::white) : QColor(Qt::black), 1));

    QTextOption option;
    option.setAlignment(Qt::AlignCenter);
    painter.drawText(rect(), m_text, option);
}
