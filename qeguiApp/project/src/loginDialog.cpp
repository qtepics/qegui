/*  loginDialog.cpp
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
 * This class is used to change the current user level
 * It uses a QELogin widget to manage the task
 */

#include <QVBoxLayout>
#include "loginDialog.h"
#include <QELogin.h>

loginDialog::loginDialog()
{
    // Create the login widget
    QELogin* login = new QELogin();
    login->setCompactStyle( false );
    login->setFrameStyle( QFrame::NoFrame );
    QObject::connect(login, SIGNAL(login()), this, SLOT(accept()));

    // Create the cancel button
    QPushButton* cancelButton = new QPushButton( this );
    cancelButton->setText( "Cancel" );
    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    // Add the login widget to the the dialog
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget( login );
    layout->addWidget( cancelButton );

    // Give the QELogin widget the focus so an enter pressed after entering the password will change the user level
    login->setFocus();

    // Set the dialog title
    setWindowTitle( "Change User Level" );

    // Explicity set the desired size. If we don't and then apply scaling, the sensible
    // default sizing "goes out the window"..
    //
    QRect g = geometry();
    g.setWidth( 200 );
    g.setHeight( 280 );
    setGeometry( g );
}

// end
