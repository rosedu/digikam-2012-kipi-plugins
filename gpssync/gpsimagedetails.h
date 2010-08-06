/* ============================================================
 *
 * Date        : 2010-08-06
 * Description : A widget to show details about images
 *
 * Copyright (C) 2010 by Michael G. Hansen <mike at mghansen dot de>
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

#ifndef GPSIMAGEDETAILS_H
#define GPSIMAGEDETAILS_H

// KDE includes

#include <kurl.h>

// Qt includes

#include <QModelIndex>
#include <QWidget>

// local includes

class KConfigGroup;

namespace KIPIGPSSyncPlugin
{

class KipiImageModel;
class GPSUndoCommand;

class GPSImageDetails : public QWidget
{
Q_OBJECT

public:

    GPSImageDetails(QWidget* const parent, KipiImageModel* const imageModel, const int marginHint, const int spacingHint);
    ~GPSImageDetails();

    void setUIEnabledExternal(const bool state);
    void saveSettingsToGroup(KConfigGroup* const group);
    void readSettingsFromGroup(const KConfigGroup* const group);

Q_SIGNALS:
    void signalUndoCommand(GPSUndoCommand* undoCommand);

public Q_SLOTS:
    void slotSetCurrentImage(const QModelIndex& index);

private Q_SLOTS:
    void updateUIState();

private:

    class GPSImageDetailsPrivate;
    GPSImageDetailsPrivate* const d;
};

} /* KIPIGPSSyncPlugin */

#endif /* GPSIMAGEDETAILS_H */

