/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2003-12-03
 * Description : a class to manage JPEGLossLess plugin
 *               actions using threads
 *
 * Copyright (C) 2003-2005 by Renchi Raju <renchi dot raju at gmail dot com>
 * Copyright (C) 2004-2011 by Marcel Wiesweg <marcel dot wiesweg at gmx dot de>
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

#ifndef ACTIONTHREAD_H
#define ACTIONTHREAD_H

// Qt includes

#include <QThread>

// KDE includes

#include <kurl.h>

// Local includes

#include "actions.h"
#include "kpactionthreadbase.h"

namespace KIPI
{
    class Interface;
}

using namespace KIPI;
using namespace KIPIPlugins;

namespace KIPIJPEGLossLessPlugin
{

class ActionThread : public KPActionThreadBase
{
    Q_OBJECT

public:

    ActionThread(Interface* const interface, QObject* const parent);
    ~ActionThread();

    void rotate(const KUrl::List& urlList, RotateAction val);
    void flip(const KUrl::List& urlList, FlipAction val);
    void convert2grayscale(const KUrl::List& urlList);

Q_SIGNALS:

    void starting(const KUrl& url, int action);
    void finished(const KUrl& url, int action);
    void failed(const KUrl& url, int action, const QString& errString);

private Q_SLOTS:

    void slotJobDone(ThreadWeaver::Job*);
    void slotJobStarted(ThreadWeaver::Job*);

private:

    bool  m_updateFileStamp;

    class Task;
};

}  // namespace KIPIJPEGLossLessPlugin

#endif /* ACTIONTHREAD_H */
