/* ============================================================
 * File  : singledialog.cpp
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Date  : 2003-10-22
 * Description : 
 * 
 * Copyright 2003 by Renchi Raju

 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * ============================================================ */

#include <qpushbutton.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qvbuttongroup.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qtimer.h>
#include <qfileinfo.h>
#include <qevent.h>

#include <klocale.h>
#include <kfiledialog.h>
#include <kmessagebox.h>
#include <kconfig.h>
#include <kapplication.h>

extern "C"
{
#include <stdio.h>
}

#include "singledialog.h"
#include "previewwidget.h"
#include "cspinbox.h"
#include "processcontroller.h"

namespace KIPIRawConverterPlugin
{

SingleDialog::SingleDialog(const QString& file)
    : QDialog(0,0,false,Qt::WDestructiveClose)
{

    setCaption(i18n("KIPI Raw Image Converter"));
    
    // --------------------------------------------------------------

    inputFile_     = file;
    inputFileName_ = QFileInfo(file).fileName();

    QGridLayout *mainLayout = new QGridLayout(this,5,2,5);
        
    // --------------------------------------------------------------

    QGroupBox *lbox = new QGroupBox(i18n("Image Preview"), this);
    lbox->setColumnLayout(0, Qt::Vertical);
    lbox->layout()->setSpacing( 6 );
    lbox->layout()->setMargin( 11 );
    QVBoxLayout* lboxLayout =
        new QVBoxLayout(lbox->layout());
    
    previewWidget_ = new PreviewWidget(lbox);
    lboxLayout->addWidget(previewWidget_);

    mainLayout->addMultiCellWidget(lbox, 0, 2, 0, 0);

    // ---------------------------------------------------------------
    
    QGroupBox *settingsBox = new QGroupBox(i18n("Settings"), this);
    settingsBox->setColumnLayout(0, Qt::Vertical);
    settingsBox->layout()->setSpacing( 6 );
    settingsBox->layout()->setMargin( 11 );
    QVBoxLayout* settingsBoxLayout =
        new QVBoxLayout(settingsBox->layout());

    // ---------------------------------------------------------------

    cameraWBCheckBox_ = new QCheckBox(i18n("Use Camera White Balance"),
                                      settingsBox);
    QToolTip::add(cameraWBCheckBox_,
                  i18n("Use the camera's custom white-balance settings.\n"
                       "The default  is to use fixed daylight values,\n"
                       "calculated from sample images."));
    settingsBoxLayout->addWidget(cameraWBCheckBox_);

    fourColorCheckBox_ = new QCheckBox(i18n("Four Color RGBG"), settingsBox);
    QToolTip::add(fourColorCheckBox_,
                  i18n("Interpolate RGB as four colors.\n"
                       "The default is to assume that all green\n"
                       "pixels are the same. If even-row green\n"
                       "pixels are more sensitive to ultraviolet light\n"
                       "than odd-row this difference causes a mesh\n"
                       "pattern in the output; using this option solves\n"
                       "this problem with minimal loss of detail."));
    settingsBoxLayout->addWidget(fourColorCheckBox_);

    QHBoxLayout *hboxLayout;

    // ---------------------------------------------------------------
    
    hboxLayout = new QHBoxLayout(0,0,6,"layout1");
    gammaSpinBox_ = new CSpinBox(settingsBox);
    gammaSpinBox_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    hboxLayout->addWidget(gammaSpinBox_);
    hboxLayout->addWidget(new QLabel(i18n("Gamma"), settingsBox));
    QToolTip::add(gammaSpinBox_,
                  i18n("Specify the gamma value"));
    settingsBoxLayout->addLayout(hboxLayout);
    
    // ---------------------------------------------------------------

    hboxLayout = new QHBoxLayout(0,0,6,"layout2");
    brightnessSpinBox_ = new CSpinBox(settingsBox);
    brightnessSpinBox_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    hboxLayout->addWidget(brightnessSpinBox_);
    hboxLayout->addWidget(new QLabel(i18n("Brightness"), settingsBox));
    QToolTip::add(brightnessSpinBox_,
                  i18n("Specify the output brightness"));
    settingsBoxLayout->addLayout(hboxLayout);

    // ---------------------------------------------------------------

    hboxLayout = new QHBoxLayout(0,0,6,"layout3");
    redSpinBox_ = new CSpinBox(settingsBox);
    redSpinBox_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QToolTip::add(redSpinBox_,
                  i18n("After all other color adjustments,\n"
                       "multiply the red channel by this value"));

    hboxLayout->addWidget(redSpinBox_);
    hboxLayout->addWidget(new QLabel(i18n("Red Multiplier"), settingsBox));
    settingsBoxLayout->addLayout(hboxLayout);

    // ---------------------------------------------------------------
    
    hboxLayout = new QHBoxLayout(0,0,6,"layout4");
    blueSpinBox_ = new CSpinBox(settingsBox);
    blueSpinBox_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QToolTip::add(blueSpinBox_,
                  i18n("After all other color adjustments,\n"
                       "multiply the blue channel by this value"));

    hboxLayout->addWidget(blueSpinBox_);
    hboxLayout->addWidget(new QLabel(i18n("Blue Multiplier"), settingsBox));
    settingsBoxLayout->addLayout(hboxLayout);

    // ---------------------------------------------------------------

    saveButtonGroup_ = new QVButtonGroup(i18n("Save Format"),this);
    saveButtonGroup_->setRadioButtonExclusive(true);

    QRadioButton *radioButton;
    radioButton = new QRadioButton("JPEG",saveButtonGroup_);
    QToolTip::add(radioButton,
                  i18n("Output the processed image in JPEG Format.\n"
                       "This is a lossy format, but will give\n"
                       "smaller-sized files"));
    radioButton->setChecked(true);

    radioButton = new QRadioButton("TIFF",saveButtonGroup_);
    QToolTip::add(radioButton,
                  i18n("Output the processed image in TIFF Format.\n"
                       "This generates larges, without\n"
                       "losing quality"));

    radioButton = new QRadioButton("PPM",saveButtonGroup_);
    QToolTip::add(radioButton,
                  i18n("Output the processed image in PPM Format.\n"
                       "This generates the largest files, without\n"
                       "losing quality"));

    // ---------------------------------------------------------------

    mainLayout->addWidget(settingsBox, 0, 1);
    mainLayout->addWidget(saveButtonGroup_, 1, 1);
    mainLayout->addItem(new QSpacerItem(10,10,
                                        QSizePolicy::Minimum,
                                        QSizePolicy::Expanding), 2, 1);
    
    // ---------------------------------------------------------------

    QFrame *hline = new QFrame(this);
    hline->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    mainLayout->addMultiCellWidget(hline, 3, 3, 0, 1);

    // ---------------------------------------------------------------

    hboxLayout = new QHBoxLayout(0,0,6);

    hboxLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,
                                        QSizePolicy::Minimum));

    helpButton_ = new QPushButton(i18n("&Help"), this);
    hboxLayout->addWidget(helpButton_);

    aboutButton_ = new QPushButton(i18n("About"), this);
    hboxLayout->addWidget(aboutButton_);

    previewButton_ = new QPushButton(i18n("&Preview"), this);
    QToolTip::add(previewButton_,
                  i18n("Generate a Preview from current settings.\n"
                       "Uses a simple bilinear interpolation for\n"
                       "quick results."));
    hboxLayout->addWidget(previewButton_);

    processButton_ = new QPushButton(i18n("P&rocess"), this);
    QToolTip::add(processButton_,
                  i18n("Convert the Raw Image from current settings.\n"
                       "This uses a high-quality adaptive algorithm."));
    hboxLayout->addWidget(processButton_);

    abortButton_ = new QPushButton(i18n("&Abort"), this);
    QToolTip::add(abortButton_, i18n("Abort the current operation"));
    hboxLayout->addWidget(abortButton_);

    closeButton_ = new QPushButton(i18n("&Close"), this);
    QToolTip::add(closeButton_, i18n("Exit Raw Converter"));
    hboxLayout->addWidget(closeButton_);

    
    mainLayout->addMultiCellLayout(hboxLayout, 4, 4, 0, 1);

    // ---------------------------------------------------------------

    connect(helpButton_, SIGNAL(clicked()),
            SLOT(slotHelp()));
    connect(aboutButton_, SIGNAL(clicked()),
            SLOT(slotAbout()));
    connect(previewButton_, SIGNAL(clicked()),
            SLOT(slotPreview()));
    connect(processButton_, SIGNAL(clicked()),
            SLOT(slotProcess()));
    connect(closeButton_, SIGNAL(clicked()),
            SLOT(slotClose()));
    connect(abortButton_, SIGNAL(clicked()),
            SLOT(slotAbort()));

    // ---------------------------------------------------------------

    controller_ = new ProcessController(this);

    connect(controller_,
            SIGNAL(signalIdentified(const QString&, const QString&)),
            SLOT(slotIdentified(const QString&, const QString&)));
    connect(controller_,
            SIGNAL(signalIdentifyFailed(const QString&, const QString&)),
            SLOT(slotIdentifyFailed(const QString&, const QString&)));
    connect(controller_,
            SIGNAL(signalPreviewing(const QString&)),
            SLOT(slotPreviewing(const QString&)));
    connect(controller_,
            SIGNAL(signalPreviewed(const QString&, const QString&)),
            SLOT(slotPreviewed(const QString&, const QString&)));
    connect(controller_,
            SIGNAL(signalPreviewFailed(const QString&)),
            SLOT(slotPreviewFailed(const QString&)));
    connect(controller_,
            SIGNAL(signalProcessing(const QString&)),
            SLOT(slotProcessing(const QString&)));
    connect(controller_,
            SIGNAL(signalProcessed(const QString&, const QString&)),
            SLOT(slotProcessed(const QString&, const QString&)));
    connect(controller_,
            SIGNAL(signalProcessingFailed(const QString&)),
            SLOT(slotProcessingFailed(const QString&)));
    connect(controller_,
            SIGNAL(signalBusy(bool)), SLOT(slotBusy(bool)));

    // ---------------------------------------------------------------

    slotBusy(false);

    readSettings();

    QTimer::singleShot(0, this, SLOT( slotIdentify() ) );
}

SingleDialog::~SingleDialog()
{
    saveSettings();
}

void SingleDialog::closeEvent(QCloseEvent *e)
{
    if (!e) return;
    if (abortButton_->isEnabled()) {
        qWarning("close?");
    }
    e->accept();
}

void SingleDialog::readSettings()
{
    KConfig* config=kapp->config();

    config->setGroup("RawConverter Settings");

    gammaSpinBox_->setValue(config->readNumEntry("Gamma", 8));
    brightnessSpinBox_->setValue(config->readNumEntry("Brightness",10));

    redSpinBox_->setValue(config->readNumEntry("Red Scale",10));
    blueSpinBox_->setValue(config->readNumEntry("Blue Scale",10));

    cameraWBCheckBox_->setChecked(config->readBoolEntry("Use Camera WB", true));
    fourColorCheckBox_->setChecked(config->readBoolEntry("Four Color RGB", false));

    saveButtonGroup_->setButton(config->readNumEntry("Output Format", 0));
}

void SingleDialog::saveSettings()
{
    KConfig* config=kapp->config();

    config->setGroup("RawConverter Settings");
    
    config->writeEntry("Gamma", gammaSpinBox_->value());
    config->writeEntry("Brightness", brightnessSpinBox_->value());

    config->writeEntry("Red Scale", redSpinBox_->value());
    config->writeEntry("Blue Scale", blueSpinBox_->value());

    config->writeEntry("Use Camera WB", cameraWBCheckBox_->isChecked());
    config->writeEntry("Four Color RGB", fourColorCheckBox_->isChecked());

    config->writeEntry("Output Format",
                       saveButtonGroup_->id(saveButtonGroup_->selected()));
    
    config->sync();
}

void SingleDialog::slotHelp()
{
    KApplication::kApplication()->invokeHelp("rawconverter",
                                             "kipi-plugins");
}

void SingleDialog::slotAbout()
{
    KMessageBox::about(this, i18n("A KIPI plugin for RAW image conversion\n\n"
                                "Author: Renchi Raju\n\n"
                                "Email: renchi@pooh.tam.uiuc.edu\n\n"
                                "This plugin uses the Dave Coffin RAW photo decoder program \"dcraw\""),
                                i18n("About RAW image converter"));
}

void SingleDialog::slotPreview()
{
    Settings& s      = controller_->settings;
    s.cameraWB       = cameraWBCheckBox_->isChecked();
    s.fourColorRGB   = fourColorCheckBox_->isChecked();
    s.gamma          = gammaSpinBox_->value()/10.0;
    s.brightness     = brightnessSpinBox_->value()/10.0;
    s.redMultiplier  = redSpinBox_->value()/10.0;
    s.blueMultiplier = blueSpinBox_->value()/10.0;
    s.outputFormat   = saveButtonGroup_->selected()->text();

    controller_->preview(inputFile_);
}

void SingleDialog::slotProcess()
{
    Settings& s      = controller_->settings;
    s.cameraWB       = cameraWBCheckBox_->isChecked();
    s.fourColorRGB   = fourColorCheckBox_->isChecked();
    s.gamma          = gammaSpinBox_->value()/10.0;
    s.brightness     = brightnessSpinBox_->value()/10.0;
    s.redMultiplier  = redSpinBox_->value()/10.0;
    s.blueMultiplier = blueSpinBox_->value()/10.0;
    s.outputFormat   = saveButtonGroup_->selected()->text();

    controller_->process(inputFile_);
}

void SingleDialog::slotIdentify()
{
    controller_->identify(inputFile_);
}

void SingleDialog::slotClose()
{
    close();    
}

void SingleDialog::slotAbort()
{
    controller_->abort();
}

void SingleDialog::slotBusy(bool val)
{
    abortButton_->setEnabled(val);
    closeButton_->setEnabled(!val);
    previewButton_->setEnabled(!val);
    processButton_->setEnabled(!val);
}

void SingleDialog::slotIdentified(const QString&, const QString& identity)
{
    previewWidget_->setText(inputFileName_ + QString(" : ") +
                            identity);
}

void SingleDialog::slotIdentifyFailed(const QString&, const QString& identity)
{
    previewWidget_->setText(i18n("Failed to identify raw image\n")
                            + identity);
}

void SingleDialog::slotPreviewing(const QString&)
{
    previewWidget_->setText(i18n("Generating Preview ..."));
}

void SingleDialog::slotPreviewed(const QString&, const QString& tmpFile_)
{
    previewWidget_->load(tmpFile_);
}

void SingleDialog::slotPreviewFailed(const QString&)
{
    previewWidget_->setText(i18n("Failed to generate preview"));
}

void SingleDialog::slotProcessing(const QString&)
{
    previewWidget_->setText(i18n("Converting Raw Image ..."));
}

void SingleDialog::slotProcessed(const QString&, const QString& tmpFile_)
{
    previewWidget_->load(tmpFile_);
    QString filter("*.");
    filter += saveButtonGroup_->selected()->text().lower();
    QString saveFile =
        KFileDialog::getSaveFileName(QFileInfo(inputFile_).dirPath(true),
                                     filter, this);
    
    if (saveFile.isEmpty()) return;

    if (::rename(tmpFile_.latin1(), saveFile.latin1()) != 0)
    {
        KMessageBox::error(this, i18n("Failed to save image ")
                           + saveFile);
    }
                                 
}

void SingleDialog::slotProcessingFailed(const QString&)
{
    previewWidget_->setText(i18n("Failed to convert raw image"));
}

} // NameSpace KIPIRawConverterPlugin

#include "singledialog.moc"
