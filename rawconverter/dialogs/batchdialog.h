/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2003-10-24
 * Description : Raw converter batch dialog
 *
 * Copyright (C) 2003-2005 by Renchi Raju <renchi dot raju at gmail dot com>
 * Copyright (C) 2006-2012 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef BATCHDIALOG_H
#define BATCHDIALOG_H

// KDE includes

#include <kdialog.h>
#include <kurl.h>

// Local includes

#include "kpaboutdata.h"

class QCloseEvent;

namespace KIPI
{
    class Interface;
}

using namespace KIPI;

namespace KIPIRawConverterPlugin
{

class ActionData;

class BatchDialog : public KDialog
{

Q_OBJECT

public:

    BatchDialog(Interface* const iface);
    ~BatchDialog();

    void addItems(const KUrl::List& itemList);

protected:

    void closeEvent(QCloseEvent*);

private:

    void readSettings();
    void saveSettings();

    void busy(bool busy);

    void processOne();
    void processed(const KUrl& url, const QString& tmpFile);
    void processingFailed(const KUrl& url);

private Q_SLOTS:

    void slotDefault();
    void slotClose();
    void slotHelp();
    void slotStartStop();
    void slotAborted();
    void slotIdentify();
    void slotSaveFormatChanged();

    void slotAction(const KIPIRawConverterPlugin::ActionData&);

    void slotSixteenBitsImageToggled(bool);

private:

    class BatchDialogPriv;
    BatchDialogPriv* const d;
};

} // namespace KIPIRawConverterPlugin

#endif /* BATCHDIALOG_H */
