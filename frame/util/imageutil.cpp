/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *
 * Maintainer: sbw <sbw@sbw.so>
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

#include "imageutil.h"

#include <QIcon>
#include <QPainter>

QPixmap ImageUtil::tintWhitePixels(const QPixmap &pixmap, const QColor &color)
{
    if (pixmap.isNull())
        return pixmap;

    QImage image = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < image.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x) {
            const QRgb px = line[x];
            const int alpha = qAlpha(px);
            if (alpha == 0)
                continue;
            const int r = qRed(px), g = qGreen(px), b = qBlue(px);
            const int maxc = qMax(r, qMax(g, b)), minc = qMin(r, qMin(g, b));
            // 亮且低饱和 = 白色/浅灰部分
            if (minc > 160 && maxc - minc < 40)
                line[x] = qRgba(color.red(), color.green(), color.blue(), alpha);
        }
    }

    QPixmap result = QPixmap::fromImage(image);
    result.setDevicePixelRatio(pixmap.devicePixelRatio());
    return result;
}

QPixmap ImageUtil::tintGrayPixels(const QPixmap &pixmap, const QColor &color) {
    if (pixmap.isNull()) {
        return pixmap;
    }

    QImage image = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < image.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x) {
            const QRgb px = line[x];
            const int alpha = qAlpha(px);
            if (alpha == 0) {
                continue;
            }
            const int r = qRed(px), g = qGreen(px), b = qBlue(px);
            if (qMax(r, qMax(g, b)) - qMin(r, qMin(g, b)) < 40)
                line[x] = qRgba(color.red(), color.green(), color.blue(), alpha);
        }
    }

    QPixmap result = QPixmap::fromImage(image);
    result.setDevicePixelRatio(pixmap.devicePixelRatio());
    return result;
}

const QPixmap ImageUtil::loadSvg(const QString &iconName, const QString &localPath, const int size, const qreal ratio)
{
    QIcon icon = QIcon::fromTheme(iconName);
    if (!icon.isNull()) {
        QPixmap pixmap = icon.pixmap(QSize(size, size), ratio);
        pixmap.setDevicePixelRatio(ratio);
        return pixmap;
    }

    QPixmap pixmap(int(size * ratio), int(size * ratio));
    QString localIcon = QString("%1%2%3").arg(localPath).arg(iconName).arg(iconName.contains(".svg") ? "" : ".svg");
    QSvgRenderer renderer(localIcon);
    pixmap.fill(Qt::transparent);

    QPainter painter;
    painter.begin(&pixmap);
    renderer.render(&painter);
    painter.end();
    pixmap.setDevicePixelRatio(ratio);

    return pixmap;
}
