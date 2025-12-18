/*  restoreDialog.h
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

#ifndef RESTORE_DIALOG_H
#define RESTORE_DIALOG_H

#include <QEDialog.h>
#include <QListWidget>

namespace Ui {
    class restoreDialog;
}

class restoreDialog : public QEDialog
{
    Q_OBJECT

public:
    explicit restoreDialog( QStringList names, bool hasDefault, QWidget *parent = 0 );
    ~restoreDialog();

    bool getUseDefault() const;
    QString getName() const;

private:
    Ui::restoreDialog *ui;

    void enableNamedItems( bool enable );
    void enableOpen();

private slots:
    void on_namesListWidget_itemSelectionChanged();
    void on_namesListWidget_doubleClicked(QModelIndex index);
    void on_namedRadioButton_clicked(bool checked);
    void on_defaultRadioButton_clicked(bool checked);
};

#endif // RESTORE_DIALOG_H
