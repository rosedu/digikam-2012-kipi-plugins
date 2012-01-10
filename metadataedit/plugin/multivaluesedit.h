/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2007-10-08
 * Description : a widget to edit a tag with multiple fixed values.
 *
 * Copyright (C) 2007-2011 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef MULTI_VALUES_EDIT_H
#define MULTI_VALUES_EDIT_H

// Qt includes

#include <QWidget>

namespace KIPIMetadataEditPlugin
{

class MultiValuesEdit : public QWidget
{
    Q_OBJECT

public:

    MultiValuesEdit(QWidget* parent, const QString& title, const QString& desc);
    ~MultiValuesEdit();

    void setValid(bool v);
    bool isValid() const;

    void setData(const QStringList& data);
    QStringList getData() const;

    void setValues(const QStringList& values);
    bool getValues(QStringList& oldValues, QStringList& newValues);

Q_SIGNALS:

    void signalModified();

private Q_SLOTS:

    void slotSelectionChanged();
    void slotAddValue();
    void slotDeleteValue();
    void slotReplaceValue();

private:

    class MultiValuesEditPriv;
    MultiValuesEditPriv* const d;
};

}  // namespace KIPIMetadataEditPlugin

#endif // MULTI_VALUES_EDIT_H
