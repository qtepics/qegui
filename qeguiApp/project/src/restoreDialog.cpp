/*
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
 * This class is used to restore configurations
 */

#include "restoreDialog.h"
#include "ui_restoreDialog.h"
#include <QPushButton>

restoreDialog::restoreDialog( QStringList names, bool hasDefault, QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::restoreDialog)
{
    ui->setupUi(this);

    enableNamedItems( false );
    ui->namesListWidget->addItems( names );
    ui->defaultRadioButton->setEnabled( hasDefault );
    ui->namedRadioButton->setEnabled( names.count() );
}

restoreDialog::~restoreDialog()
{
    delete ui;
}

void restoreDialog::enableOpen()
{
    QPushButton* openButton = ui->buttonBox->button(QDialogButtonBox::Open);
    if( openButton )
    {
        openButton->setEnabled( ui->defaultRadioButton->isChecked() || ui->namesListWidget->selectedItems().count() );
    }
}

void restoreDialog::on_defaultRadioButton_clicked( bool )
{
    enableNamedItems( false );
    enableOpen();
}

void restoreDialog::on_namedRadioButton_clicked( bool )
{
    enableNamedItems( true );
    enableOpen();
}

void restoreDialog::enableNamedItems( bool enable )
{
    ui->namesListWidget->setEnabled( enable );
}

bool restoreDialog::getUseDefault()
{
    return ui->defaultRadioButton->isChecked();
}

QString restoreDialog::getName()
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

void restoreDialog::on_namesListWidget_doubleClicked( QModelIndex )
{
    accept();
}

void restoreDialog::on_namesListWidget_itemSelectionChanged()
{
    enableOpen();
}
