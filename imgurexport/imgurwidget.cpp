/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2012-02-12
 * Description : a kipi plugin to export images to the Imgur web service
 *
 * Copyright (C) 2010-2012 by Marius Orcsik <marius at habarnam dot ro>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "imgurwidget.moc"

// Qt includes

#include <QBoxLayout>

// KDE includes

#include <klocale.h>

using namespace KIPIPlugins;

namespace KIPIImgurExportPlugin
{

ImgurWidget::ImgurWidget(Interface* const interface, QWidget* const parent)
    : QWidget(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    m_imagesList            = new ImgurImagesList(interface, this);

    m_imagesList->loadImagesFromCurrentSelection();
    m_imagesList->setAllowDuplicate(false);
    m_imagesList->setAllowRAW(false);

    QWidget* settingsBox           = new QWidget(this);
    QVBoxLayout* settingsBoxLayout = new QVBoxLayout(settingsBox);

    m_headerLbl = new QLabel(settingsBox);
    m_headerLbl->setWhatsThis(i18n("This is a clickable link to open the Imgur home page in a web browser"));
    m_headerLbl->setText(QString("<h2><a href='http://imgur.com'>imgur.com</a></h2>"));
    m_headerLbl->setOpenExternalLinks(true);
    m_headerLbl->setFocusPolicy(Qt::NoFocus);

    m_textLbl = new QLabel(settingsBox);
    m_textLbl->setText(i18n("You can retreive the image URLs from the Xmp tags:\n"
                            "\"Imgur URL\" and \"Imgur Delete URL\". \n"));
    m_textLbl->setFocusPolicy(Qt::NoFocus);

    m_progressBar = new QProgressBar(settingsBox);
    m_progressBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_progressBar->setVisible(false);

    // --------------------------------------------

    settingsBoxLayout->addWidget(m_headerLbl);
    settingsBoxLayout->addWidget(m_textLbl);

    settingsBoxLayout->addWidget(m_progressBar);
    settingsBoxLayout->setSpacing(KDialog::spacingHint());
    settingsBoxLayout->setMargin(KDialog::spacingHint());

    // --------------------------------------------

    mainLayout->addWidget(m_imagesList);
    mainLayout->addWidget(settingsBox);
    mainLayout->setSpacing(KDialog::spacingHint());
    mainLayout->setMargin(0);

    connect(m_imagesList, SIGNAL(signalAddItems(KUrl::List)),
            this, SLOT(slotAddItems(KUrl::List)));

    connect(m_imagesList, SIGNAL(signalImageListChanged()),
            this, SLOT(slotImageListChanged()));

}

void ImgurWidget::slotAddItems(const KUrl::List& list)
{
    emit signalAddItems(list);
}

void ImgurWidget::slotImageListChanged()
{
    emit signalImageListChanged();
}

ImgurWidget::~ImgurWidget()
{
    // TODO
}

ImgurImagesList* ImgurWidget::imagesList() const
{
    return m_imagesList;
}

QProgressBar* ImgurWidget::progressBar() const
{
    return m_progressBar;
}

} // namespace KIPIImgurExportPlugin
