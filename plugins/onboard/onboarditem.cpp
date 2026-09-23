/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     listenerri <listenerri@gmail.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "onboarditem.h"

#include <QSvgRenderer>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QIcon>
#include <QPainterPath>

#include <DStyle>
#include <DGuiApplicationHelper>

DWIDGET_USE_NAMESPACE;

OnboardItem::OnboardItem(QWidget *parent)
    : QWidget(parent)
    , m_hover(false)
    , m_pressed(false)
{
    setMouseTracking(true);
    setMinimumSize(PLUGIN_BACKGROUND_MIN_SIZE, PLUGIN_BACKGROUND_MIN_SIZE);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [ = ] {
        update();
    });
    m_icon = QIcon::fromTheme(":/icons/icon/deepin-virtualkeyboard.svg");
}

void OnboardItem::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPixmap pixmap;
    // 只画按键图形（白色、透明背景），不走主题查找；
    // 主题里的 deepin-virtualkeyboard 是带卡片底的图标，且 SVG 带 Qt 不支持的 filter
    QString iconName = ":/icons/icon/keyboard-keys.svg";
    int iconSize = PLUGIN_ICON_MAX_SIZE;

    QPainter painter(this);
    if (std::min(width(), height()) > PLUGIN_BACKGROUND_MIN_SIZE) {
        // 平时不画背景泡泡，只在悬停/按下时给轻微反馈
        if (m_hover || m_pressed) {
            QColor color;
            if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
                color = Qt::black;
                painter.setOpacity(m_pressed ? 0.3 : 0.2);
            } else {
                color = Qt::white;
                painter.setOpacity(m_pressed ? 0.05 : 0.2);
            }

            painter.setRenderHint(QPainter::Antialiasing, true);

            DStyleHelper dstyle(style());
            const int radius = dstyle.pixelMetric(DStyle::PM_FrameRadius);

            QPainterPath path;

            int minSize = std::min(width(), height());
            QRect rc(0, 0, minSize, minSize);
            rc.moveTo(rect().center() - rc.center());

            path.addRoundedRect(rc, radius, radius);
            painter.fillPath(path, color);
        }
    }

    pixmap = loadSvg(iconName, QSize(iconSize, iconSize));

    // 浅色面板下把白色按键着成深灰，否则看不见
    if (!pixmap.isNull() && DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        QPainter pixPainter(&pixmap);
        pixPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        pixPainter.fillRect(pixmap.rect(), QColor(50, 50, 50));
    }

    painter.setOpacity(1);
    const QRectF &rf = QRectF(rect());
    const QRectF &rfp = QRectF(pixmap.rect());
    painter.drawPixmap(rf.center() - rfp.center() / devicePixelRatioF(), pixmap);
}

const QPixmap OnboardItem::loadSvg(const QString &fileName, const QSize &size) const
{
    const auto ratio = devicePixelRatioF();

    QPixmap pixmap;
    if (fileName.startsWith(":")) {
        pixmap = QIcon(fileName).pixmap(size * ratio);
    } else {
        pixmap = QIcon::fromTheme(fileName, m_icon).pixmap(size * ratio);
    }
    pixmap.setDevicePixelRatio(ratio);

    return pixmap;
}

void OnboardItem::mousePressEvent(QMouseEvent *event)
{
    m_pressed = true;
    update();

    QWidget::mousePressEvent(event);
}

void OnboardItem::mouseReleaseEvent(QMouseEvent *event)
{
    m_pressed = false;
    m_hover = false;
    update();

    QWidget::mouseReleaseEvent(event);
}

void OnboardItem::mouseMoveEvent(QMouseEvent *event)
{
    m_hover = true;

    QWidget::mouseMoveEvent(event);
}

void OnboardItem::leaveEvent(QEvent *event)
{
    m_hover = false;
    m_pressed = false;
    update();

    QWidget::leaveEvent(event);
}

void OnboardItem::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}
