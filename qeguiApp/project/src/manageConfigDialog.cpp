/*  manageConfigDialog.cpp
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
 * This class is used to manage the save/restore configurations
 */

#include "manageConfigDialog.h"
#include "ui_manageConfigDialog.h"
#include "persistanceManager.h"

//------------------------------------------------------------------------------
// Present the dialog to the user
manageConfigDialog::manageConfigDialog( QStringList names, bool hasDefault, QWidget *parent ) :
    QEDialog( parent ),
    ui( new Ui::manageConfigDialog )
{
    ui->setupUi(this);
    ui->namesListWidget->addItems( names );
    ui->deletePushButton->setEnabled( false );
    ui->deleteDefaultPushButton->setEnabled( hasDefault );
}

//------------------------------------------------------------------------------
// Remove the dialog
manageConfigDialog::~manageConfigDialog()
{
    delete ui;
}

//------------------------------------------------------------------------------
// The user has changed the configurations selected
void manageConfigDialog::on_namesListWidget_itemSelectionChanged()
{
    ui->deletePushButton->setEnabled( ui->namesListWidget->selectedItems().count() );
}

//------------------------------------------------------------------------------
// The user pressed "Delete" named configurations
void manageConfigDialog::on_deletePushButton_clicked()
{
    QStringList names;
    QList<QListWidgetItem*> list = ui->namesListWidget->selectedItems();
    for( int i = 0; i < list.count(); i++ )
    {
        names.append( list.at( i )->text() );
    }
    emit deleteConfigs( this, names );
    ui->namesListWidget->clear();
    ui->namesListWidget->addItems( currentNames );
}

//------------------------------------------------------------------------------
// Update the list of current names (when initialising the dialog, or after deleting configurations)
void manageConfigDialog::setCurrentNames( QStringList currentNamesIn )
{
    currentNames = currentNamesIn;
}

//------------------------------------------------------------------------------
// User is deleting the default configuration
void manageConfigDialog::on_deleteDefaultPushButton_clicked()
{
    QStringList names;
    names.append( PersistanceManager::defaultName );
    emit deleteConfigs( this, names );
    ui->deleteDefaultPushButton->setEnabled( false );
}

// end
