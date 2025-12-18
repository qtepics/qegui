/*  loginDialog.h
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
  * A simple dialog containing a QELogin widget so the user can change the user level
  */

#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QEDialog.h>

class loginDialog  : public QEDialog
{
public:
    loginDialog();
};

#endif // LOGINDIALOG_H
