/*  saveDialog.cpp
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
 * This class is used to save configurations
 */

#include <saveDialog.h>
#include <ui_saveDialog.h>
#include <QDebug>
#include <QPushButton>

//------------------------------------------------------------------------------
//
saveDialog::saveDialog( QStringList names, QWidget *parent ) :
    QEDialog(parent),
    ui(new Ui::saveDialog)
{
    ui->setupUi(this);

    savingStartup = true;
    enableNamedItems( false );
    ui->namesListWidget->addItems( names );
}

//------------------------------------------------------------------------------
//
saveDialog::~saveDialog()
{
    delete ui;
}

//------------------------------------------------------------------------------
//
void saveDialog::enableSave()
{
    QPushButton* saveButton = ui->buttonBox->button(QDialogButtonBox::Save);
    if( saveButton )
    {
        saveButton->setEnabled( ui->defaultRadioButton->isChecked() ||
                                !ui->nameLineEdit->text().isEmpty() );
    }
}

//------------------------------------------------------------------------------
//
void saveDialog::on_defaultRadioButton_clicked( bool )
{
    enableNamedItems( false );
    savingStartup = true;
    enableSave();
}

//------------------------------------------------------------------------------
//
void saveDialog::on_namedRadioButton_clicked( bool )
{
    enableNamedItems( true );
    savingStartup = false;
    enableSave();
}

//------------------------------------------------------------------------------
//
void saveDialog::enableNamedItems( bool enable )
{
    ui->namesListWidget->setEnabled( enable );
    ui->nameLineEdit->setEnabled( enable );
}

//------------------------------------------------------------------------------
//
void saveDialog::on_namesListWidget_clicked(QModelIndex)
{
    if( ui->namesListWidget->currentItem() )
    {
        ui->nameLineEdit->setText( ui->namesListWidget->currentItem()->text() );
    }
}

//------------------------------------------------------------------------------
//
bool saveDialog::getUseDefault() const
{
    return ui->defaultRadioButton->isChecked();
}

//------------------------------------------------------------------------------
//
QString saveDialog::getName() const
{
    if( ui->namedRadioButton->isChecked() )
    {
        return ui->nameLineEdit->text();
    }
    return "";
}

//------------------------------------------------------------------------------
//
void saveDialog::on_namesListWidget_doubleClicked( QModelIndex )
{
    accept();
}

//------------------------------------------------------------------------------
//
void saveDialog::on_nameLineEdit_textChanged(QString )
{
    enableSave();
}

// end
