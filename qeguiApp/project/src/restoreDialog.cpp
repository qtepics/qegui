/*  restoreDialog.cpp
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
 * This class is used to restore configurations
 */

#include "restoreDialog.h"
#include "ui_restoreDialog.h"
#include <QPushButton>


//------------------------------------------------------------------------------
//
restoreDialog::restoreDialog( QStringList names, bool hasDefault, QWidget *parent ) :
    QEDialog(parent),
    ui(new Ui::restoreDialog)
{
    ui->setupUi(this);

    enableNamedItems( false );
    ui->namesListWidget->addItems( names );
    ui->defaultRadioButton->setEnabled( hasDefault );
    ui->namedRadioButton->setEnabled( names.count() );
}

//------------------------------------------------------------------------------
//
restoreDialog::~restoreDialog()
{
    delete ui;
}

//------------------------------------------------------------------------------
//
void restoreDialog::enableOpen()
{
    QPushButton* openButton = ui->buttonBox->button(QDialogButtonBox::Open);
    if( openButton )
    {
        openButton->setEnabled( ui->defaultRadioButton->isChecked() || ui->namesListWidget->selectedItems().count() );
    }
}

//------------------------------------------------------------------------------
//
void restoreDialog::on_defaultRadioButton_clicked( bool )
{
    enableNamedItems( false );
    enableOpen();
}

//------------------------------------------------------------------------------
//
void restoreDialog::on_namedRadioButton_clicked( bool )
{
    enableNamedItems( true );
    enableOpen();
}

//------------------------------------------------------------------------------
//
void restoreDialog::enableNamedItems( bool enable )
{
    ui->namesListWidget->setEnabled( enable );
}

//------------------------------------------------------------------------------
//
bool restoreDialog::getUseDefault() const
{
    return ui->defaultRadioButton->isChecked();
}

//------------------------------------------------------------------------------
//
QString restoreDialog::getName() const
{
    if( ui->namesListWidget->currentItem() )
    {
        return ui->namesListWidget->currentItem()->text();
    }
    else
    {
        return QString();
    }
}

//------------------------------------------------------------------------------
//
void restoreDialog::on_namesListWidget_doubleClicked( QModelIndex )
{
    accept();
}

//------------------------------------------------------------------------------
//
void restoreDialog::on_namesListWidget_itemSelectionChanged()
{
    enableOpen();
}

// end
