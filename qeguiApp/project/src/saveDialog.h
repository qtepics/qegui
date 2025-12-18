/*  saveDialog.h
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

#ifndef SAVE_DIALOG_H
#define SAVE_DIALOG_H

#include <QEDialog.h>
#include <QListWidget>

namespace Ui {
    class saveDialog;
}

class saveDialog : public QEDialog
{
    Q_OBJECT
    
public:
    explicit saveDialog(QStringList names, QWidget *parent = 0);
    ~saveDialog();
    
    bool getUseDefault() const;
    QString getName() const;

private slots:
    void on_nameLineEdit_textChanged(QString );
    void on_namesListWidget_doubleClicked(QModelIndex index);
    void on_namesListWidget_clicked(QModelIndex index);

    void on_defaultRadioButton_clicked(bool checked);

    void on_namedRadioButton_clicked(bool checked);

private:
    Ui::saveDialog *ui;

    void enableNamedItems( bool enable );

    bool savingStartup;
    void enableSave();

};

#endif // SAVE_DIALOG_H
