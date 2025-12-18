/*  recentFile.h
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  SPDX-FileCopyrightText: 2013-2025 Australian Synchrotron
 *  SPDX-License-Identifier: LGPL-3.0-only
 *
 *  Author:     Andrew Rhyder
 *  Maintainer: Andrew Starritt
 *  Contact:    andrews@ansto.gov.au
 */

/*
 * Description:
 *
 * The application keeps a list of recent files.
 * An insctance of the recentFile class is used to represent each recent file.
 * Since the recentFile class is based on QAction, the list of recent files can be directly
 * added to each main window 'Recent...' menu.
 */

#ifndef QEGUI_RECENT_FILE_H
#define QEGUI_RECENT_FILE_H

#include <QObject>
#include <QAction>
#include <QString>

#include <QLabel>
#include <QEWidget.h>
#include <QEString.h>
#include <QEStringFormatting.h>
#include <managePixmaps.h>
#include <QEStringFormattingMethods.h>
#include <QCaVariableNamePropertyManager.h>

class QEGui;

// Class used to hold recent file information
class recentFile : public QAction
{
    Q_OBJECT
public:
    explicit recentFile( const QString& nameIn, const QString& pathIn, const QStringList& pathListIn,
                         const QString& macroSubstitutionsIn, const QString& customisationNameIn,
                         QEGui* appIn );
    QString name;               // GUI title
    QString path;               // Full GUI file name
    QStringList pathList;       // Paths for locating other files
    QString macroSubstitutions; // Macro Substitutions
    QString customisationName;  // Window customisations

    QEGui* app;                 // Reference to main application

public slots:
    void recentSelected( bool );// Slot to act on selectino of this action in a 'Recent...' menu

};

#endif // QEGUI_RECENT_FILE_H
