/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2004-05-04
 * Description : Batch progress dialog
 *
 * Copyright (C) 2004-2011 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

/** @file batchprogressdialog.h */

#ifndef BATCHPROGRESSDIALOG_H
#define BATCHPROGRESSDIALOG_H

// KDE includes

#include <kdialog.h>
#include <kvbox.h>

// Local includes

#include "kipiplugins_export.h"

class QListWidget;
class QProgressBar;

namespace KIPIPlugins
{

enum ActionMessageType
{
    StartingMessage = 0,
    SuccessMessage,
    WarningMessage,
    ErrorMessage,
    ProgressMessage
};

// --------------------------------------------------------------------------------------

class KIPIPLUGINS_EXPORT BatchProgressWidget : public KVBox
{
    Q_OBJECT

public:

    explicit BatchProgressWidget(QWidget* parent=0);
    ~BatchProgressWidget();

    QListWidget*  listView() const;
    QProgressBar* progressBar() const;

    void addedAction(const QString& text, int type);
    void reset();
    void setProgress(int current, int total);

    int progress() const;
    int total() const;

public Q_SLOTS:

    void setProgress(int);
    void setTotal(int total);

private Q_SLOTS:

    void slotContextMenu();
    void slotCopy2ClipBoard();

private:

    class BatchProgressWidgetPriv;
    BatchProgressWidgetPriv* const d;
};

// --------------------------------------------------------------------------------------

class KIPIPLUGINS_EXPORT BatchProgressDialog : public KDialog
{
    Q_OBJECT

public:

    explicit BatchProgressDialog(QWidget* parent=0, const QString& caption=QString());
    ~BatchProgressDialog();

    BatchProgressWidget* progressWidget() const;

private:

    class BatchProgressDialogPriv;
    BatchProgressDialogPriv* const d;
};

}  // namespace KIPIPlugins

#endif  // BATCHPROGRESSDIALOG_H
