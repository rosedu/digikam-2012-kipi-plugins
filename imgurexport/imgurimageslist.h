/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2010-02-04
 * Description : a tool to export images to imgur.com
 *
 * Copyright (C) 2010-2012 by Marius Orcsik <marius at habarnam dot ro>
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

#ifndef IMGURIMAGESLIST_H
#define IMGURIMAGESLIST_H

// Qt includes

#include <QWidget>

// LibKIPI includes

#include <libkipi/interface.h>

// Local includes

#include "kpimageslist.h"

using namespace KIPI;
using namespace KIPIPlugins;

namespace KIPIImgurExportPlugin
{

class ImgurImagesList : public KPImagesList
{
    Q_OBJECT

public:

    ImgurImagesList(Interface* const iface, QWidget* const parent = 0);
    ~ImgurImagesList();

public:

    // implement this, if you have special item widgets, e.g. an edit line
    // they will be set automatically when adding items, changing order, etc.
    virtual void updateItemWidgets();
};

} // namespace KIPIImgurTalkerPlugin

#endif // IMGURIMAGESLIST_H
