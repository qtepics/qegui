/*  loginDialog.cpp
 *
 *  This file is part of the EPICS QT Framework, initially developed at the Australian Synchrotron.
 *
 *  The EPICS QT Framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The EPICS QT Framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with the EPICS QT Framework.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright (c) 2013 Australian Synchrotron
 *
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
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
