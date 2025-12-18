/*  manageConfigDialog.h
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

#ifndef MANAGE_CONFIG_DIALOG_H
#define MANAGE_CONFIG_DIALOG_H

#include <QEDialog.h>

namespace Ui {
    class manageConfigDialog;
}

class manageConfigDialog : public QEDialog
{
    Q_OBJECT

public:
    explicit manageConfigDialog( QStringList names, bool hasDefault, QWidget *parent = 0 );
    ~manageConfigDialog();
    void setCurrentNames( QStringList currentNamesIn );


private:
    Ui::manageConfigDialog *ui;
    QStringList currentNames;

signals:
    void deleteConfigs( manageConfigDialog* mcd, const QStringList names );

private slots:
    void on_deleteDefaultPushButton_clicked();
    void on_deletePushButton_clicked();
    void on_namesListWidget_itemSelectionChanged();
};

#endif // MANAGE_CONFIG_DIALOG_H
