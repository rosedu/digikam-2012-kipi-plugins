/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2007-10-18
 * Description : XMP content settings page.
 *
 * Copyright (C) 2007-2012 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef XMP_CONTENT_H
#define XMP_CONTENT_H

// Qt includes

#include <QWidget>
#include <QByteArray>

namespace KIPIMetadataEditPlugin
{

class XMPContent : public QWidget
{
    Q_OBJECT

public:

    XMPContent(QWidget* parent);
    ~XMPContent();

    void applyMetadata(QByteArray& exifData, QByteArray& xmpData);
    void readMetadata(QByteArray& xmpData);

    bool syncJFIFCommentIsChecked() const;
    bool syncHOSTCommentIsChecked() const;
    bool syncEXIFCommentIsChecked() const;

    void setCheckedSyncJFIFComment(bool c);
    void setCheckedSyncHOSTComment(bool c);
    void setCheckedSyncEXIFComment(bool c);

    QString getXMPCaption() const;

Q_SIGNALS:

    void signalModified();

private Q_SLOTS:

    void slotSyncOptionsEnabled(bool);

private:

    class XMPContentPriv;
    XMPContentPriv* const d;
};

}  // namespace KIPIMetadataEditPlugin

#endif // XMP_CONTENT_H 
