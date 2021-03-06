/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2011-05-23
 * Description : a plugin to create panorama by fusion of several images.
 * Acknowledge : based on the expoblending plugin
 *
 * Copyright (C) 2011 by Benjamin Girault <benjamin dot girault at gmail dot com>
 * Copyright (C) 2009-2012 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2009-2011 by Johannes Wienke <languitar at semipol dot de>
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
#include <kprocess.h>

// LibKDcraw includes

#include <libkdcraw/rawdecodingsettings.h>

// Local includes

#include "actions.h"
#include "ptotype.h"

using namespace KDcrawIface;

namespace KIPIPanoramaPlugin
{

struct ActionData;

class ActionThread : public QThread
{
    Q_OBJECT

public:

    typedef enum
    {
        JPEG,
        TIFF
    }
    PanoramaFileType;

public:

    explicit ActionThread(QObject* const parent);
    ~ActionThread();

    void setPreProcessingSettings(bool celeste, bool hdr, PanoramaFileType fileType,
                                  const RawDecodingSettings& settings);
    void preProcessFiles(const KUrl::List& urlList, const QString& cpcleanPath,
                         const QString& cpfindPath);
    void optimizeProject(const KUrl& ptoUrl, bool levelHorizon, bool optimizeProjectionAndSize,
                         const QString& autooptimiserPath);
    void generatePanoramaPreview(const KUrl& ptoUrl, const ItemUrlsMap& preProcessedUrlsMap,
                                 const QString& makePath, const QString& pto2mkPath,
                                 const QString& enblendPath, const QString& nonaPath);
    void compileProject(const KUrl& ptoUrl, const ItemUrlsMap& preProcessedUrlsMap,
                        PanoramaFileType fileType, const QString& makePath, const QString& pto2mkPath,
                        const QString& enblendPath, const QString& nonaPath);
    void copyFiles(const KUrl& ptoUrl, const KUrl& panoUrl, const KUrl& finalPanoUrl,
                   const ItemUrlsMap& preProcessedUrlsMap, bool savePTO);
    void cancel();

    /**
     * Clean up all temporary results produced so far.
     */
    void cleanUpResultFiles();

Q_SIGNALS:

    void starting(const KIPIPanoramaPlugin::ActionData& ad);
    void stepFinished();
    void finished(const KIPIPanoramaPlugin::ActionData& ad);

private:

    void    run();

    bool    startPreProcessing(const KUrl::List& inUrls, ItemUrlsMap& preProcessedUrlsMap,
                               const RawDecodingSettings& settings);
    bool    startCPFind(KUrl& ptoUrl, bool celeste, const QString& cpfindPath, QString& errors);
    bool    startCPClean(KUrl& ptoUrl, PTOType& ptoUrlData, const QString& cpcleanPath, QString& errors);
    bool    startOptimization(KUrl& ptoUrl, bool levelHorizon, bool optimizeProjectionAndSize,
                              const QString& autooptimiserPath, QString& errors);
    bool    computePanoramaPreview(KUrl& ptoUrl, KUrl& previewUrl, const ItemUrlsMap& preProcessedUrlsMap,
                                   const QString& makePath, const QString& pto2mkPath,
                                   const QString& enblendPath, const QString& nonaPath, QString& errors);
    bool    computePreview(const KUrl& inUrl, KUrl& outUrl);
    bool    convertRaw(const KUrl& inUrl, KUrl& outUrl, const RawDecodingSettings& settings);
    bool    isRawFile(const KUrl& url);
    bool    createPTO(bool hdr, PanoramaFileType fileType, const KUrl::List& inUrls,
                      const KIPIPanoramaPlugin::ItemUrlsMap& urlList, KUrl& ptoUrl);
    bool    createMK(KUrl& ptoUrl, KUrl& mkUrl, KUrl& panoUrl, PanoramaFileType fileType,
                     const QString& pto2mkPath, const QString& enblendPath,
                     const QString& nonaPath, QString& errors);
    bool    compileMK(KUrl& mkUrl, const QString& makePath, QString& errors);
    bool    compileMKStepByStep(KUrl& mkUrl, const KIPIPanoramaPlugin::ItemUrlsMap& urlList,
                                const QString& makePath, QString& errors);
    bool    copyFiles(const KUrl& panoUrl, const KUrl& finalPanoUrl, const KUrl& ptoUrl,
                      const ItemUrlsMap& urlList, bool savePTO, QString& errors);

    QString getProcessError(KProcess* proc) const;

private:

    struct ActionThreadPriv;
    ActionThreadPriv* const d;
};

}  // namespace KIPIPanoramaPlugin

#endif /* ACTIONTHREAD_H */
