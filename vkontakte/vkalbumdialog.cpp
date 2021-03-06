/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2011-02-19
 * Description : a kipi plugin to export images to VKontakte web service
 *
 * Copyright (C) 2011 by Roman Tsisyk <roman at tsisyk dot com>
 * Copyright (C) 2011 by Alexander Potashev <aspotashev at gmail dot com>
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

#include "vkalbumdialog.h"

// Qt includes

#include <QtGui/QFormLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

// KDE includes

#include <klocale.h>
#include <kdebug.h>
#include <kdialog.h>
#include <klineedit.h>
#include <ktextedit.h>
#include <kmessagebox.h>
#include <kcombobox.h>

// libvkontakte includes

#include <libkvkontakte/albuminfo.h>

namespace KIPIVkontaktePlugin
{

VkontakteAlbumDialog::VkontakteAlbumDialog(QWidget *parent, Vkontakte::AlbumInfoPtr album, bool editing)
    : KDialog(parent), m_album(album)
{
    setWindowTitle(editing ?
        i18nc("@title:window", "Edit album") :
        i18nc("@title:window", "New album"));
    setButtons(KDialog::Ok | KDialog::Cancel);
    setDefaultButton(KDialog::Ok);

    QWidget *mainWidget = new QWidget(this);
    setMainWidget(mainWidget);
    mainWidget->setMinimumSize(400, 300);

    QGroupBox *albumBox = new QGroupBox(i18nc("@title:group Header above Title and Summary fields", "Album"), mainWidget);
    albumBox->setWhatsThis(
        i18n("These are basic settings for the new VKontakte album."));

    m_titleEdit = new KLineEdit(album->title());
    m_titleEdit->setWhatsThis(i18n("Title of the album that will be created (required)."));

    m_summaryEdit = new KTextEdit(album->description());
    m_summaryEdit->setWhatsThis(i18n("Description of the album that will be created (optional)."));


    QFormLayout *albumBoxLayout  = new QFormLayout;
    albumBoxLayout->addRow(i18n("Title:"), m_titleEdit);
    albumBoxLayout->addRow(i18n("Summary:"), m_summaryEdit);
    albumBox->setLayout(albumBoxLayout);

    QGroupBox *privacyBox = new QGroupBox(i18n("Privacy Settings"), mainWidget);
    QGridLayout *privacyBoxLayout = new QGridLayout;

    m_albumPrivacyCombo = new KComboBox(privacyBox);
    m_albumPrivacyCombo->addItem(i18n("Only me"), QVariant(Vkontakte::AlbumInfo::PRIVACY_PRIVATE));
    m_albumPrivacyCombo->addItem(i18n("My friends"), QVariant(Vkontakte::AlbumInfo::PRIVACY_FRIENDS));
    m_albumPrivacyCombo->addItem(i18n("Friends of my friends"), QVariant(Vkontakte::AlbumInfo::PRIVACY_FRIENDS_OF_FRIENDS));
    m_albumPrivacyCombo->addItem(i18n("Everyone"), QVariant(Vkontakte::AlbumInfo::PRIVACY_PUBLIC));
    privacyBoxLayout->addWidget(new QLabel(i18n("Album available to:")), 0, 0);
    privacyBoxLayout->addWidget(m_albumPrivacyCombo, 0, 1);

    m_commentsPrivacyCombo = new KComboBox(privacyBox);
    m_commentsPrivacyCombo->addItem(i18n("Only me"), QVariant(Vkontakte::AlbumInfo::PRIVACY_PRIVATE));
    m_commentsPrivacyCombo->addItem(i18n("My friends"), QVariant(Vkontakte::AlbumInfo::PRIVACY_FRIENDS));
    m_commentsPrivacyCombo->addItem(i18n("Friends of my friends"), QVariant(Vkontakte::AlbumInfo::PRIVACY_FRIENDS_OF_FRIENDS));
    m_commentsPrivacyCombo->addItem(i18n("Everyone"), QVariant(Vkontakte::AlbumInfo::PRIVACY_PUBLIC));
    privacyBoxLayout->addWidget(new QLabel(i18n("Comments available to:")), 1, 0);
    privacyBoxLayout->addWidget(m_commentsPrivacyCombo, 1, 1);

    privacyBox->setLayout(privacyBoxLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->addWidget(albumBox);
    mainLayout->addWidget(privacyBox);
    mainLayout->setSpacing(KDialog::spacingHint());
    mainWidget->setLayout(mainLayout);

    if (editing)
    {
        m_titleEdit->setText(album->title());
        m_summaryEdit->setText(album->description());
        m_albumPrivacyCombo->setCurrentIndex(m_albumPrivacyCombo->findData(album->privacy()));
        m_commentsPrivacyCombo->setCurrentIndex(m_commentsPrivacyCombo->findData(album->commentPrivacy()));
    }

    m_titleEdit->setFocus();
}

VkontakteAlbumDialog::~VkontakteAlbumDialog()
{
    // nothing
}

void VkontakteAlbumDialog::slotButtonClicked(int button)
{
    if (button == KDialog::Ok)
    {
        if (m_titleEdit->text().isEmpty())
        {
            KMessageBox::error(this, i18n("Title cannot be empty."),
                               i18n("Error"));
            return;
        }

        m_album->setTitle(m_titleEdit->text());
        m_album->setDescription(m_summaryEdit->toPlainText());

        if (m_albumPrivacyCombo->currentIndex() != -1)
            m_album->setPrivacy(m_albumPrivacyCombo->itemData(m_albumPrivacyCombo->currentIndex()).toInt());
        else // for safety, see info about VK API bug below
            m_album->setPrivacy(Vkontakte::AlbumInfo::PRIVACY_PRIVATE);

        if (m_commentsPrivacyCombo->currentIndex() != -1)
            m_album->setCommentPrivacy(m_commentsPrivacyCombo->itemData(m_commentsPrivacyCombo->currentIndex()).toInt());
        else // VK API has a bug: if "comment_privacy" is not set, it will be set to PRIVACY_PUBLIC
            m_album->setCommentPrivacy(Vkontakte::AlbumInfo::PRIVACY_PRIVATE);
    }

    return KDialog::slotButtonClicked(button);
}

} // namespace KIPIVkontaktePlugin

#include "vkalbumdialog.moc"
