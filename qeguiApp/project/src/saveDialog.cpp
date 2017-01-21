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
 * This class is used to save configurations
 */

#include <saveDialog.h>
#include <ui_saveDialog.h>
#include <QDebug>
#include <QPushButton>

saveDialog::saveDialog( QStringList names, QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::saveDialog)
{
    ui->setupUi(this);

    savingStartup = true;
    enableNamedItems( false );
    ui->namesListWidget->addItems( names );
}

saveDialog::~saveDialog()
{
    delete ui;
}

void saveDialog::enableSave()
{
    QPushButton* saveButton = ui->buttonBox->button(QDialogButtonBox::Save);
    if( saveButton )
    {
        saveButton->setEnabled( ui->defaultRadioButton->isChecked() || !ui->nameLineEdit->text().isEmpty() );
    }
}

void saveDialog::on_defaultRadioButton_clicked( bool )
{
    enableNamedItems( false );
    savingStartup = true;
    enableSave();
}

void saveDialog::on_namedRadioButton_clicked( bool )
{
    enableNamedItems( true );
    savingStartup = false;
    enableSave();
}

void saveDialog::enableNamedItems( bool enable )
{
    ui->namesListWidget->setEnabled( enable );
    ui->nameLineEdit->setEnabled( enable );
}

void saveDialog::on_namesListWidget_clicked(QModelIndex)
{
    if( ui->namesListWidget->currentItem() )
    {
        ui->nameLineEdit->setText( ui->namesListWidget->currentItem()->text() );
    }
}

bool saveDialog::getUseDefault()
{
    return ui->defaultRadioButton->isChecked();
}

QString saveDialog::getName()
{
    if( ui->namedRadioButton->isChecked() )
    {
        return ui->nameLineEdit->text();
    }
    return "";
}

void saveDialog::on_namesListWidget_doubleClicked( QModelIndex )
{
    accept();
}

void saveDialog::on_nameLineEdit_textChanged(QString )
{
    enableSave();
}
