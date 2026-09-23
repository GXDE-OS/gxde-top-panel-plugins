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

#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H

#include <QPixmap>
#include <QColor>
#include <QSvgRenderer>

class ImageUtil
{
public:
    static const QPixmap loadSvg(const QString &iconName, const QString &localPath, const int size, const qreal ratio);
    // 把近白色的像素染成 color（保留透明度），彩色部分不动；
    // 用于浅色面板上显示主题里画成白色的 symbolic 图标
    static QPixmap tintWhitePixels(const QPixmap &pixmap, const QColor &color);
};

#endif // IMAGEUTIL_H
