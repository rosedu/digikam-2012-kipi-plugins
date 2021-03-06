/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2009-11-13
 * Description : a plugin to blend bracketed images.
 *
 * Copyright (C) 2009-2012 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef FLASHMANAGER_H
#define FLASHMANAGER_H

// Qt includes

#include <QObject>

// KDE includes

#include <kurl.h>


// Local includes

//#include "actions.h"
#include "simpleviewersettingscontainer.h"
#include "simpleviewer.h"

namespace KIPI
{
    class Interface;
}

using namespace KIPI;

namespace KIPIFlashExportPlugin
{

class FlashExportAboutData;  // must write own flashexport aboutdata
class SimpleViewer;
class FlashManagerPriv;

class FlashManager: public QObject
{
    Q_OBJECT

public:

    FlashManager(QObject* const parent=0);
    ~FlashManager();

// replace with flashexport
    void setAbout(FlashExportAboutData* const about);
    FlashExportAboutData* about() const;

    void setIface(Interface* const iface);
    Interface* iface() const;

    bool installPlugin(const KUrl& url);

    SimpleViewer* simpleView() const;

    void initSimple();

    void run();

private:

    void startWizard();

private:

    class FlashManagerPriv;
    FlashManagerPriv* const d;
};

} // namespace KIPIExpoBlendingPlugin

#endif /* FLASHMANAGER_H */
