/*  manageConfigDialog.h
 * 
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  Copyright (c) 2013-2019 Australian Synchrotron
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
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
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
