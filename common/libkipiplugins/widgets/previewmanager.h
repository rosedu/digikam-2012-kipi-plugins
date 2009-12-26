/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2009-12-23
 * Description : a widget to manage preview.
 *
 * Copyright (C) 2009 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef PREVIEW_MANAGER_H
#define PREVIEW_MANAGER_H

// Qt includes

#include <QStackedWidget>
#include <QString>
#include <QColor>

// Local includes

#include "kipiplugins_export.h"

namespace KIPIPlugins
{
class PreviewManagerPriv;

class KIPIPLUGINS_EXPORT PreviewManager : public QStackedWidget
{
    Q_OBJECT

public:

    enum DisplayMode
    {
        MessageMode = 0,
        PreviewMode
    };

public:

    PreviewManager(QWidget* parent);
    ~PreviewManager();

    void load(const QString& file);
    void setText(const QString& text, const QColor& color=Qt::white);
    void setBusy(bool b, const QString& text=QString());
    void setThumbnail(const QPixmap& preview=QPixmap());
    void setButtonText(const QString& text);
    void setButtonVisible(bool b);

Q_SIGNALS:

    void signalButtonClicked();

private Q_SLOTS:

    void slotProgressTimerDone();

private:

    PreviewManagerPriv* const d;
};

} // namespace KIPIPlugins

#endif /* PREVIEW_MANAGER_H */