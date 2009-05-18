/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-10-01
 * Description : a kipi plugin to batch process images
 *
 * Copyright (C) 2004-2009 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef BATCHPROCESSIMAGESDIALOG_H
#define BATCHPROCESSIMAGESDIALOG_H

#define OVERWRITE_ASK    0
#define OVERWRITE_RENAME 1
#define OVERWRITE_SKIP   2
#define OVERWRITE_OVER   3

#define NO_PROCESS       0
#define UNDER_PROCESS    1
#define PROCESS_DONE     2
#define STOP_PROCESS     3

// Qt includes

#include <QString>

// KDE includes

#include <kdialog.h>
#include <kio/job.h>
#include <kurl.h>

// LibKIPI includes

#include <libkipi/interface.h>

// Local includes

#include "batchprocessimagesitem.h"
#include "batchprocessimageslist.h"

class QGroupBox;
class QCheckBox;
class QFileInfo;
class QLabel;
class QProgressBar;

class KComboBox;
class KProcess;
class KUrlRequester;

namespace KIPIBatchProcessImagesPlugin
{

class BatchProcessImagesDialog : public KDialog
{
Q_OBJECT

 public:
   // Don't forget to add the 'm_Type' and 'm_labelType' implementation in the constructor of
   // children dialog class.

   BatchProcessImagesDialog( KUrl::List urlList, KIPI::Interface* interface, QString caption, QWidget *parent=0 );
   ~BatchProcessImagesDialog();

protected slots:
    void slotProcessStop(void);

private slots:

   // --------------------------------------------------------------------------------------------------------
   // Standards slots

    void slotOk(void);

    void slotProcessStart(void);
    void slotReadyRead();
    void slotFinished();

    void slotPreview(void);
    void slotPreviewFinished();
    void slotPreviewReadyRead();
    void slotPreviewStop(void);

    void slotListDoubleClicked(QTreeWidgetItem *itemClicked);

    void slotAddDropItems(QStringList filesPath);

    void slotImageSelected( QTreeWidgetItem * item );
    void slotGotPreview(const KFileItem&, const QPixmap &pixmap);

    void slotImagesFilesButtonAdd(void);
    void slotImagesFilesButtonRem(void);

   // --------------------------------------------------------------------------------------------------------
   // Standards virtuals slots for re-implementation

    virtual void slotHelp(void){};                             // Called when 'Help' menu option is actived.
    virtual void slotOptionsClicked(void){};                   // Called when 'Options' button is clicked.
    virtual void slotTypeChanged(int){};                       // Called when the current type option is changed.

private:

    void enableWidgets(bool state);

   // --------------------------------------------------------------------------------------------------------
   // Standards widgets in the dialog

   QLabel                 *m_labelOverWrite;
   QLabel                 *m_imageLabel;

   QProgressBar           *m_progress;

   QGroupBox              *groupBox2;
   QGroupBox              *groupBox3;
   QGroupBox              *groupBox4;
   QGroupBox              *groupBox41;

   QPushButton            *m_addImagesButton;
   QPushButton            *m_remImagesButton;

protected:
   // --------------------------------------------------------------------------------------------------------
   // Widgets used by inherited dialogs
   QGroupBox              *groupBox1;
   QLabel                 *m_labelType;
   KComboBox              *m_Type;
   QCheckBox              *m_smallPreview;
   KComboBox              *m_overWriteMode;
   QCheckBox              *m_removeOriginal;
   QPushButton            *m_previewButton;
   QPushButton            *m_optionsButton;

   BatchProcessImagesList *m_listFiles;

   KUrlRequester          *m_destinationURL;
   ////

   KConfig                *m_config;

   int                     m_convertStatus;
   int                     m_progressStatus;
   int                     m_nbItem;

   KProcess               *m_ProcessusProc;
   KProcess               *m_PreviewProc;

   QTreeWidgetItemIterator *m_listFile2Process_iterator;

   QString                 whatsThis;
   QString                 m_ImagesFilesSort;
   QString                 m_newDir;
   QString                 m_commandLine;
   QString                 m_tmpFolder;
   QString                 m_previewOutput;
   KUrl::List              m_selectedImageFiles;

   KIPI::Interface        *m_interface;

   // --------------------------------------------------------------------------------------------------------
   // Standards fonctions

   // Fonctions for ImageMAgick call implementations.

   void    processAborted(bool removeFlag=false); // Called when ImageMagick process is stopped by user.
   void    endPreview(void);                      // Called when preview process is done or stopped by user.

   // Called for to parse the files overwriting mode selected by user.

   int     overwriteMode(void);

   // Called for rename the target current image file name if already exist.

   QString RenameTargetImageFile(QFileInfo *fi);

   // --------------------------------------------------------------------------------------------------------
   // Standard virtual functions for re-implementation.

   // Called when ImageMagick process is started or for another re-implementation without
   // ImageMagick calls (like 'RenameImages' plugin).

   virtual bool startProcess(void);

   // Called when ImageMagick process is done or stopped by user or for another re-implementation without
   // ImageMagick calls (like 'RenameImages' plugin).

   virtual void endProcess(void);

   //  Functions for ImageMagick implementation calls only.

   // Called by 'startProcess' function at start. You can checking here some things about the current
   // 'item' before the 'startprocess' function execution.

    virtual bool prepareStartProcess(BatchProcessImagesItem* /*item*/, const QString& /*albumDest*/) { return true; };

   // Called to construct the ImageMagick command line used for to process or preview the image traitements.
   // If 'albumDest' = 0L ==> preview process.

    virtual void initProcess(KProcess* /*proc*/, BatchProcessImagesItem* /*item*/,
                                const QString& /*albumDest*/ = QString::null,
                                bool  /*previewMode*/ = false) = 0;

    virtual void processDone(){};

   // Functions used for to read and to save the settings in the configuration file.

   virtual void readSettings(void){};
   virtual void saveSettings(void){};

   // Called for add or update the list of images to process in the dialog.

   virtual void listImageFiles(void);

   // Called by 'listImageFiles' for renamed if necessary the target image file name.

   virtual QString oldFileName2NewFileName(QString fileName)
           { return (fileName); };
};

}  // namespace KIPIBatchProcessImagesPlugin

#endif  // BATCHPROCESSIMAGESDIALOG_H
