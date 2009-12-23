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

#include "previewmanager.moc"

// Qt includes

#include <QStyle>
#include <QLabel>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <QFrame>

// KDE includes

#include <klocale.h>
#include <kiconloader.h>
#include <kvbox.h>

// Local includes

#include "previewimage.h"

namespace KIPIPlugins
{

class PreviewManagerPriv
{
public:

    PreviewManagerPriv()
    {
        progressPix    = SmallIcon("process-working", 22);
        progressCount  = 0;
        progressTimer  = 0;
        progressLabel  = 0;
        thumbLabel     = 0;
        busy           = false;
        textLabel      = 0;
        preview        = 0;
    }

    bool          busy;

    QLabel*       textLabel;
    QLabel*       thumbLabel;

    int           progressCount;
    QPixmap       progressPix;
    QTimer*       progressTimer;
    QLabel*       progressLabel;

    PreviewImage* preview;
};

PreviewManager::PreviewManager(QWidget* parent)
              : QStackedWidget(parent), d(new PreviewManagerPriv)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumSize(QSize(400, 300));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    KVBox* vbox      = new KVBox(this);
    QLabel* space1   = new QLabel(vbox);
    d->progressLabel = new QLabel(vbox);
    d->progressLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QLabel* space2   = new QLabel(vbox);
    d->thumbLabel    = new QLabel(vbox);
    d->thumbLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QLabel* space3   = new QLabel(vbox);
    d->textLabel     = new QLabel(vbox);
    d->textLabel->setScaledContents(true);
    d->textLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QLabel* space4   = new QLabel(vbox);

    vbox->setStretchFactor(space1, 10);
    vbox->setStretchFactor(d->progressLabel, 5);
    vbox->setStretchFactor(space2, 1);
    vbox->setStretchFactor(d->thumbLabel, 5);
    vbox->setStretchFactor(space3, 1);
    vbox->setStretchFactor(d->textLabel, 5);
    vbox->setStretchFactor(space4, 10);
    vbox->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    vbox->setLineWidth( style()->pixelMetric(QStyle::PM_DefaultFrameWidth) );

    d->preview = new PreviewImage(this);

    insertWidget(MessageMode, vbox);
    insertWidget(PreviewMode, d->preview);

    d->progressTimer = new QTimer(this);

    connect(d->progressTimer, SIGNAL(timeout()),
            this, SLOT(slotProgressTimerDone()));
}

PreviewManager::~PreviewManager()
{
    delete d;
}

void PreviewManager::load(const QString& file)
{
    setBusy(false);

    if (!d->preview->load(file))
    {
        setText(i18n( "Failed to load image after processing" ));
        return;
    }

    setCurrentIndex(PreviewMode);
    d->preview->slotZoom2Fit();
}

void PreviewManager::setThumbnail(const QPixmap& thumbnail)
{
    d->thumbLabel->setPixmap(thumbnail);
    setCurrentIndex(MessageMode);
}

void PreviewManager::setText(const QString& text, const QColor& color)
{
    d->textLabel->setText(QString("<qt text=\"%1\">%2</qt>").arg(color.name()).arg(text));
    setCurrentIndex(MessageMode);
}

void PreviewManager::setBusy(bool b, const QString& text)
{
    d->busy = b;

    if (d->busy)
    {
        setCursor( Qt::WaitCursor );
        d->progressTimer->start(300);
        setText(text);
    }
    else
    {
        unsetCursor();
        d->progressTimer->stop();
        setText(text);
    }
}

/*
void PreviewManager::resizeEvent(QResizeEvent*)
{
}
*/

void PreviewManager::slotProgressTimerDone()
{
    d->progressLabel->setPixmap(QPixmap(d->progressPix.copy(0, d->progressCount*22, 22, 22)));
    d->progressCount++;
    if (d->progressCount == 8)
        d->progressCount = 0;

    d->progressTimer->start(300);
}

} // namespace KIPIPlugins
