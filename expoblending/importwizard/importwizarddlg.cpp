/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2009-11-13
 * Description : a plugin to blend bracketed images.
 *
 * Copyright (C) 2009-2011 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "importwizarddlg.moc"

// Qt includes

#include <QDesktopWidget>
#include <QApplication>

// KDE includes

#include <kmenu.h>
#include <klocale.h>
#include <khelpmenu.h>
#include <kpushbutton.h>
#include <ktoolinvocation.h>

// LibKIPI includes

#include <libkipi/interface.h>

// Locale incudes.

#include "manager.h"
#include "actionthread.h"
#include "aboutdata.h"
#include "intropage.h"
#include "itemspage.h"
#include "preprocessingpage.h"
#include "lastpage.h"

namespace KIPIExpoBlendingPlugin
{

class ImportWizardDlg::ImportWizardDlgPriv
{
public:

    ImportWizardDlgPriv()
    {
        mngr              = 0;
        introPage         = 0;
        itemsPage         = 0;
        preProcessingPage = 0;
        lastPage          = 0;
    }

    Manager*           mngr;

    IntroPage*         introPage;
    ItemsPage*         itemsPage;
    PreProcessingPage* preProcessingPage;
    LastPage*          lastPage;
};

ImportWizardDlg::ImportWizardDlg(Manager* mngr, QWidget* parent)
               : KAssistantDialog(parent), d(new ImportWizardDlgPriv)
{
    setModal(false);
    setWindowTitle(i18n("Exposure Blending Import Wizard"));

    d->mngr              = mngr;
    d->introPage         = new IntroPage(d->mngr, this);
    d->itemsPage         = new ItemsPage(d->mngr, this);
    d->preProcessingPage = new PreProcessingPage(d->mngr, this);
    d->lastPage          = new LastPage(d->mngr, this);

    // ---------------------------------------------------------------
    // About data and help button.

    disconnect(this, SIGNAL(helpClicked()),
               this, SLOT(slotHelp()));

    KHelpMenu* helpMenu = new KHelpMenu(this, d->mngr->about(), false);
    helpMenu->menu()->removeAction(helpMenu->menu()->actions().first());
    QAction *handbook   = new QAction(i18n("Handbook"), this);
    connect(handbook, SIGNAL(triggered(bool)),
            this, SLOT(slotHelp()));
    helpMenu->menu()->insertAction(helpMenu->menu()->actions().first(), handbook);
    button(Help)->setMenu(helpMenu->menu());

    // ---------------------------------------------------------------

    QDesktopWidget* desktop = QApplication::desktop();
    int screen = desktop->screenNumber();
    QRect srect = desktop->availableGeometry(screen);
    resize(800 <= srect.width()  ? 800 : srect.width(),
           750 <= srect.height() ? 750 : srect.height());
//     resize(800, 580);

    connect(d->introPage, SIGNAL(signalIntroPageIsValid(bool)),
            this, SLOT(slotIntroPageIsValid(bool)));

    connect(d->itemsPage, SIGNAL(signalItemsPageIsValid(bool)),
            this, SLOT(slotItemsPageIsValid(bool)));

    connect(d->preProcessingPage, SIGNAL(signalPreProcessed(ItemUrlsMap)),
            this, SLOT(slotPreProcessed(ItemUrlsMap)));

    setValid(d->introPage->page(), d->introPage->binariesFound());
}

ImportWizardDlg::~ImportWizardDlg()
{
    delete d;
}

void ImportWizardDlg::slotHelp()
{
    KToolInvocation::invokeHelp("expoblending", "kipi-plugins");
}

Manager* ImportWizardDlg::manager() const
{
    return d->mngr;
}

KUrl::List ImportWizardDlg::itemUrls() const
{
    return d->itemsPage->itemUrls();
}

void ImportWizardDlg::next()
{
    if (currentPage() == d->itemsPage->page())
    {
        d->mngr->setItemsList(d->itemsPage->itemUrls());
    }
    else if (currentPage() == d->preProcessingPage->page())
    {
        // Do not give access to Next button during alignment process.
        setValid(d->preProcessingPage->page(), false);
        d->preProcessingPage->process();
        // Next is handled with signals/slots
        return;
    }

    KAssistantDialog::next();
}

void ImportWizardDlg::back()
{
    if (currentPage() == d->preProcessingPage->page())
    {
        d->preProcessingPage->cancel();
        KAssistantDialog::back();
        setValid(d->preProcessingPage->page(), true);
        return;
    }

    KAssistantDialog::back();
}

void ImportWizardDlg::slotIntroPageIsValid(bool binariesFound)
{
    setValid(d->introPage->page(), binariesFound);
}

void ImportWizardDlg::slotPreProcessed(const ItemUrlsMap& map)
{
    if (map.isEmpty())
    {
        // pre-processing failed.
        setValid(d->preProcessingPage->page(), false);
    }
    else
    {
        // pre-processing Done.
        d->mngr->setPreProcessedMap(map);
        KAssistantDialog::next();
    }
}

void ImportWizardDlg::slotItemsPageIsValid(bool valid)
{
    setValid(d->itemsPage->page(), valid);
}

} // namespace KIPIExpoBlendingPlugin
