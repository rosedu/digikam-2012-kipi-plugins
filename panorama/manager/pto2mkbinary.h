/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2011-05-23
 * Description : Autodetect pto2mk binary program and version
 *
 * Copyright (C) 2011 by Benjamin Girault <benjamin dot girault at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef PTO2MKBINARY_H
#define PTO2MKBINARY_H

// Local includes

#include "binaryiface.h"

using namespace KIPIPlugins;

namespace KIPIPanoramaPlugin
{

class Pto2MkBinary : public BinaryIface
{

public:

    Pto2MkBinary();
    ~Pto2MkBinary();

    QString minimalVersion() const;
    KUrl    url()            const;
    QString projectName()    const;

protected:
    virtual bool parseHeader(const QString& output);
};

} // namespace KIPIPanoramaPlugin

#endif  // PTO2MKBINARY_H
