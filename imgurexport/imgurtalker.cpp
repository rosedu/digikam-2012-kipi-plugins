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

#include "imgurtalker.moc"

// Qt includes

#include <QVariant>

// KDE includes

#include <kdebug.h>
#include <kio/job.h>

// LibKIPI includes

#include <libkipi/imagecollection.h>

// qJson include

#include <qjson/parser.h>

// Local includes

#include "mpform.h"
#include "kpversion.h"

namespace KIPIImgurExportPlugin
{

ImgurTalker::ImgurTalker(Interface* const interface, QWidget* const parent)
{
    m_parent               = parent;
    m_interface            = interface;

    m_job                  = 0;
    m_userAgent            = QString("KIPI-Plugins-ImgurTalker/" + kipipluginsVersion());

    m_apiKey               = _IMGUR_API_KEY;

    m_queue                = new KUrl::List();
    ImageCollection images = interface->currentSelection();

    if (images.isValid())
    {
        slotAddItems(images.images());
    }
}

ImgurTalker::~ImgurTalker()
{
    if (m_job)
    {
        kDebug() << "Killing job";
        m_job->kill();
    }
}

void ImgurTalker::slotData(KIO::Job* j, const QByteArray& data)
{
    if (data.isEmpty())
    {
        return;
    }

    int oldSize = m_buffer.size();
    int newSize = data.size();

    m_buffer.resize(m_buffer.size() + newSize);
    memcpy(m_buffer.data()+oldSize, data.data(), newSize);

    emit signalUploadProgress(j->percent());
}

void ImgurTalker::slotResult(KJob* kjob)
{
    KIO::Job* job = static_cast<KIO::Job*>(kjob);

    if ( job->error() )
    {
        ImgurError err;
        err.message = tr("Upload failed");
        emit signalError(err); //job->errorString()
        kDebug() << "Error :" << job->errorString();
    }

    bool parseOk = false;

    switch(m_state)
    {
        case IE_REMOVEPHOTO:
            parseOk = parseResponseImageRemove(m_buffer);
            break;

        case IE_ADDPHOTO:
            parseOk = parseResponseImageUpload(m_buffer);
            break;
        default:
            break;
    }

    m_buffer.resize(0);

    emit signalUploadDone();
    emit signalBusy(false);

    return;
}

bool ImgurTalker::parseResponseImageRemove(const QByteArray& data)
{
    if (data.isEmpty())
    {
        // TODO
    }
    return false;
}

bool ImgurTalker::parseResponseImageUpload(const QByteArray& data)
{
    bool ok = false;

    if (data.isEmpty())
        return false;

    QJson::Parser* p = new QJson::Parser();
    QVariant       r = p->parse(data, &ok);

    if (ok)
    {
        QMap<QString, QVariant> m = r.toMap();
        QString responseType      = m.begin().key();

        if (responseType == "error")
        {
            ImgurError error;
            QMap<QString,QVariant> errData = m.begin().value().toMap();

            for (QMap<QString,QVariant>::iterator it = errData.begin(); it != errData.end(); ++it)
            {
                QString v = it.value().toString();

                if (it.key() == "message")
                {
                    error.message = v;
                }
                if (it.key() == "request")
                {
                    error.request = v;
                }

                if (it.key() == "method")
                {
                    if ( v == "get")
                    {
                        error.method = ImgurError::GET;
                    }
                    if ( v == "post")
                    {
                        error.method = ImgurError::POST;
                    }
                }

                if (it.key() == "format")
                {
                    if ( v == "json")
                    {
                        error.format = ImgurError::JSON;
                    }
                    if ( v == "xml")
                    {
                        error.format = ImgurError::XML;
                    }
                }

                if (it.key() == "parameters")
                {
                    error.parameters =  v;
                }
            }

            emit signalError(error); // p->errorString()
            return false;
        }

        if (responseType == "upload" )
        {
            ImgurSuccess success;
            QMap<QString, QVariant> successData = m.begin().value().toMap();

            for (QMap<QString,QVariant>::iterator it = successData.begin(); it != successData.end(); ++it)
            {
                if (it.key() == "image")
                {
                    QMap<QString, QVariant> v = it.value().toMap();

                    for (QMap<QString,QVariant>::iterator it = v.begin(); it != v.end(); ++it)
                    {
                        QString value = it.value().toString();
                        if (it.key() == "name")
                        {
                            success.image.name = value;
                        }
                        if (it.key() == "title")
                        {
                            success.image.title = value;
                        }
                        if (it.key() == "caption")
                        {
                            success.image.caption = value;
                        }
                        if (it.key() == "hash")
                        {
                            success.image.hash = value;
                        }
                        if (it.key() == "deleteHash")
                        {
                            success.image.deletehash = value;
                        }
                        if (it.key() == "dateTime")
                        {
                            //success.image.datetime = QDateTime(value);
                        }
                        if (it.key() == "type")
                        {
                            success.image.type = value;
                        }
                        if (it.key() == "animated")
                        {
                            success.image.animated = (value == "true");
                        }
                        if (it.key() == "width")
                        {
                            success.image.width = value.toInt();
                        }
                        if (it.key() == "height")
                        {
                            success.image.height = value.toInt();
                        }
                        if (it.key() == "size")
                        {
                            success.image.size = value.toInt();
                        }
                        if (it.key() == "views")
                        {
                            success.image.views = value.toInt();
                        }
                        if (it.key() == "bandwidth")
                        {
                            success.image.bandwidth = value.toLongLong();
                        }
                    }
                }
                if (it.key() == "links")
                {
                    QMap<QString, QVariant> v = it.value().toMap();

                    for (QMap<QString,QVariant>::iterator it = v.begin(); it != v.end(); ++it)
                    {
                        QString value = it.value().toString();

                        if (it.key() == "original")
                        {
                            success.links.original = value;
                        }
                        if (it.key() == "imgur_page")
                        {
                            success.links.imgur_page = value;
                        }
                        if (it.key() == "delete_page")
                        {
                            success.links.delete_page = value;
                        }
                        if (it.key() == "small_square")
                        {
                            success.links.small_square = value;
                        }
                        if (it.key() == "largeThumbnail")
                        {
                            success.links.large_thumbnail = value;
                        }
                    }
                }
            }

            emit signalSuccess(success);
//             kDebug() << "Link:" << success.links.imgur_page;
//             kDebug() << "Delete:" << success.links.delete_page;
        }
    }
    else
    {
        ImgurError error;
        error.message = "Parse error";

        emit signalError (error); // p->errorString()
        kDebug() << "Parser error :" << p->errorString();
        return false;
    }

    return true;
}

bool ImgurTalker::imageUpload(const KUrl& filePath)
{
    kDebug() << "Upload image" << filePath;
    m_currentUrl   = filePath;

    MPForm form;

    KUrl exportUrl = KUrl("http://api.imgur.com/2/upload.json");
    exportUrl.addQueryItem("key", m_apiKey);

    exportUrl.addQueryItem("name", filePath.fileName());
    exportUrl.addQueryItem("title", filePath.fileName()); // this should be replaced with something the user submits
//    exportUrl.addQueryItem("caption", ""); // this should be replaced with something the user submits

    exportUrl.addQueryItem("type", "file");

    form.addFile("image", filePath.path());
    form.finish();

    KIO::TransferJob* job = KIO::http_post(exportUrl, form.formData(), KIO::HideProgressInfo);
    job->addMetaData("content-type", form.contentType());
    job->addMetaData("content-length", QString("Content-Length: %1").arg(form.formData().length()));
    job->addMetaData("UserAgent", m_userAgent);

    connect(job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(slotData(KIO::Job*, const QByteArray&)));

    connect(job, SIGNAL(result(KJob*)),
            this, SLOT(slotResult(KJob*)));

    m_state = IE_ADDPHOTO;

    emit signalUploadStart(filePath);
    emit signalBusy(true);

    return true;
}

bool ImgurTalker::imageRemove(const QString& delete_hash)
{
    // @TODO : make sure it works
    MPForm form;

    KUrl removeUrl = KUrl("http://api.imgur.com/2/delete/");
    removeUrl.addPath(delete_hash + ".json");

    form.finish();

    KIO::TransferJob* job = KIO::http_post(removeUrl, form.formData(), KIO::HideProgressInfo);
    job->addMetaData("content-type", form.contentType());
    job->addMetaData("UserAgent", m_userAgent);

    m_state = IE_REMOVEPHOTO;

    emit signalBusy(true);

    return true;
}

void ImgurTalker::cancel()
{
    if (m_job)
    {
        m_job->kill();
        m_job = 0;
    }

    emit signalBusy(false);
}

//void ImgurTalker::startUpload()
//{
//    ImageCollection images = m_interface->currentSelection();

//    if (images.isValid())
//    {
//        KUrl::List list = images.images();
//        for (KUrl::List::ConstIterator it = list.begin(); it != list.end(); ++it)
//        {
//            KUrl imageUrl = *it;

//            imageUpload(imageUrl);
//            //kDebug() << images.images().at(i).pathOrUrl();
//        }
//    }
//}

void ImgurTalker::slotAddItems(const KUrl::List& list)
{
    if (list.count() == 0)
    {
        return;
    }

    kDebug() << "Appended" << list;
    m_queue->append(list);
}

KUrl::List* ImgurTalker::imageQueue() const
{
    return m_queue;
}

} // namespace KIPIImgurExportPlugin
