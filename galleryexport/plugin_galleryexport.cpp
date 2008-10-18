/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Copyright (C) 2003-2005 by Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Copyright (C) 2006 by Colin Guthrie <kde@colin.guthr.ie>
 * Copyright (C) 2006-2008 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2008 by Andrea Diamantini <adjam7 at gmail dot com>
 *
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

// Local includes.
#include "plugin_galleryexport.h"
#include "galleries.h"
#include "gallerywindow.h"
#include "galleryconfig.h"

// libkipi includes.
#include <libkipi/interface.h>
#include <libkipi/plugin.h>

// KDE includes.
#include <KLocale>
#include <KAction>
#include <KDebug>
#include <KMessageBox>
#include <KIconLoader>
#include <KApplication>
#include <KGenericFactory>


K_PLUGIN_FACTORY(Factory, registerPlugin<Plugin_GalleryExport>();)
K_EXPORT_PLUGIN(Factory("kipiplugin_galleryexport"))


Plugin_GalleryExport::Plugin_GalleryExport(QObject *parent,
        const QVariantList&)
        : KIPI::Plugin(Factory::componentData(), parent, "GalleryExport")
{
    kDebug(51001) << "Plugin_GalleryExport plugin loaded"
    << endl;
}


void Plugin_GalleryExport::setup(QWidget* widget)
{
    mpGallery = new KIPIGalleryExportPlugin::Gallery();

    KIPI::Plugin::setup(widget);

    KIPI::Interface* interface = dynamic_cast<KIPI::Interface*>(parent());

    if (!interface) {
        kError(51000) << "Kipi interface is null!" << endl;
        return;
    }

    m_action = new KAction(i18n("remote-gallery-sync"), this);
    m_action->setText(i18n("Gallery Export..."));
    m_action->setIcon(KIcon("applications-internet"));
    connect(m_action, SIGNAL(triggered(bool)), this, SLOT(slotSync()));
    m_action->setEnabled(true);
    addAction(m_action);
}


Plugin_GalleryExport::~Plugin_GalleryExport()
{
    if (mpGallery)
        delete mpGallery;
}

// this slot uses GalleryWindow Class
void Plugin_GalleryExport::slotSync()
{
    KIPI::Interface* interface = dynamic_cast<KIPI::Interface*>(parent());
    if (!interface) {
        kError(51000) << "Kipi interface is null!" << endl;
        return;
    }

    KConfig config("galleryrc");
    if(!config.hasGroup("Gallery Settings") )
    {
        KIPIGalleryExportPlugin::GalleryEdit configDlg(kapp->activeWindow(), mpGallery, i18n("Edit Gallery Data") );
        configDlg.exec();
    }
    KIPIGalleryExportPlugin::GalleryWindow dlg(interface, kapp->activeWindow(), mpGallery);
    dlg.exec();
}


KIPI::Category Plugin_GalleryExport::category(KAction* action) const
{
    if (action == m_action)
        return KIPI::ExportPlugin;
//     if (action == m_action_configure)
//         return KIPI::ToolsPlugin;
// 
    kWarning(51000) << "Unrecognized action for plugin category identification" << endl;
    return KIPI::ExportPlugin;
}


#include "plugin_galleryexport.moc"
