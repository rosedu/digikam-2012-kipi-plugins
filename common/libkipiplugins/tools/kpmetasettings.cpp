/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2012-02-13
 * Description : Metadata Settings Container for kipi-plugins.
 *
 * Copyright (C) 2012 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "kpmetasettings.h"

namespace KIPIPlugins
{

KPMetaSettings::KPMetaSettings()
{
    writeRawFiles         = false;
    updateFileTimeStamp   = false;
    useXMPSidecar4Reading = false;
    metadataWritingMode   = KPMetadata::WRITETOIMAGEONLY;
}

KPMetaSettings::~KPMetaSettings()
{
}

}  // namespace KIPIPlugins
